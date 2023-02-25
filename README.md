
[![Website](https://img.shields.io/badge/Website-Link-blue.svg)](https://gavinlyonsrepo.github.io/)  [![Rss](https://img.shields.io/badge/Subscribe-RSS-yellow.svg)](https://gavinlyonsrepo.github.io//feed.xml)  [![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/paypalme/whitelight976)

![ ERM19264 ](https://github.com/gavinlyonsrepo/ERM19264_UC1609/blob/main/extras/image/color.jpg)

Table of contents
---------------------------

  * [Overview](#overview)
  * [Output](#output)
  * [Hardware](#hardware)
  * [Software](#software)
  * [Files](#files)
  
Overview
--------------------
* Name : ERM19264_UC1609
* Title : Library for ERM19264-5 v3 LCD  (UC1609C controller)
* Description : 

1. Raspberry pi PICO RP2040 library.       
2. Inverse, Scroll, rotate and contrast control. 
3. 8 ASCII fonts included.
4. Graphics class included.
5. Sleep mode.
6. Bitmaps supported.
7. Hardware SPI options
8. Port of my arduino [library](https://github.com/gavinlyonsrepo/ERM19264_UC1609) 

* Author: Gavin Lyons
* Developed on
	1. Raspberry pi PICO RP2040
	2. SDK C++ compiler G++ for arm-none-eabi
	3. CMAKE , VScode

Output
---------------------------------

Output Screenshots, From left to right top to bottom.

1. Full screen bitmap displayed
2. FPS test recorded at 192 FPS for 8Mhz SPI baud rate
3. Different size and inverted  default font 
4. ASCII font 1-127 printed out with default font size 1 
5. Fonts 7-8
6. Fonts 1-4

![op](https://github.com/gavinlyonsrepo/ERM19264_UC1609/blob/main/extras/image/output.jpg)

![op2](https://github.com/gavinlyonsrepo/ERM19264_UC1609/blob/main/extras/image/output2.jpg)

Hardware
----------------------------

9 pins , Vcc and GND, anode and cathode for the backlight LED and an SPI interface.
The backlight control is left up to user. The CLK and Data lines are fixed to the SPI interface but any GPIO can be used for reset, DC and CS. 

There are 3 different colours in range, Parts used purchased from [ebay](https://www.ebay.ie/itm/2-inch-White-192x64-Graphic-LCD-Display-Module-UC1609-SPI-for-Arduino/293617684779?hash=item445cfa512b:g:10MAAOSwYV9e6xsi)
 
1. ERM19264SBS-5 V3 LCD Display UC1609C controller ,  white on blue
2. ERM19264FS-5 V3 LCD Display  UC1609C controller , black on white
3. ERM19264DNS-5 V3 LCD Display  UC1609C controller white on black

The library was tested on 1 and 2. 
This wiring Diagram from the manufacturer showing hardware setup connected to an ~8051 MCU, showing both 3.3 volt and 5 volt systems. 

![ ERM19264 ](https://github.com/gavinlyonsrepo/ERM19264_UC1609/blob/main/extras/image/connect.jpg)

Software
-------------------------

*SPI*

Hardware SPI. The hardware SPI speed is set at 8MHz you can change this if necessary by changing LCDbegin method arguments.
Spi0 is used in example files but can be changed by passing a different SPI channel.(Spi0 or Spi1) 

*fonts*

There are eight fonts.
A print class is available to print out most passed data types.
The fonts 1-6 are a byte high(at text size 1) scale-able fonts.
Font 7-8 are specials large fonts but are numbers only and cannot be scaled(just one size).  
Font 7-8 will print just numbers + semi-colons ,  if you print a float using print method
it will place a space and use a circle for a decimal point, you can also print negative numbers with this method.  

Font data table: 

| Font num | Font enum name | Font size xbyy |  ASCII range | Size in bytes |
| ------ | ------ | ------ | ------ |  ------ | 
| 1 | UC1609Font_Default | 5x8 | ASCII 0 - 0xFF, Full Extended  | 1275 |
| 2 | UC1609Font_Thick   | 7x8 |  ASCII  0x20 - 0x5A, no lowercase letters | 406 | 
| 3 | UC1609Font_SevenSeg  | 4x8 | ASCII  0x20 - 0x7A | 360 |
| 4 | UC1609Font_Wide | 8x8 |  ASCII 0x20 - 0x5A, no lowercase letters| 464 |
| 5 | UC1609Font_Tiny | 3x8 | ASCII  0x20 - 0x7E | 285 |
| 6 | UC1609Font_Homespun  | 7x8 | ASCII  0x20 - 0x7E |  658 |
| 7 | UC1609Font_Bignum | 16x32 | ASCII 0x30-0x3A ,Numbers + : . - only | 704 |
| 8 | UC1609Font_Mednum | 16x16 | ASCII 0x30-0x3A , Numbers + : . - only | 352 |


By default All fonts are included to save memory by not including fonts that you do not want. 

1. Comment out the respective define at top of library header file ERM19264_UC1609_font.hpp in the USER FONT OPTION ONE section


*font mods*

The default ASCII font (font one) is an [extended ASCII font](https://www.extended-ascii.com/) 0-255 characters.
If you do not need characters 127-255 and wish to save memory space:
In library header file ERM19264_UC1609_font.hpp  in the USER FONT OPTION TWO section
Simply comment this define out. 

1. UC_FONT_MOD_TWO (save 640 bytes) extended ASCII 127-255

You can also remove the first 30 characters if not needed but user will need to change 
ERM19264_ASCII_OFFSET  from 0x00 to 0x20. This will save a further 150 bytes.

*bitmaps*

There is a few different ways of displaying bitmaps, 

| Num | Method | Data addressing | Note |
| ------ | ------ |  ------ |  ------ |  
| 1 | LcdBitmap() | Vertical |  Writes directly to screen , no buffer used. | 
| 2 | LcdBuffer() |  Vertical  |  For internal use mostly | 
| 3 | drawBitmap() |  Vertical | default,  setDrawBitmapAddr(true) | 
| 4 | drawBitmap() |   Horizontal | setDrawBitmapAddr(false) |

See the bitmap example file for more details on each method. Bitmaps can be turned to data [here at link]( https://javl.github.io/image2cpp/).

*User adjustments*

When the user calls LCDbegin() to start LCD they can specify a contrast setting from 0x00 to 0xFF.
Datasheet says 0x49 is default. (VbiasPOT). Lower contrast works better on the blue version.

It is also possible for user to change LCD bias ,  Temperature coefficient, frame rate and power control but this must be done by changing defines in header file. Choose lower frame rate for lower power, and choose higher frame rate to improve LCD contrast and minimize flicker. See Data sheet for range of values
here. Defaults where found to be fine during all testing of this library.

| Parameter | default Values |  Define | Register |
| ------ | ------ |  ------ | ------ |
| LCD bias |  9 | BIAS_RATIO_SET | BR 1:0 |
| Temp coefficient | -0.00%/ C |  TEMP_COMP_SET | TC 1:0  |
| Frame rate | 95 fps |  FRAMERATE_SET |  LC 4:3 |
| Power control | 1.4mA + Internal VLCD (7x charge pump) |  PC_SET | PC 2:0 |
| V bias Bot(contrast) | 0x49h default|  Set by user with LCDbegin | PM 7:0 |


Files
-------------------

X = ERM19264_UC1609 in tables below

| Src Files | Description  |
| ------ | ------ |
| X.hpp | library header file  |
| X.cpp |  library  source file  |
| X_graphics.hpp | graphics header file |
| X_graphics.cpp |  graphics source file |
| X_font.hpp |  font header file |
| X_font.cpp |  font data file |
| X_print.hpp | used for printing data types |
| X_print.cpp |  used for printing data types |

The example files are in example folder.
To build the one you want, edit the Cmaketxt file 
add_executable(${PROJECT_NAME} section, comment in one example file path and one only.
The default is the Hello world one.

| Examples files main.cpp  | Description |
| ------ | ------ | 
| X_HELLO | Hello world , Basic usage | 
| X_MISC | Fonts + graphics + FPS test & misc functions, rotate , scroll,  etc | 
| X_BITMAP | Shows use of bitmaps methods  | 