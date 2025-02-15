/*!
   @file ERM19264_UC1609_graphics.hpp
   @brief ER_LCDM1 LCD driven by UC1609 controller header file
   for the graphics based functions.ERM19264_UC1609_PICO
   @author  Gavin Lyons
*/

#pragma once

#include <cmath> // for abs function
#include <cstdio>
#include "ERM19264_UC1609_Print.hpp"
#include "ERM19264_UC1609_font.hpp"

/*! @brief Graphics class to hold graphic related functions */
class ERM19264_graphics : public ERM19264_LCDFonts, public Print
{

public:
	ERM19264_graphics(int16_t w, int16_t h); // Constructor

	/*! Enum to hold current screen rotation in degrees bi color display  */
	enum LCD_rotate_e : uint8_t
	{
		LCD_Degrees_0 =   0,    /**< display screen rotated 0 degrees */
		LCD_Degrees_90 =  1,    /**< display screen rotated 90 degrees  */
		LCD_Degrees_180 = 2,    /**< display screen rotated 180 degrees  */
		LCD_Degrees_270 = 3     /**< display screen rotated 270 degrees */
	};

	virtual size_t write(uint8_t);
	DisplayRet::Ret_Codes_e writeChar(int16_t x, int16_t y, char value);
	DisplayRet::Ret_Codes_e writeCharString(int16_t x, int16_t y, char *text);
	virtual void drawPixel(int16_t x, int16_t y, uint8_t color) = 0;
	void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color);
	void drawFastVLine(int16_t x, int16_t y, int16_t h, uint8_t color);
	void drawFastHLine(int16_t x, int16_t y, int16_t w, uint8_t color);
	void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t color);
	void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t color);
	void fillScreen(uint8_t color);

	void drawCircle(int16_t x0, int16_t y0, int16_t r, uint8_t color);
	void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,
						  uint8_t color);
	void fillCircle(int16_t x0, int16_t y0, int16_t r, uint8_t color);
	void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,
						  int16_t delta, uint8_t color);
	void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
					  int16_t x2, int16_t y2, uint8_t color);
	void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
					  int16_t x2, int16_t y2, uint8_t color);
	void drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h,
					   int16_t radius, uint8_t color);
	void fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h,
					   int16_t radius, uint8_t color);

	void setDrawBitmapAddr(bool mode);
	uint8_t drawBitmap(int16_t x, int16_t y, std::span<const uint8_t> bitmap,
					   int16_t w, int16_t h, uint8_t color, uint8_t bg);

	void setCursor(int16_t x, int16_t y);
	void setTextWrap(bool w);

	void setRotation(LCD_rotate_e );
	LCD_rotate_e getRotation(void);

	int16_t height(void) const;
	int16_t width(void) const;

protected:
	const int16_t WIDTH;  /**< This is the 'raw' display w - never changes */
	const int16_t HEIGHT; /**< This is the 'raw' display h - never changes*/
	int16_t _width;		  /**< Display w as modified by current rotation*/
	int16_t _height;	  /**< Display h as modified by current rotation*/
	int16_t _cursor_x;	  /**< Current X co-ord cursor position */
	int16_t _cursor_y;	  /**< Current Y co-ord cursor position */

	bool _textwrap;		  /**< If set, 'wrap' text at right edge of display*/
	bool _drawBitmapAddr; /**< data addressing mode for method drawBitmap, True-vertical , false-horizontal */
	LCD_rotate_e LCD_rotate = LCD_Degrees_0; /**< Enum to hold rotation */

	private:
	/*!
		@brief Swaps the values of two int16_t variables.
		@param a Reference to the first integer.
		@param b Reference to the second integer.
		@note This function swaps the values in place without using additional memory.
	*/
	inline void LCDUC1609swap(int16_t& a, int16_t& b) {
		int16_t t = a;
		a = b;
		b = t;
	}
};
