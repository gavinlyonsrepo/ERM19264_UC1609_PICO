/*!
	@file ERM19264_UC1609_font.hpp
	@brief LCD driven by UC1609 controller. font data file 10 fonts.
	@author Gavin Lyons.
	@details
		-#  1. pFontDefault  6 by 8
		-#  2. pFontWide  9 by 8 (NO LOWERCASE)
		-#  3. pFontpico 3 by 6
		-#  4. pFontSinclairS 8 by 8
		-#  5. pFontMega  Mega 16 by 16
		-#  6. pFontArialBold 16 by 16
		-#  7. pFontHallfetica 16 by 16
		-#  8. pFontArialRound 16 by 24
		-#  9. pFontGroTesk 16 by 32
		-#  10. pFontSixteenSeg 32 by 48 (NUMBERS ONLY + : . -)
*/

#pragma once

#include <cstdint>

// Font data is in the cpp file accessed thru extern pointers.
extern const uint8_t *pFontDefault;	   /**< pFontDefault  6 by 8 */
extern const uint8_t *pFontWide;	   /**< pFontWide  9 by 8 (NO LOWERCASE) */
extern const uint8_t *pFontPico;	   /**< pFontpico 3 by 6 */
extern const uint8_t *pFontSinclairS;  /**< pFontSinclairS 8 by 8 */
extern const uint8_t *pFontMega;	   /**< pFontMega  Mega 16 by 16 */
extern const uint8_t *pFontArialBold;  /**< pFontArialBold 16 by 16 */
extern const uint8_t *pFontHallfetica; /**< pFontHallfetica 16 by 16*/
extern const uint8_t *pFontArialRound; /**< pFontArialRound 16 by 24 */
extern const uint8_t *pFontGroTesk;	   /**< pFontGroTesk 16 by 32 */
extern const uint8_t *pFontSixteenSeg; /**< pFontSixteenSeg 32 by 48 (NUMBERS ONLY + : . -)a */
