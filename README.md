
[![Website](https://img.shields.io/badge/Website-Link-blue.svg)](https://gavinlyonsrepo.github.io/)  [![Rss](https://img.shields.io/badge/Subscribe-RSS-yellow.svg)](https://gavinlyonsrepo.github.io//feed.xml)  [![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/paypalme/whitelight976)

![ ERM19264 ](https://github.com/gavinlyonsrepo/ERM19264_UC1609/blob/main/extras/image/color.jpg)

# ERM19264_UC1609_PICO

## Table of contents

  * [Overview](#overview)
  * [Hardware](#hardware)
  * [Software](#software)
	* [SPI](#spi)
	* [Fonts](#fonts)
	* [Bitmaps](#bitmaps)
	* [User adjustments](#user-adjustments)
	* [File system](#file-system)
  * [Notes and Issues](#notes-and-issues)

## Overview

* Name : ERM19264_UC1609
* Title : Library for ERM19264-5 v3 LCD  (UC1609C controller)
* Description : 

1. Raspberry pi PICO RP2040 library.       
2. Inverse, Scroll, rotate and contrast control. 
3. 10 ASCII fonts included.
4. Graphics class included.
5. Sleep mode.
6. Bitmaps supported.
7. Hardware SPI options
8. Polymorphic print class included to print many data types.
9. Port of my arduino [library](https://github.com/gavinlyonsrepo/ERM19264_UC1609) , More details there. 

* Author: Gavin Lyons
* Developed on
	1. Raspberry pi PICO RP2040
	2. SDK C++ compiler G++ for arm-none-eabi
	3. CMAKE , VScode


## Hardware

9 pins , Vcc and GND, anode and cathode for the backlight LED and an SPI interface.
The backlight control is left up to user. The CLK and Data lines are fixed to the SPI interface but any GPIO can be used for reset, DC and CS. 

There are 3 different colours in range, Parts used purchased from [ebay](https://www.ebay.ie/itm/2-inch-White-192x64-Graphic-LCD-Display-Module-UC1609-SPI-for-Arduino/293617684779?hash=item445cfa512b:g:10MAAOSwYV9e6xsi)
 
1. ERM19264SBS-5 V3 LCD Display UC1609C controller ,  white on blue
2. ERM19264FS-5 V3 LCD Display  UC1609C controller , black on white
3. ERM19264DNS-5 V3 LCD Display  UC1609C controller white on black

The library was tested on 1 and 2. 
This wiring Diagram from the manufacturer showing hardware setup connected to an ~8051 MCU, showing both 3.3 volt and 5 volt systems. 

![ ERM19264 ](https://github.com/gavinlyonsrepo/ERM19264_UC1609/blob/main/extras/image/connect.jpg)

## Software

### API Documentation

The Software is commented mostly for "doxygen" and if users uses "doxygen" software
an API document  can be generated. A Doxyfile is in "doc" sub folder in repository.

### SPI

Hardware SPI. The hardware SPI speed is set at 8MHz you can change this if necessary by changing LCDSPISetup method arguments.
Spi0 is used in example files but can be changed by passing a different SPI channel.(Spi0 or Spi1) 

### Fonts

There are 10 fonts packaged with library. Fonts can be easily added or removed by user.
All the Font data is in the font files.

Font data table: 

| num | Font pointer name | character size XxY |  ASCII range | Size in bytes |
| ------ | ------ | ------ | ------ |  ------ | 
| 1 | pFontDefault | 6x8 |  0 - 0xFE, Full Extended  | 1534 |
| 2 | pFontWide | 9x8 | 0x20 - 0x5A, NO lowercase letters | 535 |
| 3 | pFontPico | 4x6 | 0x20 - 0x7E  | 289 | 
| 4 | pFontSinclairS  | 8x8 | 0x20 - 0x7E | 764 |
| 5 | pFontMega | 16x16 | 0x20 - 0x7E | 3044 |
| 6 | pFontArialBold  | 16x16 | 0x20 - 0x7E |  3044 |
| 7 | pFontHallfetica | 16x16 | 0x20 - 0x7E | 3044 |
| 8 | pFontArialRound| 16x24 | 0x20 - 0x7E | 4564 |
| 9 | pFontGroTesk | 16x32 | 0x20 - 0x7A |  5828 |
| 10 | pFontSixteenSeg | 32x48 | 0x2D-0x3A , 0-10  :  .  / - only | 2692 |

Font size in bytes = ((X * (Y/8)) * numberOfCharacters) + (4*ControlByte)

| Font class Function | Notes |
| ------ | ------ | 
| writeChar| draws single character |
| writeCharString | draws character array |
| print | Polymorphic print class which will print out many data types |

These methods return an error code in event of an error.

**Remove a font**

To remove an unwanted font from project simply comment out or delete.

1. The Font data in ERM19264_UC1609_font.cpp file
2. The span to font at bottom of ERM19264_UC1609_font.cpp file
3. The associated extern declaration in the ERM19264_UC1609_font.hpp file

**Adding a new font**

1. Add the Font data in ERM19264_UC1609_font.cpp file
2. Add a span to font at bottom of ERM19264_UC1609_font.cpp file
3. Add an associated extern declaration in the ERM19264_UC1609_font.hpp file

The new ASCII font must have following font structure.
First 4 bytes are control bytes followed by vertically addressed font data.

```
// An 4 by 8 character size font starting at 
// ASCII offset 0x30 in ASCII table with 0x02 characters in font. 
// 0 and 1 
static const std::array<uint8_t, 12>FontBinaryExample[] =
{
0x04, 0x08, 0x30, 0x01,   // x-size, y-size, offset, total characters-1
(data),(data),(data),(data) // font data '0'
(data),(data),(data),(data) // font data '1'
}
```

*Sources*

1. Some of the fonts packaged with library came from [URL](http://rinkydinkelectronics.com/)
2. These fonts have horizontally addressed data.
3. They must be converted to vertically addressed font data if user wants them for ERM19264_UC1609_PICO.
4. There is a monochrome font maker there at [URL](http://rinkydinkelectronics.com/t_make_font_file_mono.php),
5. Just upload the picture of font from URL links on step 1 to it. 
6. The font you pick MUST have : Height(or y-size) must be divisible evenly by 8. (Width X Height)


### Bitmaps

There is a few different ways of displaying bitmaps, 

| Num | Method |  Data addressing | Note |
| ------ | ------  | ------ |  ------ |  
| 1 | LCDBitmap() |  Vertical |  Writes directly to screen , no buffer used. | 
| 2 | LCDBufferScreen() |  Vertical  | For internal use mostly | 
| 3 | drawBitmap() |  Vertical | default, setDrawBitmapAddr(true) | 
| 4 | drawBitmap() |  Horizontal | setDrawBitmapAddr(false) |

The drawBitmap method will return an error if : The Bitmap is completely off screen , 
Invalid Bitmap pointer object, bitmap bigger than screen , bitmap bigger/smaller than provided width and height calculation
( This helps prevents buffer overflow).
A vertical addressed Bitmap's height must be divisible by 8. eg, for a  bitmap with width=128 and height=64.
Bitmap excepted size = 128 * (64/8) = 1024 bytes.
A horizontal addressed Bitmap's width must be divisible by 8. eg, for a bitmap with width=88 and height=48.
Bitmap excepted size = (88/8) * 48 = 528 bytes.

Bitmaps can be turned to data [here at link]( https://javl.github.io/image2cpp/) 
See example file "BITMAP" for more details.



### User adjustments

If the LCD does not initialize correctly.
Try adjusting the RAM address control setting from 0x02(default) to 0x01.
See github issue 4(arduino Source port) for details. Users can adjust setting in the "begin" method argument list. This setting changes bits 2-0 in AC register, see diagram below for details.

![ ERM19264 ram image ](https://github.com/gavinlyonsrepo/ERM19264_UC1609/blob/main/extras/image/ram.png)

When the user calls LCDinit() to start LCD they can specify a contrast setting from 0x00 to 0xFF.
Datasheet says 0x49 is default. (VbiasPOT). Lower contrast works better on the blue color version.

It is also possible for user to change LCD bias, Temperature coefficient, frame rate and power control but this must be done by changing defines in header file. Choose lower frame rate for lower power, and choose higher frame rate to improve LCD contrast and minimize flicker. See Data sheet for range of values here. Defaults where found to be fine during all testing of this library.

| Parameter | default Values |  Define | Register bits |
| ------ | ------ |  ------ | ------ |
| LCD bias |  9 | BIAS_RATIO_SET | BR 1:0 |
| Temp coefficient | -0.00%/ C |  TEMP_COMP_SET | TC 1:0  |
| Frame rate | 95 fps |  FRAMERATE_SET |  LC 4:3 |
| Power control | 1.4mA + Internal VLCD (7x charge pump) |  PC_SET | PC 2:0 |
| V bias Bot(contrast) | 0x49 |  Set by user with LCDinit| PM 7:0 |
| Ram Address Control | 0x02 |  Set by user with LCDinit  | AC 2:0 |


### File system

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
| X_TEXT_GRAPHICS | Text + graphics  |
| X_FUNCTIONS_FPS | FPS test & misc functions, rotate , scroll,  etc |
| X_BITMAP | Shows use of bitmaps methods  |

## Notes and Issues


