/*
* Project Name: ERM19264_UC1609
* File: ERM19264_UC1609_font.hpp
* Description: ERM19264 LCD driven by UC1609C controller, font data file 8 fonts.
* Author: Gavin Lyons.
* URL: https://github.com/gavinlyonsrepo/ERM19264_UC1609_PICO
*/

#ifndef ERM19264_FONT_S_H
#define ERM19264_FONT_S_H

#include "stdint.h"

// **********************************************
// ****** USER FONT OPTION ONE *************
// ****** FONT DEFINE SECTION *************** 
// Comment in the fonts YOU want, One is default. 

#define UC1609_Font_One  // (1) default  (FUll ASCII with mods)
#define UC1609_Font_Two  // (2) thick (NO LOWERCASE)
#define UC1609_Font_Three // (3) seven segment 
#define UC1609_Font_Four // (4) wide (NO LOWERCASE)
#define UC1609_Font_Five // (5) tiny
#define UC1609_Font_Six // (6) HomeSpun
#define UC1609_Font_Seven // (7) big numbers (NUMBERS ONLY )
#define UC1609_Font_Eight // (8) Med numbers (NUMBERS ONLY )

// ****** END OF FONT DEFINE SECTION ******  
// **********************************************

// ************ USER OPTION TWO**************
// This is defined to include full extended ASCII set 127-255
// To save memory and reduce program size (635 bytes saving) comment out
// Note Font_one only.
#define UC_FONT_MOD_TWO
//*****************************************

// Font data is in the cpp file accessed thru extern pointers.

#ifdef UC1609_Font_One 
extern const unsigned char * pFontDefaultptr;
#endif
#ifdef UC1609_Font_Two 
extern const unsigned char * pFontThickptr;
#endif
#ifdef UC1609_Font_Three
extern const unsigned char * pFontSevenSegptr;
#endif
#ifdef UC1609_Font_Four
extern const unsigned char * pFontWideptr;
#endif
#ifdef UC1609_Font_Five
extern const  unsigned char * pFontTinyptr;
#endif
#ifdef UC1609_Font_Six
extern const unsigned char * pFontHomeSpunptr;
#endif
#ifdef UC1609_Font_Seven
extern const uint8_t (* pFontBigNumptr)[64];
#endif
#ifdef UC1609_Font_Eight
extern const uint8_t (* pFontMedNumptr)[32]; 
#endif


#endif // font file guard header
