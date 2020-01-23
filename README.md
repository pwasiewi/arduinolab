# ArduinoLab - my arduino laboratory exercises. 
Use the smallest timer library from Polish Arduino forum: http://bit.ly/arduinotimer - in file [TimersForumArduino.zip](https://github.com/pwasiewi/arduinolab/blob/master/TimersForumArduino.zip) 

## Dice rolling using the Arduino device was always in my dreams.
<img src="https://github.com/pwasiewi/arduinolab/blob/master/images/M5Stick-C.jpg" width="250" align="right" />
Thus, I get the M5Stich-C that is an amazing device based on ESP32 with a small LCD display of 80 × 160 pixel, but bright and clear. 

### How to load the sketch

<img src="https://github.com/pwasiewi/arduinolab/blob/master/images/M5Stick_Arduino_IDE.PNG" width="300" align="right" />

Previously, it is mandatory install the development environment. If you use the Arduino IDE, exists a perfect guide to start up the device : [M5StickC Quick Start - Arduino Win](https://docs.m5stack.com/#/en/quick_start/m5stickc/m5stickc_quick_start_with_arduino_Windows?id=_2-copy-the-following-esp32-boards-manager-url-to-additional-boards-manager-urls-hit-ok) 

Finally, this is the configuration that you need to compile the Arduino sketch of M5StickCDicesForMemoir44 for 1-8 Memoir'44 dices or M5StickCDices for usual 1-8 dices.

Now only that you need is plug in the M5Stick whith the USB-C adaptor and load the program. 

### Update dice images

<img src="https://github.com/pwasiewi/arduinolab/blob/master/images/lcd_convert_s.jpg" align="right" />

Execute [the lcd-image-converter from this repository](https://github.com/riuson/lcd-image-converter).
1. Png or Jpg is read from File->Open Menu. The image will be displayed when loaded. 
2. Select: Option->Conversions Menu. The top Preset: selects "Color R5G6B5". 
3. Select: Image tap and select “8 bit” for Block size :. Press "OK". 
4. The example "Tank.c" is output by "Convert ..." in File-Convert Menu. 
Know in advance the destination path. 
Tick: Import All Images You can also output all the images with “Convert ... all”. 
5. Edit .c converted file. You will get an error as it is, so fix it with your editor. 
- Change: "Static const uint8_t image_data_Tank [4690] = {" at the top of the file deleting "image_data_" as result of "static const uint8_t Tank [4690] = {". 
- At the bottom of delete de entire line: "const tImage Tank = {image_data_Tank, 35, 67, 8};
- Save the example "Tank.c"

### Hardware components
The [M5Stick-C](https://github.com/m5stack/M5StickC) is a device with multiple components as processor, sensors, interrupters and LED. The processor features are:

|   |   |
|---|---|
|Microcontroler|ESP32 Pico|
|Frequency|240MHz|
|Flash|4MB|
|RAM|320KB|

- **I2C BM8563** : Realizes the RTC function. Address: 0x15. 
- **AXP192** :  power management IC. 
- **ST7735S** : LCD IPS Display 80 × 160 pixel, 0,96"
- **SH200Q** : IMU Movement and positioning 6 axis inertial sensor.
- **SPM1423** : PDM Micropohne
- **IR** : IR Transmitter

### Software components
- [Arduino IDE](https://www.hackster.io/arduino/products/arduino-ide?ref=project-8e87cc)

Some projects are not mine among them:
 - https://github.com/McOrts/M5StickC_Nixie_tube_Clock
 - https://github.com/BLavery/virtual-GPIO
 - https://github.com/Sash0k/bluetooth-spp-terminal
 - https://github.com/maik/pragpub
 - https://github.com/DavidAndrews/Arduino_LCD_Menu
 - https://github.com/extrasleepy/Device-Invention
 - https://github.com/dserv01/SimpleUI16x2
 - https://exploreembedded.com/wiki/
 - http://arduino.cc
 - https://github.com/zerokol
 - https://github.com/arkhipenko/TaskScheduler
 - https://github.com/gioblu/Agenda
 - https://github.com/jonblack/arduino-menusystem
 - https://github.com/Sash0k/bluetooth-spp-terminal

