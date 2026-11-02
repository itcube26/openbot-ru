// Required App Version: 0.7.0
// ---------------------------------------------------------------------------
// OpenBot + VEX V5 Smart Motors (ESP32)
//
// Самодостаточный скетч: телефон управляет роботом по Bluetooth,
// моторы VEX V5 подключены через V5 Motor Controller по I2C.
//
// Идея проекта OpenBot сохранена полностью:
//  - канал связи с телефоном — BLE UART, тот же профиль и те же UUID
//    сервисов/характеристик, что в openbot.ino и в инструкции
//    esp32_buildin/Инструкция по управлению по bluetooth.md; приложение
//    OpenBot подключается к роботу как обычно (режим Bluetooth);
//  - низкоуровневое управление транспортным средством: вместо ШИМ-драйверов
//    постоянного тока те же значения ctrl_left / ctrl_right передаются на
//    порты контроллера VEX V5 по I2C (протокол — см. vexv5_examples/);
//  - тот же протокол сообщений, что и в openbot.ino: заголовок-буква +
//    тело до символа '\n' ('c' — команды движения, 'f' — конфигурация);
//  - отправка телеметрии (odom, battery, speed) обратно в приложение.
//
// Проводка:
//   ESP32 GPIO21 (SDA) -> V5 Motor Controller SDA
//   ESP32 GPIO22 (SCL) -> V5 Motor Controller SCL
//   Левые моторы VEX V5 -> порт 1, правые -> порт 20
//   Общий источник питания 7.2..12 В, GND соединён с ESP32.
//
// Приложение: OpenBot Android, режим "Bluetooth" (не USB), выберите
// устройство "OpenBot VEX V5".
//
// Board: "ESP32 Dev Module" (esp32 board package 2.0.17).
// ---------------------------------------------------------------------------

#include <Wire.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

//------------------------------------------------------//
// НАСТРОЙКИ
//------------------------------------------------------//
#define DEBUG 1                 // отладочный вывод в Serial Monitor

// I2C-подключение контроллера VEX V5
const uint8_t V5_CONTROLLER_ADDR = 0x3c;  // адрес V5 Motor Controller
const int PIN_V5_I2C_SDA = 21;
const int PIN_V5_I2C_SCL = 22;
const uint8_t V5_PORT_LEFT = 1;   // левые моторы вставлены в порт 1
const uint8_t V5_PORT_RIGHT = 20; // правые моторы вставлены в порт 20

// Ограничение скорости: ctrl из приложения приходит в диапазоне ±255,
// контроллер V5 принимает ±127.
const int CTRL_MAX = 255;
const int V5_MAX = 127;

// Пины обратной связи (при наличии датчиков)
const int PIN_SPEED_LF = 35;  // передний левый оптический датчик (input-only)
const int PIN_SPEED_RF = 22;  // передний правый оптический датчик
const int PIN_VIN = 34;       // напряжение батареи через делитель (input-only)
const float ADC_FACTOR = 3.3 / 4095;
const float VOLTAGE_DIVIDER_FACTOR = (30 + 10) / 10;  // (R1+R2)/R2

const char *BT_DEVICE_NAME = "OpenBot VEX V5";

// BLE UART — те же UUID, что в openbot.ino и приложении OpenBot
const char *SERVICE_UUID = "61653dc3-4021-4d1e-ba83-8b4eec61d613";
const char *CHARACTERISTIC_UUID_RX = "06386c14-86ea-4d71-811c-48f97c58f8c9";
const char *CHARACTERISTIC_UUID_TX = "9bf1103b-834c-47cf-b149-c9e4bcf778a7";

//------------------------------------------------------//
// СОСТОЯНИЕ
//------------------------------------------------------//
BLEServer *bleServer = NULL;
BLECharacteristic *pTxCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;

int ctrl_left = 0;     // команда левого мотора, -255..255
int ctrl_right = 0;    // команда правого мотора, -255..255

unsigned long counter_lf = 0;  // счётчики импульсов колёс (одометрия)
unsigned long counter_rf = 0;

volatile bool newLeftSpeed = false;
volatile bool newRightSpeed = false;

uint8_t signalLeftSpeed = 0;   // состояние сигнала для сообщения odom
uint8_t signalRightSpeed = 0;

float battery_voltage = 0;
unsigned long last_send = 0;
const unsigned long SEND_INTERVAL = 20;  // мс, как в openbot.ino (50 Гц)

enum msgParts { HEADER, BODY };
msgParts msgPart = HEADER;
char header;
const char endChar = '\n';
const char MAX_MSG_SZ = 60;
char msg_buf[MAX_MSG_SZ] = "";
int msg_idx = 0;

//------------------------------------------------------//
// МОТОРЫ VEX V5 (I2C) — протокол из vexv5_examples
//------------------------------------------------------//
// Каждый порт управляется парой регистров: (port*2) — старший байт,
// (port*2+1) — младший. 16-битное значение:
//   bit 15    : направление (1 = реверс)
//   bits 6-0  : скорость 0..127
void v5_set_motor(uint8_t port, int speed) {
  if (speed > V5_MAX) speed = V5_MAX;
  if (speed < -V5_MAX) speed = -V5_MAX;
  uint16_t reg = 0;
  if (speed < 0) reg |= 0x8000;
  reg |= (uint16_t)abs(speed);
  Wire.beginTransmission(V5_CONTROLLER_ADDR);
  Wire.write((uint8_t)(port * 2));
  Wire.write((uint8_t)(reg >> 8));
  Wire.write((uint8_t)(reg & 0xff));
  Wire.endTransmission();
}

void update_vehicle() {
  v5_set_motor(V5_PORT_LEFT, ctrl_left * V5_MAX / CTRL_MAX);
  v5_set_motor(V5_PORT_RIGHT, ctrl_right * V5_MAX / CTRL_MAX);
}

//------------------------------------------------------//
// ДАТЧИКИ КОЛЁС (прерывания по обоим фронтам)
//------------------------------------------------------//
void IRAM_ATTR on_speed_lf() {
  if (ctrl_left != 0) counter_lf += (ctrl_left > 0) ? 1 : -1;
  newLeftSpeed = true;
}
void IRAM_ATTR on_speed_rf() {
  if (ctrl_right != 0) counter_rf += (ctrl_right > 0) ? 1 : -1;
  newRightSpeed = true;
}

//------------------------------------------------------//
// BLUETOOTH (BLE UART): приём команд телефона
//------------------------------------------------------//
void on_bt_char(char inChar) {
  if (inChar != endChar) {
    switch (msgPart) {
      case HEADER: process_header(inChar); return;
      case BODY:   process_body(inChar);   return;
    }
  } else {
    msg_buf[msg_idx] = '\0';  // конец сообщения
    parse_msg();
  }
}

void process_header(char inChar) {
  header = inChar;
  msgPart = BODY;
  msg_idx = 0;
}

void process_body(char inChar) {
  msg_buf[msg_idx++] = inChar;
  if (msg_idx >= MAX_MSG_SZ - 1) {  // переполнение — сброс
    msgPart = HEADER;
    msg_idx = 0;
  }
}

void parse_msg() {
  msgPart = HEADER;
  switch (header) {
    case 'c': {  // команды движения: left,right (как process_ctrl_msg в openbot.ino)
      char *tmp = strtok(msg_buf, ",:");
      if (tmp == NULL) break;
      ctrl_left = constrain(atoi(tmp), -CTRL_MAX, CTRL_MAX);
      tmp = strtok(NULL, ",:");
      if (tmp == NULL) break;
      ctrl_right = constrain(atoi(tmp), -CTRL_MAX, CTRL_MAX);
#if DEBUG
      Serial.print("Control: ");
      Serial.print(ctrl_left);
      Serial.print(",");
      Serial.println(ctrl_right);
#endif
      break;
    }
    case 'f':  // конфигурация датчиков из приложения (принимаем как есть)
      break;
    default:
      break;
  }
}

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer *bleServer) {
    deviceConnected = true;
    Serial.println("BT Connected");
  }
  void onDisconnect(BLEServer *bleServer) {
    deviceConnected = false;
    Serial.println("BT Disconnected");
  }
};

class MyCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string value = pCharacteristic->getData();
    for (size_t i = 0; i < value.length(); i++) on_bt_char(value[i]);
  }
};

//------------------------------------------------------//
// ТЕЛЕМЕТРИЯ: отправка в приложение
//------------------------------------------------------//
void sendData(const String &data) {
  Serial.print(data);
  Serial.println();
  if (deviceConnected) {
    char outData[MAX_MSG_SZ] = "";
    for (int i = 0; i < data.length() && i < MAX_MSG_SZ - 1; i++) {
      outData[i] = data[i];
    }
    pTxCharacteristic->setValue(outData);
    pTxCharacteristic->notify();
  }
}

uint8_t read_signal(volatile bool &flag, uint8_t &state, int pin) {
  if (flag) {
    flag = false;
    state = digitalRead(pin) ? 1 : 0;
  }
  return state;
}

void send_data() {
  // Сообщение одометрии: "o,left,right,lsig,rsig"
  String odom = "o,";
  odom += String(counter_lf) + "," + String(counter_rf) + ",";
  odom += String(read_signal(newLeftSpeed, signalLeftSpeed, PIN_SPEED_LF)) + ",";
  odom += String(read_signal(newRightSpeed, signalRightSpeed, PIN_SPEED_RF));
  sendData(odom);

  // Батарея: "B,<десятые доли вольта>"
  int raw = analogRead(PIN_VIN);
  battery_voltage = raw * ADC_FACTOR * VOLTAGE_DIVIDER_FACTOR;
  sendData("B," + String((int)(battery_voltage * 10)));

  // Скорость: "S,<cmdL>,<cmdR>"
  sendData("S," + String(ctrl_left) + "," + String(ctrl_right));
}

//------------------------------------------------------//
// SETUP / LOOP
//------------------------------------------------------//
void setup() {
  Serial.begin(115200);

  // Моторы остановить как можно раньше
  Wire.begin(PIN_V5_I2C_SDA, PIN_V5_I2C_SCL);
  v5_set_motor(V5_PORT_LEFT, 0);
  v5_set_motor(V5_PORT_RIGHT, 0);

  pinMode(PIN_SPEED_LF, INPUT_PULLUP);
  pinMode(PIN_SPEED_RF, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_SPEED_LF), on_speed_lf, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_SPEED_RF), on_speed_rf, CHANGE);

  // BLE UART — тот же сервис и характеристики, что в openbot.ino
  BLEDevice::init(BT_DEVICE_NAME);
  bleServer = BLEDevice::createServer();
  bleServer->setCallbacks(new MyServerCallbacks());
  BLEService *pService = bleServer->createService(BLEUUID(SERVICE_UUID));

  pTxCharacteristic = pService->createCharacteristic(BLEUUID(CHARACTERISTIC_UUID_TX), BLECharacteristic::PROPERTY_NOTIFY);
  pTxCharacteristic->addDescriptor(new BLE2902());

  BLECharacteristic *pRxCharacteristic = pService->createCharacteristic(BLEUUID(CHARACTERISTIC_UUID_RX), BLECharacteristic::PROPERTY_WRITE_NR);
  pRxCharacteristic->setCallbacks(new MyCallbacks());
  pRxCharacteristic->addDescriptor(new BLE2902());

  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(BLEUUID(SERVICE_UUID));
  bleServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");

  Serial.print('r');  // рукопожатие: прошивка готова (как в openbot.ino)
}

void loop() {
  // переподключение телефона
  if (!deviceConnected && oldDeviceConnected) {
    delay(500);                     // дать стеку Bluetooth подготовиться
    bleServer->startAdvertising();  // заново запустить рекламу
    Serial.println("Waiting a client connection to notify...");
    oldDeviceConnected = deviceConnected;
  }
  if (deviceConnected && !oldDeviceConnected) {
    oldDeviceConnected = deviceConnected;
  }

  update_vehicle();

  unsigned long now = millis();
  if (now - last_send >= SEND_INTERVAL) {
    last_send = now;
    send_data();
  }
}
