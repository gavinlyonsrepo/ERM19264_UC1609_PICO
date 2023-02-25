/*
 * Project Name: ERM19264_UC1609
 * File: ERM19264_UC1609.cpp
 * Description: ERM19264 LCD driven by UC1609C controller source file
 * Author: Gavin Lyons.
 * URL: https://github.com/gavinlyonsrepo/ERM19264_UC1609_PICO
 */

#include "../include/erm19264/ERM19264_UC1609.hpp"

// Class Constructor
ERM19264_UC1609 ::ERM19264_UC1609(int8_t cd, int8_t rst, int8_t cs, int8_t sclk, int8_t din) : ERM19264_graphics(LCD_WIDTH, LCD_HEIGHT)

{
	_LCD_CD = cd;
	_LCD_RST = rst;
	_LCD_CS = cs;
	_LCD_DIN = din;
	_LCD_SCLK = sclk;
}

// Desc: begin Method initialise LCD
// Sets pinmodes and SPI setup
// Param1: VBiasPOT default = 0x49 , range 0x00 to 0xFE
void ERM19264_UC1609::LCDbegin(uint8_t VbiasPOT, spi_inst_t *spiType, uint32_t spiSpeedKhz)
{
	_VbiasPOT = VbiasPOT;

	gpio_init(_LCD_CD);
	gpio_init(_LCD_RST);
	gpio_init(_LCD_CS);
	gpio_init(_LCD_SCLK);
	gpio_init(_LCD_DIN);

	gpio_set_dir(_LCD_CD, GPIO_OUT);
	gpio_set_dir(_LCD_RST, GPIO_OUT);
	gpio_set_dir(_LCD_CS, GPIO_OUT);
	gpio_put(_LCD_CS, true);

	spi_inst_t *spi = spiType;
	// Initialize SPI port
	spi_init(spi, spiSpeedKhz * 1000);
	// Initialize SPI pins
	gpio_set_function(_LCD_SCLK, GPIO_FUNC_SPI);
	gpio_set_function(_LCD_DIN, GPIO_FUNC_SPI);

	// Set SPI format
	spi_set_format(spi,		   // SPI instance
				   8,		   // Number of bits per transfer
				   SPI_CPOL_0, // Polarity (CPOL)
				   SPI_CPHA_0, // Phase (CPHA)
				   SPI_MSB_FIRST);

	LCDinit();
}

// Desc: Called from LCDbegin carries out Power on sequence and register init
// Can be used to reset LCD to default values.
void ERM19264_UC1609::LCDinit()
{
	UC1609_CD_SetHigh;
	UC1609_CS_SetHigh;
	busy_wait_ms(UC1609_INIT_DELAY2);

	LCDReset();

	UC1609_CS_SetLow;

	send_command(UC1609_TEMP_COMP_REG, UC1609_TEMP_COMP_SET);
	send_command(UC1609_ADDRESS_CONTROL, UC1609_ADDRESS_SET);
	send_command(UC1609_FRAMERATE_REG, UC1609_FRAMERATE_SET);
	send_command(UC1609_BIAS_RATIO, UC1609_BIAS_RATIO_SET);
	send_command(UC1609_POWER_CONTROL, UC1609_PC_SET);
	busy_wait_ms(UC1609_INIT_DELAY);

	send_command(UC1609_GN_PM, 0);
	send_command(UC1609_GN_PM, _VbiasPOT); //  changed by user

	send_command(UC1609_DISPLAY_ON, 0x01);					  // turn on display
	send_command(UC1609_LCD_CONTROL, UC1609_ROTATION_NORMAL); // rotate to normal

	UC1609_CS_SetHigh;
}

// Desc: Sends a command to the display
// Param1: the command
// Param2: the values to change
void ERM19264_UC1609::send_command(uint8_t command, uint8_t value)
{
	UC1609_CD_SetLow;
	send_data(command | value);
	UC1609_CD_SetHigh;
}

// Desc: Resets LCD in a four wire setup called at start
// and  should also be called in a controlled power down setting
void ERM19264_UC1609::LCDReset()
{
	UC1609_RST_SetLow;
	busy_wait_ms(UC1609_RESET_DELAY);
	UC1609_RST_SetHigh;
	busy_wait_ms(UC1609_RESET_DELAY2);
}

// Desc: Powerdown procedure for LCD see datasheet P40
void ERM19264_UC1609::LCDPowerDown(void)
{
	LCDReset();
	LCDEnable(0);
	UC1609_CD_SetLow;
	UC1609_RST_SetLow;
	UC1609_CS_SetLow;
	UC1609_SCLK_SetLow;
	UC1609_SDA_SetLow;
}

// Desc: turns on display
// Param1: bits 1  on , 0 off
void ERM19264_UC1609::LCDEnable(uint8_t bits)
{
	UC1609_CS_SetLow;
	send_command(UC1609_DISPLAY_ON, bits);
	UC1609_CS_SetHigh;
}

// Desc: Scroll the displayed image up by SL rows.
// The valid SL value is between 0 (for no
// scrolling) and (64).
// Setting SL outside of this range causes undefined effect on the displayed
// image.
// Param1: bits 0-64 line number y-axis
void ERM19264_UC1609::LCDscroll(uint8_t bits)
{
	UC1609_CS_SetLow;
	send_command(UC1609_SCROLL, bits);
	UC1609_CS_SetHigh;
}

// Desc: Rotates the display
// Set LC[2:1] for COM (row) mirror (MY), SEG (column) mirror (MX).
// Param1: 4 possible values 000 010 100 110 (defined)
// If Mx is changed the buffer must BE updated
void ERM19264_UC1609::LCDrotate(uint8_t rotatevalue)
{
	UC1609_CS_SetLow;
	switch (rotatevalue)
	{
	case 0:
		rotatevalue = 0;
		break;
	case 0x02:
		rotatevalue = UC1609_ROTATION_FLIP_ONE;
		break;
	case 0x04:
		rotatevalue = UC1609_ROTATION_NORMAL;
		break;
	case 0x06:
		rotatevalue = UC1609_ROTATION_FLIP_TWO;
		break;
	default:
		rotatevalue = UC1609_ROTATION_NORMAL;
		break;
	}
	send_command(UC1609_LCD_CONTROL, rotatevalue);
	UC1609_CS_SetHigh;
}

// Desc: invert the display
// Param1: bits, 1 invert , 0 normal
void ERM19264_UC1609::invertDisplay(uint8_t bits)
{
	UC1609_CS_SetLow;
	send_command(UC1609_INVERSE_DISPLAY, bits);
	UC1609_CS_SetHigh;
}

// Desc: turns on all Pixels
// Param1: bits Set DC[1] to force all SEG drivers to output ON signals
// 1 all on ,  0 all off
void ERM19264_UC1609::LCDallpixelsOn(uint8_t bits)
{

	UC1609_CS_SetLow;
	send_command(UC1609_ALL_PIXEL_ON, bits);
	UC1609_CS_SetHigh;
}

// Desc: Fill the screen NOT the buffer with a datapattern
// Param1: datapattern can be set to zero to clear screen (not buffer) range 0x00 to 0ff
// Param2: optional delay in microseconds can be set to zero normally.
void ERM19264_UC1609::LCDFillScreen(uint8_t dataPattern = 0, uint8_t delay = 0)
{

	UC1609_CS_SetLow;
	uint16_t numofbytes = LCD_WIDTH * (LCD_HEIGHT / 8); // width * height
	for (uint16_t i = 0; i < numofbytes; i++)
	{
		send_data(dataPattern);
		busy_wait_us(delay);
	}
	UC1609_CS_SetHigh;
}

// Desc: Fill the chosen page(1-8)  with a datapattern
// Param1: datapattern can be set to 0 to FF (not buffer)
void ERM19264_UC1609::LCDFillPage(uint8_t dataPattern = 0)
{
	UC1609_CS_SetLow;
	uint16_t numofbytes = ((LCD_WIDTH * (LCD_HEIGHT / 8)) / 8); // (width * height/8)/8 = 192 bytes
	for (uint16_t i = 0; i < numofbytes; i++)
	{
		send_data(dataPattern);
	}
	UC1609_CS_SetHigh;
}

// Desc: Draw a bitmap in  to the screen
// Param1: x offset 0-192
// Param2: y offset 0-64
// Param3: width 0-192
// Param4 height 0-64
// Param5 the bitmap
void ERM19264_UC1609::LCDBitmap(int16_t x, int16_t y, uint8_t w, uint8_t h, const uint8_t *data)
{
	UC1609_CS_SetLow;

	uint8_t tx, ty;
	uint16_t offset = 0;
	uint8_t column = (x < 0) ? 0 : x;
	uint8_t page = (y < 0) ? 0 : y >> 3;

	for (ty = 0; ty < h; ty = ty + 8)
	{
		if (y + ty < 0 || y + ty >= LCD_HEIGHT)
		{
			continue;
		}
		send_command(UC1609_SET_COLADD_LSB, (column & 0x0F));
		send_command(UC1609_SET_COLADD_MSB, (column & 0xF0) >> 4);
		send_command(UC1609_SET_PAGEADD, page++);

		for (tx = 0; tx < w; tx++)
		{
			if (x + tx < 0 || x + tx >= LCD_WIDTH)
			{
				continue;
			}
			offset = (w * (ty >> 3)) + tx;
			send_data(data[offset]);
		}
	}
	UC1609_CS_SetHigh;
}

// Desc: Send data byte with SPI to UC1609C
// Param1: the data byte
void ERM19264_UC1609::send_data(uint8_t data)
{
	spi_write_blocking(spi, &data, 1);
}

// Desc: updates the buffer i.e. writes it to the screen
void ERM19264_UC1609::LCDupdate()
{
	uint8_t x = 0;
	uint8_t y = 0;
	uint8_t w = this->bufferWidth;
	uint8_t h = this->bufferHeight;
	LCDBuffer(x, y, w, h, (uint8_t *)this->LCDbuffer);
}

// Desc: clears the buffer i.e. does NOT write to the screen
void ERM19264_UC1609::LCDclearBuffer()
{
	memset(this->LCDbuffer, 0x00, (this->bufferWidth * (this->bufferHeight / 8)));
}

// Desc: Draw a bitmap to the screen
// Param1: x offset 0-192
// Param2: y offset 0-64
// Param3: width 0-192
// Param4 height 0-64
// Param5 the bitmap
void ERM19264_UC1609::LCDBuffer(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t *data)
{
	UC1609_CS_SetLow;

	uint8_t tx, ty;
	uint16_t offset = 0;
	uint8_t column = (x < 0) ? 0 : x;
	uint8_t page = (y < 0) ? 0 : y / 8;

	for (ty = 0; ty < h; ty = ty + 8)
	{
		if (y + ty < 0 || y + ty >= LCD_HEIGHT)
		{
			continue;
		}

		send_command(UC1609_SET_COLADD_LSB, (column & 0x0F));
		send_command(UC1609_SET_COLADD_MSB, (column & 0XF0) >> 4);
		send_command(UC1609_SET_PAGEADD, page++);

		for (tx = 0; tx < w; tx++)
		{
			if (x + tx < 0 || x + tx >= LCD_WIDTH)
			{
				continue;
			}
			offset = (w * (ty / 8)) + tx;
			send_data(data[offset++]);
		}
	}

	UC1609_CS_SetHigh;
}

// Desc: Draws a Pixel to the screen overides the custom graphics library
// Passed x and y co-ords and colour of pixel.
void ERM19264_UC1609::drawPixel(int16_t x, int16_t y, uint8_t colour)
{
	if ((x < 0) || (x >= this->bufferWidth) || (y < 0) || (y >= this->bufferHeight))
	{
		return;
	}
	uint16_t tc = (bufferWidth * (y / 8)) + x;
	switch (colour)
	{
	case FOREGROUND:
		this->LCDbuffer[tc] |= (1 << (y & 7));
		break;
	case BACKGROUND:
		this->LCDbuffer[tc] &= ~(1 << (y & 7));
		break;
	case INVERSE:
		this->LCDbuffer[tc] ^= (1 << (y & 7));
		break;
	}
}

//***********************************************
