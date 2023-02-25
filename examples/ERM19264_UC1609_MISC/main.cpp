// ******************************
// Example file name : main.cpp :: folder = ERM19264_UC1609_MISC
// Description:
// Test file for ERM19264_UC1609 library,
// (1) showing use of TEXT mode .
// (2) showing use of graphic functions from included graphics library.
// (3) showing misc functions
// (4) Fps test
// URL: https://github.com/gavinlyonsrepo/ERM19264_UC1609_PICO
// *****************************

/*
  Test list ::
  Text tests.  1-6
  Font tests.   7-14
  Graphic test. 15
  Misc functions test. 16A-E
  FPs Test. 17

  Test 1 Font size 3 float
  Test 2 font size 2 integer
  Test 3 font size 1 string inverted
  Test 4 draw a single character font size 4
  Test 4b drawtext method test
  Test 5 print ASCII  font 0-127
  Test 6 print ASCII font 128-255, if #define UC_FONT_MOD_TWO comment in will not work
  Test 7 "thick" font 2 (NO LOWERCASE)
  Test 8 "seven seg" font 3
  Test 9 "wide" font 4 (NO LOWERCASE)
  Test 10 "tiny" font 5
  Test 11 "homespun" font 6
  Test 12 "bigNums" font 7 (Numbers ONLY + : )
  Test 14 "medNums" font 8 (Numbers ONLY + : )

  Test 15 Graphics

  Test 16 function tests
  Test 16A All Pixels on
  Test 16B inverse
  Test 16C Rotate screen
  Test 16D Scroll Screen
  Test 16E Enable and disable Screen 

  Test 17 FPS
*/

// === Libraries ===
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "erm19264/ERM19264_UC1609.hpp"

// === Defines ===
#define LCDcontrast 0x49 // Constrast 00 to FE , 0x49 is datasheet default. User adjust.
#define myLCDHEIGHT 64
#define myLCDWIDTH 192
#define DisplayDelay1 5000
#define DisplayDelay2 0

// === Globals ===
const uint mosi_pin = 19;
const uint sck_pin = 18;
const uint cs_pin = 17;
const uint res_pin = 3;
const uint dc_pin = 2;

ERM19264_UC1609 myLCD(dc_pin, res_pin, cs_pin, sck_pin, mosi_pin);

// === Function prototypes ===
void DisplayText();
void DisplayFonts();
void DisplayGraphics();
void DisplayMiscTests();
void DisplayFPS();

// === Main ===
int main()
{
	// initialize the LCD , contrast , Spi interface , spi Baud rate in Khz
	myLCD.LCDbegin(LCDcontrast, spi0, 8000);

	myLCD.LCDFillScreen(0x0F, 0); // Splash  screen
	busy_wait_ms(1500);
	myLCD.setTextColor(FOREGROUND);
	myLCD.setTextSize(1);
	myLCD.setFontNum(UC1609Font_Default);

	// Buffer setup
	// Define a buffer to cover whole screen
	uint8_t screenBuffer[myLCDWIDTH * (myLCDHEIGHT / 8)]; // 1536 bytes = 192 * 64/8
	myLCD.LCDbuffer = (uint8_t *)&screenBuffer;			  // Assign the pointer to the buffer
	myLCD.LCDclearBuffer();								  // Clear  buffer

	DisplayText();
	DisplayFonts();
	DisplayGraphics();
	DisplayMiscTests();
	DisplayFPS();

	myLCD.LCDclearBuffer();
	myLCD.setCursor(5, 5);
	myLCD.print("End of tests");
	myLCD.LCDupdate(); // Write to the buffer
	busy_wait_ms(5000);

	myLCD.LCDPowerDown(); // Turn off DISPLAY when tests over
	while (1)
	{
		busy_wait_ms(100);
	}
}
// === END OF MAIN ===

// === Function Space ===

// Text tests 1-6 , A series of tests to display the text mode
void DisplayText()
{

	myLCD.setTextWrap(true);
	myLCD.LCDclearBuffer(); // Clear the buffer

	// Test 1
	myLCD.setTextColor(FOREGROUND);
	myLCD.setTextSize(3);
	myLCD.setCursor(0, 0);
	myLCD.print(3.1423);

	// Test 2
	myLCD.setTextSize(2);
	myLCD.setCursor(0, 30);
	int count = 123;
	myLCD.print(count);

	// Test 3
	myLCD.setTextSize(1);
	myLCD.setTextColor(BACKGROUND, FOREGROUND);
	myLCD.setCursor(0, 50);
	myLCD.print("Hello World");

	// Test 4
	myLCD.drawChar(95, 15, 'H', FOREGROUND, BACKGROUND, 6);

	myLCD.LCDupdate(); // Write to the buffer
	busy_wait_ms(DisplayDelay1);
	myLCD.LCDclearBuffer();

	// Test 4b
	char myString1[9] = {'1', '2', ':', '1', '3', ':', '2', '9'};
	myLCD.drawText(0, 0, myString1, FOREGROUND, BACKGROUND, 1);
	myLCD.drawText(0, 32, myString1, FOREGROUND, BACKGROUND, 2);

	myLCD.LCDupdate();
	busy_wait_ms(DisplayDelay1);
	myLCD.LCDclearBuffer();

	// Test 5
	myLCD.setCursor(0, 0);
	myLCD.setTextColor(FOREGROUND);
	myLCD.setTextSize(1);
	myLCD.print("ASCII font ");
	myLCD.setCursor(0, 15);

	// Print first 127 chars of font
	for (char i = 0; i < 126; i++)
	{
		if (i == '\n' || i == '\r')
			continue;
		myLCD.print(i);
		busy_wait_ms(DisplayDelay2);
	}
	myLCD.LCDupdate(); // Write to the buffer
	busy_wait_ms(DisplayDelay1);
	myLCD.LCDclearBuffer();

	myLCD.setCursor(0, 0);
	myLCD.setTextColor(FOREGROUND);
	myLCD.setTextSize(1);
	myLCD.print("ASCII font 128-255");

	uint8_t x = 0;
	uint8_t y = 15;
	myLCD.setCursor(x, y);

	for (uint8_t i = 128; i < 255; i++)
	{
		if (x > 180)
		{
			x = 0;
			y += 9;
		}
		myLCD.drawChar(x, y, i, FOREGROUND, BACKGROUND, 1);
		x += 7;
		busy_wait_ms(DisplayDelay2);
	}
	myLCD.LCDupdate(); // Write to the buffer
	busy_wait_ms(DisplayDelay1);
	myLCD.LCDclearBuffer();

	// Test 6
	myLCD.setTextColor(FOREGROUND, BACKGROUND);
	myLCD.setCursor(0, 0);
	myLCD.print(47, DEC);
	myLCD.setCursor(0, 16);
	myLCD.print(47, HEX);
	myLCD.setCursor(0, 32);
	myLCD.print(47, BIN);
	myLCD.setCursor(0, 48);
	myLCD.print(47, OCT);
	myLCD.LCDupdate(); // Write to the buffer
	busy_wait_ms(DisplayDelay1);
	myLCD.LCDclearBuffer();

} // end DisplayText

// Fonts tests 7 -14  , A series of tests to display the fonts
void DisplayFonts()
{
	// Test 7
	myLCD.setFontNum(UC1609Font_Default);
	myLCD.setTextSize(1);
	myLCD.setCursor(0, 0);
	myLCD.print("Thick Font:");
	myLCD.setFontNum(UC1609Font_Thick);
	myLCD.setCursor(0, 15);
	myLCD.print("1234567890123456");
	myLCD.drawChar(70, 25, 'H', FOREGROUND, BACKGROUND, 4);
	myLCD.setCursor(0, 45);
	myLCD.setTextSize(2);
	myLCD.print("TEST");
	myLCD.LCDupdate(); // Write to the buffer
	busy_wait_ms(DisplayDelay1);
	myLCD.LCDclearBuffer();

	// Test 8
	myLCD.setFontNum(UC1609Font_Default);
	myLCD.setTextSize(1);
	myLCD.setCursor(0, 0);
	myLCD.print("7 seg Font:");
	myLCD.setFontNum(UC1609Font_Seven_Seg);
	myLCD.setCursor(0, 15);
	myLCD.print("1234567890abcDEF780xyz45");
	myLCD.setCursor(0, 30);
	myLCD.setTextSize(4);
	myLCD.print("14:30");
	myLCD.LCDupdate(); // Write to the buffer
	busy_wait_ms(DisplayDelay1);
	myLCD.LCDclearBuffer();

	// Test 9
	myLCD.setFontNum(UC1609Font_Default);
	myLCD.setTextSize(1);
	myLCD.setCursor(0, 0);
	myLCD.print("Wide Font:");
	myLCD.setFontNum(UC1609Font_Wide);
	myLCD.setCursor(0, 15);
	myLCD.print("123456 XYZABC");
	myLCD.setCursor(0, 35);
	myLCD.setTextSize(2);
	myLCD.print("T 13:57");
	myLCD.LCDupdate(); // Write to the buffer
	busy_wait_ms(DisplayDelay1);
	myLCD.LCDclearBuffer();

	// Test 10 tiny
	myLCD.setFontNum(UC1609Font_Default);
	myLCD.setTextSize(1);
	myLCD.setCursor(0, 0);
	myLCD.print("tiny Font:");
	myLCD.setFontNum(UC1609Font_Tiny);
	myLCD.setCursor(0, 15);
	myLCD.print("123456 tiny");
	myLCD.setCursor(0, 35);
	myLCD.setTextSize(2);
	myLCD.print("T 11:52");
	myLCD.LCDupdate(); // Write to the buffer
	busy_wait_ms(DisplayDelay1);
	myLCD.LCDclearBuffer();

	// Test 11 homespun
	myLCD.setFontNum(UC1609Font_Default);
	myLCD.setTextSize(1);
	myLCD.setCursor(0, 0);
	myLCD.print("Home Font:");
	myLCD.setFontNum(UC1609Font_Homespun);
	myLCD.setCursor(0, 15);
	myLCD.print("123456 home");
	myLCD.setCursor(0, 35);
	myLCD.setTextSize(2);
	myLCD.print("T 19:24");
	myLCD.LCDupdate(); // Write to the buffer
	busy_wait_ms(DisplayDelay1);
	myLCD.LCDclearBuffer();

	// Test 12 big num
	myLCD.setFontNum(UC1609Font_Default);
	myLCD.setTextSize(1);
	myLCD.setCursor(0, 0);
	myLCD.print("BigNums Font:");

	myLCD.setFontNum(UC1609Font_Bignum);
	char myString[9] = {'1', '3', ':', '2', '6', ':', '1', '8'};
	myLCD.drawTextNumFont(0, 32, myString, BACKGROUND, FOREGROUND); // Test 10a drawTextBigNum
	myLCD.LCDupdate();												// Write to the buffer
	busy_wait_ms(DisplayDelay1);
	myLCD.LCDclearBuffer();

	// Test 12b
	myLCD.drawCharNumFont(0, 0, '8', FOREGROUND, BACKGROUND); // Test 10b drawCharBigNum
	myLCD.drawCharNumFont(40, 32, '4', BACKGROUND, FOREGROUND);
	myLCD.LCDupdate(); // Write to the buffer
	busy_wait_ms(2000);
	myLCD.LCDclearBuffer();

	// Test 12c using print with bigNums font
	myLCD.setTextColor(FOREGROUND, BACKGROUND);
	myLCD.setCursor(0, 0);
	myLCD.print(314);
	myLCD.setCursor(0, 32);
	myLCD.print(19.87, 1); // print 19.9
	myLCD.LCDupdate();	   // Write to the buffer
	busy_wait_ms(DisplayDelay1);
	myLCD.LCDclearBuffer();

	// Test 14 Mednum Font
	myLCD.setFontNum(UC1609Font_Default);
	myLCD.setTextSize(1);
	myLCD.setCursor(0, 0);
	myLCD.print("MedNums Font:");

	myLCD.setFontNum(UC1609Font_Mednum);
	char myString1[9] = {'1', '2', ':', '1', '3', ':', '2', '9'};
	myLCD.drawTextNumFont(0, 16, myString1, BACKGROUND, FOREGROUND); // Test 10a drawTextBigNum
	myLCD.LCDupdate();												 // Write to the buffer
	busy_wait_ms(DisplayDelay1);
	myLCD.LCDclearBuffer();

	// Test 14b
	myLCD.drawCharNumFont(0, 0, '9', FOREGROUND, BACKGROUND); // Test 10b drawCharBigNum
	myLCD.drawCharNumFont(40, 16, '2', BACKGROUND, FOREGROUND);
	myLCD.LCDupdate(); // Write to the buffer
	busy_wait_ms(2000);
	myLCD.LCDclearBuffer();

	// Test 14c using print with medNums font
	myLCD.setTextColor(FOREGROUND, BACKGROUND);
	myLCD.setCursor(0, 0);
	myLCD.print(314);
	myLCD.setCursor(0, 16);
	myLCD.print(19.288, 2); // print 19.29
	myLCD.LCDupdate();		// Write to the buffer
	busy_wait_ms(DisplayDelay1);
	myLCD.LCDclearBuffer();
} // end Display fonts

// Test 15 Function to display Graphics.
void DisplayGraphics()
{
	// Q1 ||  Q2
	//---------
	// Q3 ||  Q4
	//
	bool colour = 1;
	uint8_t count = 0;

	myLCD.LCDclearBuffer(); // Clear the buffer
	while (count < 50)
	{
		colour = !colour;

		// Draw the X
		myLCD.drawLine(96, 0, 96, 64, FOREGROUND);
		myLCD.drawFastVLine(94, 0, 64, FOREGROUND);
		myLCD.drawFastHLine(0, 32, 192, FOREGROUND);

		// Q1
		myLCD.fillRect(0, 10, 20, 20, colour);
		myLCD.fillCircle(40, 20, 10, FOREGROUND);
		myLCD.fillTriangle(60, 30, 70, 10, 80, 30, !colour);
		// Q2
		myLCD.drawRect(110, 10, 20, 20, FOREGROUND);
		// Q3
		myLCD.fillRoundRect(0, 40, 40, 20, 10, !colour);
		// Q4
		char i;
		for (i = 0; i < 10; i++)
		{
			myLCD.drawRect(100 + i, 40 + i, 80 - i * 2, 20 - i * 2, FOREGROUND);
			myLCD.LCDupdate();
			busy_wait_ms(50);
		}
		myLCD.LCDclearBuffer();
		count++;
	}
	myLCD.LCDclearBuffer();
	myLCD.LCDupdate();
}

// Functions tests 16A-E
void DisplayMiscTests()
{
	// Set text parameters
	myLCD.setTextSize(2);
	myLCD.setFontNum(UC1609Font_Default);
	
	// Test A LCD all pixels on
	myLCD.setCursor(20, 30);
	myLCD.print("All Pixels on");
	myLCD.LCDupdate();
	busy_wait_ms(4000);
	myLCD.LCDclearBuffer();
	myLCD.LCDupdate();
	myLCD.LCDallpixelsOn(1);
	busy_wait_ms(2000);
	myLCD.LCDallpixelsOn(0);
	busy_wait_ms(2000);

	// Test B inverse
	myLCD.setCursor(20, 30);
	myLCD.print("inverse test  ");
	myLCD.LCDupdate();
	myLCD.invertDisplay(0); // Normal
	busy_wait_ms(2000);
	myLCD.invertDisplay(1); // Inverted
	busy_wait_ms(4000);
	myLCD.invertDisplay(0);

	// Test C LCD rotate
	myLCD.LCDclearBuffer();
	myLCD.setCursor(20, 30);
	myLCD.print("rotate test");
	myLCD.LCDupdate();
	busy_wait_ms(2000);
	myLCD.LCDrotate(UC1609_ROTATION_FLIP_ONE);
	myLCD.LCDupdate();
	busy_wait_ms(5000);
	myLCD.LCDrotate(UC1609_ROTATION_FLIP_TWO);
	myLCD.LCDupdate();
	busy_wait_ms(5000);
	myLCD.LCDrotate(UC1609_ROTATION_FLIP_THREE);
	myLCD.LCDupdate();
	busy_wait_ms(5000);
	myLCD.LCDrotate(UC1609_ROTATION_NORMAL);
	myLCD.LCDupdate();
	busy_wait_ms(5000);

	// Test D LCD scroll
	myLCD.LCDclearBuffer();
	// myLCD.LCDupdate();
	myLCD.setCursor(0, 40);
	myLCD.print("scroll test");
	for (uint8_t i = 0; i < 62; i++)
	{
		myLCD.LCDscroll(i);
		myLCD.LCDupdate();
		busy_wait_ms(50);
	}
	myLCD.LCDscroll(0);

	// Test E LCD enable and disable
	myLCD.LCDclearBuffer();
	myLCD.setCursor(0, 30);
	myLCD.print("LCD Disable test");
	myLCD.LCDupdate();
	busy_wait_ms(5000);
	myLCD.LCDEnable(0);
	busy_wait_ms(5000);
	myLCD.LCDEnable(1);
	myLCD.LCDclearBuffer();
}

// Test 17 FPS frames per second test
void DisplayFPS()
{
	// Set text parameters
	myLCD.setTextSize(1);

	// Values to count frame rate per second
	long previousMillis = 0;
	long lastFramerate = 0;
	long currentFramerate = 0;
	uint16_t count = 0;
	uint16_t seconds = 0;
	uint16_t fps = 0;
	bool colour = 1;

	while (1)
	{
		unsigned long currentMillis = to_ms_since_boot(get_absolute_time());

		if (currentMillis - previousMillis >= 1000) // every second
		{
			fps = currentFramerate - lastFramerate;
			lastFramerate = currentFramerate;
			previousMillis = currentMillis;
			seconds++;
			colour = !colour;
			if (count >= 3000)
				return; // end if count gets to 3000
		}
		currentFramerate++;
		count++;

		//  ** Code to test **
		myLCD.setCursor(0, 10);
		myLCD.print("1536 bytes");
		myLCD.setCursor(0, 20);
		myLCD.print("G Lyons");
		myLCD.setCursor(0, 30);
		myLCD.print(count);
		myLCD.setCursor(0, 40);
		myLCD.print(fps);
		myLCD.print(" fps");
		myLCD.setCursor(0, 50);
		myLCD.print("V 1.5.0");
		myLCD.drawFastVLine(64, 0, 63, FOREGROUND);

		myLCD.fillRect(70, 10, 20, 20, colour);
		myLCD.fillCircle(110, 20, 10, !colour);
		myLCD.drawRoundRect(70, 40, 40, 20, 10, FOREGROUND);
		myLCD.LCDupdate();
	}
}
// ************** EOF **************
