# OpenBot DIY (сделай сам)

<p align="center">
  <a href="README.md">English</a> |
  <span>Русский</span> |
  <a href="README.zh-CN.md">简体中文</a> |
  <a href="README.de-DE.md">Deutsch</a> |
  <a href="README.fr-FR.md">Français</a> |
  <a href="README.es-ES.md">Español</a> |
  <a href="README.ko-KR.md">한국어</a>
</p>

Мы разработали корпус колёсного робота, основанный на дешёвых и легкодоступных hobby-компонентах. Ниже вы найдёте инструкции по сборке собственного робота. Если у вас возникнут дополнительные вопросы или сомнения, пожалуйста, свяжитесь с нами. Удачи в сборке робота!

## Шасси

### 3D-печать

Для сборки вашего OpenBot вам нужно напечатать и собрать следующие детали.

#### Корпус робота

Есть несколько вариантов корпуса робота в зависимости от ваших потребностей и возможностей вашего 3D-принтера. Мы поощряем вас проектировать и строить свой собственный корпус, но вот несколько вариантов в качестве отправной точки:

- [Стандартный корпус](cad/regular_body/README.md): это стандартный корпус, который мы разработали; требуется печатная платформа размером не менее 240×150 мм.
- [Узкий корпус](cad/slim_body/README.md): поскольку у многих распространённых 3D-принтеров рабочий объём меньше, мы также разработали уменьшенную версию без бамперов, которая помещается на печатной платформе 220×220 мм под углом 45 градусов.
- [Клееный корпус](cad/glue_body/README.md): для печати на 3D-принтерах с ещё меньшим рабочим объёмом есть также модульный корпус, разработанный @sloretz, состоящий из нескольких деталей, которые нужно склеить; он помещается на печатной платформе 150×140 мм.
- [Блочный корпус](cad/block_body/README.md): этот корпус, разработанный @Christos-Ps, предоставляет несколько вариантов с опциями дополнительного места внутри оболочки и крышкой, совместимой с Lego, при сохранении небольших габаритов — для печати требуется всего 221×150 мм.

#### Крепление телефона

Кроме того, вам нужно напечатать крепление для телефона, которое закрепляется на корпусе робота.

- phone_mount_bottom ([STL](../phone_mount/phone_mount_bottom.stl), [STEP](../phone_mount/phone_mount_bottom.step))
- phone_mount_top ([STL](../phone_mount/phone_mount_top.stl), [STEP](../phone_mount/phone_mount_top.step))

#### Зачистка

Прежде чем переходить к сборке, возможно, потребуется очистить 3D-печатные детали.
<p float="left">
  <img src="../../docs/images/clean_3d_print_1.jpg" width="32%" />
  <img src="../../docs/images/clean_3d_print_2.jpg" width="32%" /> 
  <img src="../../docs/images/clean_3d_print_3.jpg" width="32%" />
</p>

### Альтернативы

Если у вас нет доступа к 3D-принтеру, существует несколько готовых наборов робототехники на базе Arduino, которые можно использовать как основу. Эти наборы поставляются с шасси, моторами и аксессуарами. Мы рекомендуем взять базовый набор, поскольку электроника и датчики более дорогих наборов вам в значительной мере не понадобятся. Вот несколько вариантов:

- Perseids DIY Robot Smart Car Chassis Kit ([EU](https://www.amazon.de/dp/B07DNXBNHY), [US](https://www.amazon.com/dp/B07DNXBFQN))
- SZDoit 4WD Smart Metal Robot Car Chassis Kit ([US](https://www.amazon.com/dp/B083K4RKBP), [AE](https://www.aliexpress.com/item/33048227237.html))
- Joy-it Robot Car Kit 01 ([EU](https://www.amazon.de/dp/B073ZGJF28))
- Smart Car Kit 4WD Smart Robot Car Chassis Kit ([AE](https://www.aliexpress.com/item/4001238626191.html))

Вам также понадобится крепление для телефона. Вот несколько вариантов:

- Phone Mount ([EU](https://www.amazon.de/dp/B06XDYJNSR), [US](https://www.amazon.com/dp/B09CY8MC2R))

Вы также можете проявить креативность и сделать своё собственное шасси OpenBot и крепление для телефона из материала по вашему выбору (например, дерево, картон, пенопласт и т.д.). Если вы это сделаете, пожалуйста, опубликуйте несколько фотографий в [канале Slack](https://github.com/ob-f/OpenBot#contact), чтобы другие могли восхититься вашей изобретательностью. Вот пример от [@custom-build-robots](https://custom-build-robots.com/roboter/openbot-dein-smartphone-steuert-ein-roboter-auto-chassis-bauen/13636):

<p float="left">
  <img src="../../docs/images/chassis_cardboard_1.jpg" width="32%" />
  <img src="../../docs/images/chassis_cardboard_2.jpg" width="32%" />
  <img src="../../docs/images/chassis_cardboard_3.jpg" width="32%" />
</p>

## Сборка

Существует два различных варианта сборки робота: DIY и PCB. Подход DIY основан на популярном драйвере двигателей L298N и рекомендуется энтузиастам с некоторым опытом в электронике. Он требует изрядного количества проводки, особенно если установлены все датчики и светодиоды. Однако все компоненты легко доступны в большинстве стран, и особенно для одиночной сборки или просто чтобы попробовать проект, рекомендуется вариант DIY. Чтобы уменьшить количество проводки и упростить сборку, мы также разработали [собственную печатную плату](pcb). Она рекомендуется, если вы хотите получить более аккуратную сборку или построить несколько OpenBot.

### Спецификация материалов

Наш корпус робота основан на легкодоступной hobby-электронике. Мы приводим ссылки для Германии (EU) и США (US) с быстрой доставкой. Если у вас есть терпение подождать подольше, компоненты можно значительно дешевле купить на AliExpress (AE). Вам понадобятся следующие компоненты.

#### Необходимые компоненты

- 1x Arduino Nano ([EU](https://www.amazon.de/dp/B01MS7DUEM), [US](https://www.amazon.com/dp/B00NLAMS9C), [AE](https://www.aliexpress.com/item/32866959979.html))
- 4x мотора TT с шинами ([EU](https://www.conrad.de/de/p/joy-it-com-motor01-getriebemotor-gelb-schwarz-passend-fuer-einplatinen-computer-arduino-banana-pi-cubieboard-raspbe-1573543.html), [US](https://www.amazon.com/dp/B081YQM55P), [AE](https://www.aliexpress.com/item/4000126948489.html))
- 3x аккумулятор 18650 ([EU](https://www.conrad.de/de/p/conrad-energy-18650-usb-spezial-akku-18650-li-ion-3-7-v-1400-mah-1525536.html), [US](https://www.amazon.com/dp/B083K4XSKG), [AE](https://www.aliexpress.com/item/32352434845.html))
- 1x держатель аккумуляторов 18650 ([EU](https://www.amazon.de/dp/B075V25QJ9), [US](https://www.amazon.com/dp/B07DWQYD7H), [AE](https://www.aliexpress.com/item/33037738446.html))
- 1x кабель USB OTG ([EU](https://www.amazon.de/gp/product/B075M4CQHZ) ,[US](https://www.amazon.com/dp/B07LBHKTMM), [AE](https://www.aliexpress.com/item/10000330515850.html))
- 1x пружина или резинка ([EU](https://www.amazon.de/gp/product/B01N30EAZO/), [US](https://www.amazon.com/dp/B008RFVWU2), [AE](https://www.aliexpress.com/item/33043769059.html))
- 16x винт M3x25 ([EU](https://www.amazon.de/dp/B07KFL3SSV), [US](https://www.amazon.com/dp/B07WJL3P3X), [AE](https://www.aliexpress.com/item/4000173341865.html))
- 16x гайка M3 ([EU](https://www.amazon.de/dp/B07JMF3KMD), [US](https://www.amazon.com/dp/B071NLDW56), [AE](https://www.aliexpress.com/item/32977174437.html))
- 6x винт M3x5 ([EU](https://www.amazon.de/dp/B01HBRG3W8), [US](https://www.amazon.com/dp/B07MBHMLL2), [AE](https://www.aliexpress.com/item/32892594230.html))
- Провода Dupont ([EU](https://www.amazon.de/dp/B07KYHBVR7), [US](https://www.amazon.com/dp/B07GD2BWPY), [AE](https://www.aliexpress.com/item/4000766001685.html))

#### Опциональные компоненты

- 2 x датчик скорости ([EU](https://www.conrad.de/de/p/joy-it-sen-speed-erweiterungsmodul-passend-fuer-einplatinen-computer-arduino-banana-pi-cubieboard-raspberry-pi-pc-1646891.html), [US](https://www.amazon.com/dp/B081W2TY6Q), [AE](https://www.aliexpress.com/i/32850602744.html))
- 1x ультразвуковой датчик ([EU](https://www.amazon.de/dp/B00LSJWRXU), [US](https://www.amazon.com/dp/B0852V181G/), [AE](https://www.aliexpress.com/item/32713522570.html))
- 1x выключатель вкл/выкл ([EU](https://www.amazon.de/dp/B07QB22J62), [US](https://www.amazon.com/dp/B01N2U8PK0), [AE](https://www.aliexpress.com/item/1000005699023.html))
- 2x оранжевый светодиод 5 мм ([EU](https://www.amazon.de/gp/product/B01NCL0UTQ), [US](https://www.amazon.com/dp/B077XD7MVB), [AE](https://www.aliexpress.com/item/4000329069943.html))
- 1x OLED-дисплей ([EU](https://www.amazon.de/dp/B079H2C7WH), [US](https://www.amazon.com/dp/B085NHM5TC), [AE](https://www.aliexpress.com/item/4001268387467.html))

#### Компоненты DIY (вариант 1)

- 1x драйвер двигателей L298N ([EU](https://www.conrad.de/de/p/joy-it-motormodul-2-u-4-phasen-6-bis-12v-1573541.html), [US](https://www.amazon.com/dp/B085XSLKFQ), [AE](https://www.aliexpress.com/item/32994608743.html))
- (Опционально) Резисторы (2x 150<span>&#8486;</span> для светодиодов и 20 к<span>&#8486;</span> и 10 к<span>&#8486;</span> для делителя напряжения)
- (Комплект) 4x мотора TT и шины + 2x L298N + провода dupont ([US](https://www.amazon.com/dp/B07ZT619TD))
- (Комплект) 4x мотора TT и шины + провода + винты ([US](https://www.amazon.com/dp/B07DRGTCTP))

#### Компоненты PCB (вариант 2)

- 1x [собственная печатная плата](pcb)
- 5x кабель Micro JST PH 2.0 ([EU](https://www.amazon.de/gp/product/B07449V33P), [US](https://www.amazon.com/dp/B09JZC28DP), [AE](https://www.aliexpress.com/item/32963304134.html))

### Инструкции по сборке

**Совет:** нажмите на изображения, чтобы открыть их в полном разрешении в новой вкладке.

#### Вариант 1: DIY

<p float="left">
  <img src="../../docs/images/diy_parts.jpg" height="300" />
  <img src="../../docs/images/wiring_diagram.png" height="300" /> 
</p>

**Совет:** чтобы упростить всю проводку, вы можете сделать небольшой распределитель питания для подключений 5V и GND, припаяв гребёнку 6x2 с штырьками (male header) к монтажной плате. Затем соедините распределитель питания с 5V / GND драйвера двигателей.

1. Припаяйте провода к моторам и установите кодирующие диски на два передних мотора, если собираетесь использовать датчики скорости.
    <p float="left">
      <img src="../../docs/images/add_wires_motor.jpg" width="32%" />
      <img src="../../docs/images/add_disk_motor.jpg" width="32%" /> 
    </p>
2. Вставьте положительный и отрицательный провода двух левых моторов в OUT1 (+) и OUT2 (-) платы L298N. Вставьте положительный и отрицательный провода двух правых моторов в OUT4 (+) и OUT3 (-) платы L298N.
3. Закрепите моторы восемью винтами M3x25 с гайками.
    <p float="left">
      <img src="../../docs/images/attach_motors_1.jpg" width="32%" />
      <img src="../../docs/images/attach_motors_2.jpg" width="32%" /> 
      <img src="../../docs/images/attach_motors_3.jpg" width="32%" />
    </p>
4. Закрепите L298N четырьмя винтами M3x5
5. (Опционально) Установите ультразвуковой датчик и замените угловой разъём на прямой (или аккуратно согните штырьки).
    <p float="left">
      <img src="../../docs/images/sonar_front.jpg" width="32%" />
      <img src="../../docs/images/sonar_back.jpg" width="32%" /> 
      <img src="../../docs/images/sonar_bend_pins.jpg" width="32%" />
    </p>
6. (Опционально) Установите оранжевые светодиоды для сигналов указателей поворота.
    <p float="left">
      <img src="../../docs/images/led_insert.jpg" width="32%" />
      <img src="../../docs/images/led_left.jpg" width="32%" /> 
      <img src="../../docs/images/led_right.jpg" width="32%" />
    </p>
7. Закрепите нижнюю часть крепления телефона на верхней пластине с помощью двух винтов M3x25 и гаек.
    <p float="left">
      <img src="../../docs/images/install_camera_mount_1.jpg" width="32%" />
      <img src="../../docs/images/install_camera_mount_2.jpg" width="32%" /> 
      <img src="../../docs/images/install_camera_mount_3.jpg" width="32%" />
    </p>
8. Вставьте верхнюю часть крепления телефона и установите пружину или резинку.
    <p float="left">
      <img src="../../docs/images/install_spring_1.jpg" width="32%" />
      <img src="../../docs/images/install_spring_2.jpg" width="32%" /> 
    </p>
9. Замените угловой разъём на прямой (или аккуратно согните штырьки), затем закрепите датчики скорости одним винтом M3x5 каждый.
    <p float="left">
      <img src="../../docs/images/install_speed_sensor_1.jpg" width="32%" />
      <img src="../../docs/images/install_speed_sensor_2.jpg" width="32%" /> 
      <img src="../../docs/images/install_speed_sensor_3.jpg" width="32%" />
    </p>
10. Установите батарейный отсек (например, на велкро-липучку).
    <p float="left">
      <img src="../../docs/images/install_battery_1.jpg" width="32%" />
      <img src="../../docs/images/install_battery_2.jpg" width="32%" /> 
      <img src="../../docs/images/install_battery_3.jpg" width="32%" />
    </p>
11. (Опционально) Установите выключатель вкл/выкл в разрыв цепи.
    1. Вставьте выключатель в соответствующее отверстие до щелчка.
    2. Припаяйте красные провода (12V) батарейного отсека и кабеля питания каждый к одному из выводов выключателя. Соедините чёрные провода (GND) и закройте соединение термоусадкой.
    3. Закрепите провода несколькими кусочками изоленты.
    <p float="left">
      <img src="../../docs/images/install_switch_1.jpg" width="32%" />
      <img src="../../docs/images/install_switch_2.jpg" width="32%" /> 
      <img src="../../docs/images/install_switch_3.jpg" width="32%" />
    </p>
12. (Опционально) Установите OLED-дисплей.
13. Подключите ШИМ-входы L298N к выводам D5, D6, D9 и D10 Arduino.
14. Подключите датчики скорости и ультразвуковой датчик к 5V и GND.
15. Подключите вывод D0 датчиков скорости к выводам D2 (левый) и D3 (правый) Arduino.
16. Подключите выводы Echo и Trigger ультразвукового датчика к выводам D11 и D12 Arduino соответственно.
17. (Опционально) Подключите светодиоды к выводам D4 (левый) и D7 (правый) Arduino и GND. Мы рекомендуем добавить последовательно резистор 150 Ом для ограничения потребления тока.
18. (Опционально) Подключите делитель напряжения к выводу A7 Arduino. Он используется для измерения напряжения батареи.
19. (Опционально) Подключите OLED-дисплей (чип SSD1306) через шину I2C к Arduino Nano
    1. Подключите выводы VIN и GND дисплея к 5V и GND.
    2. Подключите вывод SCL дисплея к выводу A5.
    3. Подключите вывод SDA дисплея к выводу A4.
20. Подключите кабели питания к +12V и GND платы L298N.
21. Подключите USB-кабель к Arduino и пропустите его через верхнюю крышку.
22. Вставьте шесть гаек M3 в нижнюю пластину и закрепите верхнюю крышку шестью винтами M3x25.
23. Установите колёса.

#### Вариант 2: собственная PCB

1. Припаяйте провода с разъёмами Micro JST PH 2.0 к моторам и установите кодирующие диски на два передних мотора, если собираетесь использовать датчики скорости.
    <p float="left">
      <img src="../../docs/images/add_wires_motor.jpg" width="32%" />
      <img src="../../docs/images/add_disk_motor.jpg" width="32%" /> 
    </p>
2. Закрепите моторы восемью винтами M3x25 с гайками.
    <p float="left">
      <img src="../../docs/images/attach_motors_1.jpg" width="32%" />
      <img src="../../docs/images/attach_motors_2.jpg" width="32%" /> 
      <img src="../../docs/images/attach_motors_3.jpg" width="32%" />
    </p>
3. Подключите два левых мотора к M3 и M4, а два правых мотора — к M1 и M2.
    <p float="left">
      <img src="../../docs/images/connect_motors_pcb.jpg" width="32%" />
    </p>
4. Закрепите печатную плату четырьмя винтами M3x5, а моторы — восемью винтами M3x25 с гайками.
    <p float="left">
      <img src="../../docs/images/attach_pcb.jpg" width="32%" />
      <img src="../../docs/images/chassis_motors_pcb.jpg" width="32%" />
    </p>
5. Выполните шаги 5–12 из варианта DIY.
6. Подключите ультразвуковой датчик (VCC/+, Trig, Echo, GND/-) к 4-контактному разъёму с маркировкой *SONAR* на печатной плате.
    <p float="left">
      <img src="../../docs/images/connect_sonar_sensor.jpg" width="32%" />
    </p>
7. Подключите левый и правый сигналы указателей поворота (оранжевые светодиоды) к 2-контактным разъёмам с маркировкой *SIGNAL_L* и *SIGNAL_R* на печатной плате. Более длинная ножка — +, короткая — -.
8. Подключите левый и правый датчики скорости (VCC/+, GND/-, D0) к 3-контактным разъёмам с маркировкой *SPEED_L* и *SPEED_R*.
9. (Опционально) Подключите OLED-дисплей (чип SSD1306) к разъёму IO2 на печатной плате.
    1. Подключите выводы VIN и GND дисплея к 5V и GND.
    2. Подключите вывод SCL дисплея к выводу A5.
    3. Подключите вывод SDA дисплея к выводу A4.
10. Подключите кабели питания к Vin (разъём Micro JST PH 2.0) печатной платы.
11. Выполните шаги 21–23 из варианта DIY.

## Далее

Прошейте [прошивку Arduino](../../firmware/README.ru_RU.md)
