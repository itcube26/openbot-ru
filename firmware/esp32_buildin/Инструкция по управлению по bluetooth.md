# Загружаем скетч в Esp32-dev

```cpp
// This example code is in the Public Domain (or CC0 licensed, at your option.)
// By Evandro Copercini - 2018
//
// This example creates a bridge between Serial and Classical Bluetooth (SPP)
// and also demonstrate that SerialBT have the same functionalities of a normal Serial
// Note: Pairing is authenticated automatically by this device

#include <Arduino.h>
#include "BluetoothSerial.h"

String device_name = "openbot-ru";

// Check if Bluetooth is available
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

// Check Serial Port Profile
#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Port Profile for Bluetooth is not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  SerialBT.begin(device_name);  //Bluetooth device name
  //SerialBT.deleteAllBondedDevices(); // Uncomment this to delete paired devices; Must be called after begin
  Serial.printf("The device with name \"%s\" is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str());
}

void loop() {
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }
  delay(20);
}

```


# Загружаем скетч в arduino. В мониторе порта видим то, что посылается из телефона на Serial2 порт и транслируется на Serial порт компьютера

```cpp
// получаем символы от блютуз и выводим в Serial

void setup() {
	Serial.begin(9600);
	Serial2.begin(115200);
}

void loop() {
	if (Serial2.available()) {
		Serial.write(Serial2.read());
	}
}
```

# Пример управления по bluetooth:

```cpp
// 

#include <Vex5.h>

#define BAUDRATE         (500000)
#define SPI_NSS_PIN      (-1)
#define IS_RESET_SHIELD  (1)

Vex5_Motor motor7, motor8, motor11, motor12;

int16_t goalSpeed = 2000;
int16_t stopSpeed = 0;
int16_t realSpeed = 0;
int32_t realPosition = 0;
int16_t realPower = 0;
unsigned long time = millis();

// езда вперед
void forward(){
  motor7.setSpeed(-goalSpeed);
  motor8.setSpeed(-goalSpeed);
  motor11.setSpeed(goalSpeed);
  motor12.setSpeed(goalSpeed);
}

// езда влево
void left(){
  motor7.setSpeed(-goalSpeed);
  motor8.setSpeed(-goalSpeed);
  motor11.setSpeed(goalSpeed);
  motor12.setSpeed(goalSpeed);
}

// езда вправо
void right(){
  motor7.setSpeed(-goalSpeed);
  motor8.setSpeed(-goalSpeed);
  motor11.setSpeed(goalSpeed);
  motor12.setSpeed(goalSpeed);
}

// езда назад
void nazad(){
  motor7.setSpeed(goalSpeed);
  motor8.setSpeed(goalSpeed);
  motor11.setSpeed(-goalSpeed);
  motor12.setSpeed(-goalSpeed);
}

// стоп
void stop(){
  motor7.setSpeed(stopSpeed);
  motor8.setSpeed(stopSpeed);
  motor11.setSpeed(stopSpeed);
  motor12.setSpeed(stopSpeed);
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
  Vex5.begin(BAUDRATE, SPI_NSS_PIN, IS_RESET_SHIELD);
  
  motor7.begin(VEX5_PORT_7);
  motor8.begin(VEX5_PORT_8);
  motor11.begin(VEX5_PORT_11);
  motor12.begin(VEX5_PORT_12);
  
  time = millis();
}

void loop() {
  

  while (Serial2.available()){
    
    char command = Serial2.read();
    Serial.write(command);

    switch(command)  {
     case 'F':
      forward();
      break;

    case  'B':
      nazad();
      break;

    case   'R':
      right();
      break;

    case  'L':
      left();
      break;

    case  'S':
      stop();
      break;
    }
    motor12.getSpeed(realSpeed);
  motor12.getPosition(realPosition);
  motor12.getPower(realPower);
  
  Serial.print("Goal: ");
  Serial.print(goalSpeed);
  Serial.print(" | Real: ");
  Serial.print(realPosition);
  Serial.print(" ");
  Serial.print(realSpeed);
  Serial.print(" ");
  Serial.println(realPower);
 }
  
  
}
```