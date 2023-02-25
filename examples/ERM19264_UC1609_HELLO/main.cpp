// ******************************
// Example file name : main.cpp :: folder = ERM19264_UC1609_HELLO
// Description:
// Test file for ERM19264_UC1609 library, showing hello world
// URL: https://github.com/gavinlyonsrepo/ERM19264_UC1609_PICO
// *****************************

// === Libraries ===
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "erm19264/ERM19264_UC1609.hpp"

// === Defines ===
#define LCDcontrast 0x49 //Constrast 00 to FE , 0x49 is datasheet default. User adjust.
#define myLCDHEIGHT 64
#define myLCDWIDTH 192

// === Globals ===
const uint mosi_pin = 19;
const uint sck_pin = 18;
const uint cs_pin = 17;
const uint res_pin = 3;
const uint dc_pin = 2;

ERM19264_UC1609 myLCD(dc_pin, res_pin, cs_pin, sck_pin, mosi_pin);

// === Main ===
int main()
{

	busy_wait_ms(50);

	// Screen Setup :
	// initialize the LCD , contrast , Spi interface , spi Baud rate in Khz
	myLCD.LCDbegin(LCDcontrast, spi0, 8000);
	myLCD.setTextColor(FOREGROUND);
	myLCD.setFontNum(UC1609Font_Default);
	myLCD.LCDFillScreen(0x00, 0);

	// Buffer setup
	// Define a buffer to cover whole screen
	uint8_t screenBuffer[myLCDWIDTH * (myLCDHEIGHT / 8)]; // 1536 bytes = 192 * 64/8
	myLCD.LCDbuffer = (uint8_t *)&screenBuffer;		// Assign the pointer to the buffer
	myLCD.LCDclearBuffer();							// Clear  buffer

	while (1)
	{
		myLCD.setCursor(20, 20);
		myLCD.print("Hello world");
		myLCD.LCDupdate(); // Update  active buffer
		busy_wait_ms(5000);
	}
}
//=== end of main ===