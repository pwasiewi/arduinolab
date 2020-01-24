# ArduinoLab - my arduino laboratory exercises. 
Use the smallest timer library from Polish Arduino forum: http://bit.ly/arduinotimer - in file [TimersForumArduino.zip](https://github.com/pwasiewi/arduinolab/blob/master/TimersForumArduino.zip) 

## Dice rolling using the Arduino device was always in my dreams
<img src="https://github.com/pwasiewi/arduinolab/blob/master/images/M5Stick-C.jpg" width="250" align="right" />
Thus, I get the M5StickC that is an amazing device (as you can see in exemplary projects from https://www.hackster.io/m5stack) based on a ESP32 development board with 0.96 inch TFT color screen (80 * 160 resolution - very bright and clear), Red LED, two user buttons and a power switch, Microphone, IR transmitter, 6-axis IMU (SH200Q) and 80 mAH battery. The ESP32 module ESP32-Pico in M5StickC also has a built-in 4MB flash. I with my son made not only usual dices (from 1 to 8 ones), but also a special version for boardgame Memoir'44 - https://boardgamegeek.com/boardgame/10630/memoir-44.

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

**Red LED & IR transmitter & BUTTON A & BUTTON B**

<table>
 <tr><td>ESP32 chip</td><td>GPIO10</td><td>GPIO9</td><td>GPIO37</td><td>GPIO39</td></tr>
 <tr><td>Red LED</td><td>LED pin</td><td> </td><td> </td><td> </td></tr>
 <tr><td>IR transmitter</td><td> </td><td>transmitter pin</td><td> </td><td> </td></tr>
<tr><td>BUTTON A</td><td> </td><td> </td><td>button pin</td><td> </td></tr>
<tr><td>BUTTON B</td><td> </td><td> </td><td> </td><td>button pin</td></tr>
</table>

**TFT Screen**

*Driver IC: [ST7735S](https://github.com/m5stack/M5-Schematic/blob/master/Core/ST7735S_v1.1.pdf)*

*Resolution: 80 * 160*

<table>
 <tr><td>ESP32 chip</td><td>GPIO15</td><td>GPIO13</td><td>GPIO23</td><td>GPIO18</td><td>GPIO5</td></tr>
 <tr><td>TFT Screen</td><td>TFT_MOSI</td><td>TFT_CLK</td><td>TFT_DC</td><td>TFT_RST</td><td>TFT_CS</td></tr>
</table>

**GROVE interface**

<table>
 <tr><td>ESP32 chip</td><td>GPIO33</td><td>GPIO32</td><td>5V</td><td>GND</td></tr>
 <tr><td>GROVE interface</td><td>SCL</td><td>SDA</td><td>5V</td><td>GND</td></tr>
</table>

**Microphone ([SPM1423](https://github.com/m5stack/M5-Schematic/blob/master/Core/SPM1423HM4H-B.pdf))**

<table>
 <tr><td>ESP32 chip</td><td>GPIO0</td><td>GPIO34</td></tr>
 <tr><td>Microphone</td><td>SCL</td><td>SDA</td></tr>
</table>

**6-axis IMU ([SH200Q](https://github.com/m5stack/M5-Schematic/blob/master/Core/SH200Q.pdf)) & Power Mangement IC ([AXP192](https://github.com/m5stack/M5-Schematic/blob/master/Core/AXP192%20Datasheet%20v1.13_cn.pdf))**

<table>
 <tr><td>ESP32 chip</td><td>GPIO22</td><td>GPIO21</td>
 <tr><td>6-axis IMU (SH200Q)</td><td>SCL</td><td>SDA</td>
 <tr><td>Power Mangement IC (AXP192)</td><td>SCL</td><td>SDA</td>
</table>

**M5StickC Memoir'44 Dices

<img src="https://github.com/pwasiewi/arduinolab/blob/master/images/dicememoir44.jpg" alt="M5StickCMemoir44">

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

