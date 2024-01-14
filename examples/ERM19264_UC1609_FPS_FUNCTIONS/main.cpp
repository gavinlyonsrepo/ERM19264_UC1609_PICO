/*!
	@file main.cpp
	@author Gavin Lyons
	@brief Test file for ERM19264_UC1609_PICO library, showing use of various functions + FPS test

	@test
		-# Test 501 All Pixels on
		-# Test 502 Invert screen color
		-# Test 503 Rotate screen command
		-# Test 504 rotate screen buffer
		-# Test 505 Scroll Screen
		-# Test 506 Enable and disable screen
		-# Test 601 Frame rate per second test. FPS.
*/

// === Libraries ===
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "erm19264/ERM19264_UC1609.hpp"


// === Test setup ===
// Test Timing configuration
#define DisplayDelay1 5000
#define DisplayDelay2 0

// Screen settings
#define LCDcontrast 0x49	// Contrast
#define LCDRAMADDRCTRL 0x02 // RAM address control: Range 0-0x07, optional, default 0x02
#define myLCDwidth 192
#define myLCDheight 64
#define myScreenSize (myLCDwidth * (myLCDheight / 8))
uint8_t screenBuffer[myScreenSize]; 

// GPIO
const uint mosi_pin = 19;
const uint sck_pin = 18;
const uint cs_pin = 17;
const uint res_pin = 3;
const uint dc_pin = 2;

// SPI configuration
uint32_t mySPIBaudRate = 8000;
spi_inst_t *mySpiInst = spi0;

// instantiate  an LCD object
ERM19264_UC1609 myLCD(myLCDwidth, myLCDheight);


// === Function prototypes ===
void SetupTest(void);
void TestReset(void);
void EndTest(void);
void DisplayMiscTests();
void DisplayFPS();

// === Main ===
int main()
{
	SetupTest();
	DisplayMiscTests();
	DisplayFPS();
	EndTest();
}
// === END OF MAIN ===

// === Function Space ===

void SetupTest()
{
	stdio_init_all(); // Initialize chosen serial port, default 38400 baud
	busy_wait_ms(750);
	printf("LCD ERM19264:: Start!\r\n");
	myLCD.LCDSPISetup(mySpiInst, mySPIBaudRate, dc_pin, res_pin, cs_pin, sck_pin, mosi_pin);
	myLCD.LCDinit(LCDcontrast, LCDRAMADDRCTRL);
	if (myLCD.LCDSetBufferPtr(myLCDwidth, myLCDheight, screenBuffer, sizeof(screenBuffer)) != 0)
	{
		printf("SetupTest : ERROR : LCDSetBufferPtr Failed!\r\n");
		while (1)
		{
			busy_wait_ms(1000);
		}
	}							  // Initialize the buffer
	myLCD.LCDFillScreen(0xF0, 0); // splash screen bars
	busy_wait_ms(1000);
}

void EndTest()
{
	myLCD.LCDPowerDown();  // Switch off display
	spi_deinit(mySpiInst); // Turn off SPI
	printf("LCD ERM19264 :: End\r\n");
}


// Functions tests 
void DisplayMiscTests()
{
	// Set text parameters
	myLCD.setFont(pFontDefault);
	// Test 501 LCD all pixels on
	myLCD.setCursor(5, 30);
	myLCD.print("All Pixels on");
	myLCD.LCDupdate();
	busy_wait_ms(4000);
	myLCD.LCDclearBuffer();
	myLCD.LCDupdate();
	myLCD.LCDAllpixelsOn(1);
	busy_wait_ms(2000);
	myLCD.LCDAllpixelsOn(0);
	busy_wait_ms(2000);

	// Test 502 inverse
	myLCD.setCursor(5, 30);
	myLCD.print("Invert test");
	myLCD.LCDupdate();
	myLCD.LCDInvertDisplay(0); // Normal
	busy_wait_ms(2000);
	myLCD.LCDInvertDisplay(1); // Inverted
	busy_wait_ms(4000);
	myLCD.LCDInvertDisplay(0);

	// Test 503 LCD rotate command
	myLCD.LCDclearBuffer();
	myLCD.setCursor(5, 30);
	myLCD.print("rotate Cmd test");
	myLCD.LCDupdate();
	busy_wait_ms(2000);
	myLCD.LCDsetRotateCmd(UC1609_ROTATION_FLIP_ONE);
	myLCD.LCDupdate();
	busy_wait_ms(5000);
	myLCD.LCDsetRotateCmd(UC1609_ROTATION_FLIP_TWO);
	myLCD.LCDupdate();
	busy_wait_ms(5000);
	myLCD.LCDsetRotateCmd(UC1609_ROTATION_FLIP_THREE);
	myLCD.LCDupdate();
	busy_wait_ms(5000);
	myLCD.LCDsetRotateCmd(UC1609_ROTATION_NORMAL);
	myLCD.LCDupdate();
	busy_wait_ms(5000);

	// Test 504 LCD rotate the buffer
	myLCD.LCDclearBuffer();
	myLCD.setCursor(5, 30);
	myLCD.print("rotate Buffer test");
	myLCD.LCDupdate();
	busy_wait_ms(2000);
	myLCD.LCDclearBuffer();

	myLCD.setCursor(5, 5);
	myLCD.setRotation(LCD_Degrees_0);
	myLCD.print("rotate 0");
	myLCD.LCDupdate();
	busy_wait_ms(5000);
	myLCD.LCDclearBuffer();

	myLCD.setRotation(LCD_Degrees_90);
	myLCD.setCursor(5, 5);
	myLCD.print("rotate 90");
	myLCD.LCDupdate();
	busy_wait_ms(5000);
	myLCD.LCDclearBuffer();

	myLCD.setRotation(LCD_Degrees_180);
	myLCD.setCursor(5, 5);
	myLCD.print("rotate 180");
	myLCD.LCDupdate();
	busy_wait_ms(5000);
	myLCD.LCDclearBuffer();

	myLCD.setRotation(LCD_Degrees_270);
	myLCD.setCursor(5, 5);
	myLCD.print("rotate  270");
	myLCD.LCDupdate();
	busy_wait_ms(5000);
	myLCD.setRotation(LCD_Degrees_0); //back to normal rotation
	myLCD.LCDclearBuffer();

	// Test 505 LCD scroll

	myLCD.LCDclearBuffer();
	myLCD.setCursor(0, 40);
	myLCD.print("scroll test");
	for (uint8_t i = 0; i < 62; i++)
	{
		myLCD.LCDscroll(i);
		myLCD.LCDupdate();
		busy_wait_ms(50);
	}
	myLCD.LCDscroll(0);

	// Test 506 LCD enable and disable
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

// Test 601 FPS frames per second test
void DisplayFPS()
{
	myLCD.setFont(pFontDefault);
	// Values to count frame rate per second
	long previousMillis = 0;
	long lastFramerate = 0;
	long currentFramerate = 0;
	uint16_t count = 3000;
	uint16_t seconds = 0;
	uint16_t fps = 0;
	bool colour = 1;

	while (count >= 5)
	{
		unsigned long currentMillis = to_ms_since_boot(get_absolute_time());

		if (currentMillis - previousMillis >= 1000) // every second
		{
			fps = currentFramerate - lastFramerate;
			lastFramerate = currentFramerate;
			previousMillis = currentMillis;
			seconds++;
			colour = !colour;
		}
		currentFramerate++;
		count--;

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
		myLCD.print(myLCD.GetLibVerNum()); 

		myLCD.drawFastVLine(96, 0, 63, FG_COLOR);
		myLCD.fillRect(100, 10, 20, 20, colour);
		myLCD.fillCircle(130, 20, 10, !colour);
		myLCD.drawRoundRect(110, 40, 40, 20, 10, FG_COLOR);
		myLCD.LCDupdate();
	}
}
// ************** EOF **************
