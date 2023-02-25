// Example file name : main.cpp :: folder = ER_OLEDM1_CH1115_MISC
// Description:
// Test file for ER_OLEDM1_CH1115 library,
// (1) showing use of TEXT mode .
// (2) showing use of graphic functions from included graphics library.
// (3) showing misc functions  
// (4) Fps test
// URL: https://github.com/gavinlyonsrepo/ER_OLEDM1_CH1115_PICO
// *****************************

/*
  Test list:
  Text tests.  1-6
  Font tests.   7-14
  Graphic test. 15
  Misc functions test. 16A-F
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
  Test 16A OLED enable and disable
  Test 16B inverse
  Test 16C OLED flip
  Test 16D contrast
  Test 16E OLED scroll
  Test 16F fade ~ breath effect

  Test 17 FPS
*/

// === Libraries ===
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "ch1115/ER_OLEDM1_CH1115.hpp"

// === Defines ===
#define OLEDcontrast 0x80 //Contrast 00 to FF , 0x80 is default. user adjust
#define MYOLEDHEIGHT 64
#define MYOLEDWIDTH 128
#define DisplayDelay1 5000
#define DisplayDelay2 0

// === Globals ===
const uint mosi_pin = 19;
const uint sck_pin = 18;
const uint cs_pin = 17;
const uint res_pin = 3;
const uint dc_pin = 2;

ERMCH1115  myOLED(dc_pin, res_pin, cs_pin, sck_pin, mosi_pin); 

// === Function prototypes ===
void DisplayText();
void DisplayFonts();
void DisplayGraphics();
void DisplayMiscTests();
void DisplayFPS();

// === Main ===
int main() 
{
   // initialize the OLED , contrast , Spi interface , spi Baud rate in Khz
  myOLED.OLEDbegin(OLEDcontrast, spi0, 8000);
  
  myOLED.OLEDFillScreen(0x0F, 0); // Splash  screen
  busy_wait_ms(1500);
  myOLED.setTextColor(FOREGROUND);
  myOLED.setTextSize(1);
  myOLED.setFontNum(OLEDFontType_Default);

  // Buffer setup  
  // Define a buffer to cover whole screen
  uint8_t screenBuffer[MYOLEDWIDTH  * (MYOLEDHEIGHT / 8)]; // 1024 bytes = 128 * 64/8
  myOLED.OLEDbuffer = (uint8_t*) &screenBuffer;  // Assign the pointer to the buffer
  myOLED.OLEDclearBuffer();   // Clear  buffer

  DisplayText();
  DisplayFonts();
  DisplayGraphics();
  DisplayMiscTests();
  DisplayFPS();

  myOLED.OLEDclearBuffer();
  myOLED.setCursor(5, 5);
  myOLED.print("End of tests");
  myOLED.OLEDupdate();  // Write to the buffer
  busy_wait_ms(5000);
 
  myOLED.OLEDPowerDown(); // Turn off DISPLAY when tests over
  while (1) {busy_wait_ms(100);}
}
// === END OF MAIN ===

// === Function Space ===

// Text tests 1-6 , A series of tests to display the text mode
void DisplayText()
{

  myOLED.setTextWrap(true);
  myOLED.OLEDclearBuffer(); // Clear the buffer

  // Test 1
  myOLED.setTextColor(FOREGROUND);
  myOLED.setTextSize(3);
  myOLED.setCursor(0, 0);
  myOLED.print(3.1423);

  // Test 2
  myOLED.setTextSize(2);
  myOLED.setCursor(0, 30);
  int count = 123;
  myOLED.print(count);

  // Test 3
  myOLED.setTextSize(1);
  myOLED.setTextColor(BACKGROUND, FOREGROUND);
  myOLED.setCursor(0, 50);
  myOLED.print("Hello World");

  // Test 4
  myOLED.drawChar(95, 15 , 'H', FOREGROUND, BACKGROUND, 6);

  myOLED.OLEDupdate();  // Write to the buffer
  busy_wait_ms(DisplayDelay1);
  myOLED.OLEDclearBuffer();

  // Test 4b 
  char myString1[9] = {'1', '2', ':', '1', '3', ':', '2', '9'};
  myOLED.drawText(0,0, myString1, FOREGROUND, BACKGROUND,1);
  myOLED.drawText(0,32, myString1, FOREGROUND, BACKGROUND,2);
  
  myOLED.OLEDupdate();  
  busy_wait_ms(DisplayDelay1);
  myOLED.OLEDclearBuffer();

  // Test 5
  myOLED.setCursor(0, 0);
  myOLED.setTextColor(FOREGROUND);
  myOLED.setTextSize(1);
  myOLED.print("ASCII font ");
  myOLED.setCursor(0, 15);

  // Print first 127 chars of font
  for (char i = 0; i < 126; i++)
  {
    if (i == '\n' || i == '\r') continue;
    myOLED.print(i);
    busy_wait_ms(DisplayDelay2);
  }
  myOLED.OLEDupdate();  // Write to the buffer
  busy_wait_ms(DisplayDelay1);
  myOLED.OLEDclearBuffer();

  myOLED.setCursor(0, 0);
  myOLED.setTextColor(FOREGROUND);
  myOLED.setTextSize(1);
  myOLED.print("ASCII font 128-255");

  uint8_t x = 0;
  uint8_t y = 15;
  myOLED.setCursor(x, y);

  for (uint8_t i = 128; i < 255; i++)
  {
    if (x > 180)
    {
      x = 0;
      y += 9;
    }
    myOLED.drawChar(x, y , i, FOREGROUND, BACKGROUND, 1);
    x += 7;
    busy_wait_ms(DisplayDelay2);
   }
    myOLED.OLEDupdate();  // Write to the buffer
    busy_wait_ms(DisplayDelay1);
    myOLED.OLEDclearBuffer();

    // Test 6
    myOLED.setTextColor(FOREGROUND, BACKGROUND);
    myOLED.setCursor(0, 0);
    myOLED.print(47 , DEC);
    myOLED.setCursor(0, 16);
    myOLED.print(47 , HEX); 
    myOLED.setCursor(0, 32);
    myOLED.print(47, BIN);
    myOLED.setCursor(0, 48);
    myOLED.print(47 , OCT);
    myOLED.OLEDupdate();  // Write to the buffer
    busy_wait_ms(DisplayDelay1);
    myOLED.OLEDclearBuffer();
  
} // end DisplayText

// Fonts tests 7 -14  , A series of tests to display the fonts
void DisplayFonts()
{
  // Test 7
  myOLED.setFontNum(OLEDFontType_Default);
  myOLED.setTextSize(1);
  myOLED.setCursor(0, 0);
  myOLED.print("Thick Font:");
  myOLED.setFontNum( OLEDFontType_Thick );
  myOLED.setCursor(0, 15);
  myOLED.print("1234567890123456");
  myOLED.drawChar(70, 25 , 'H', FOREGROUND, BACKGROUND, 4);
  myOLED.setCursor(0, 45);
  myOLED.setTextSize(2);
  myOLED.print("TEST");
  myOLED.OLEDupdate();  // Write to the buffer
  busy_wait_ms(DisplayDelay1);
  myOLED.OLEDclearBuffer();

  // Test 8
  myOLED.setFontNum(OLEDFontType_Default );
  myOLED.setTextSize(1);
  myOLED.setCursor(0, 0);
  myOLED.print("7 seg Font:");
  myOLED.setFontNum(OLEDFontType_SevenSeg);
  myOLED.setCursor(0, 15);
  myOLED.print("1234567890abcDEF780xyz45");
  myOLED.setCursor(0, 30);
  myOLED.setTextSize(4);
  myOLED.print("14:30");
  myOLED.OLEDupdate();  // Write to the buffer
  busy_wait_ms(DisplayDelay1);
  myOLED.OLEDclearBuffer();

  // Test 9
  myOLED.setFontNum(OLEDFontType_Default );
  myOLED.setTextSize(1);
  myOLED.setCursor(0, 0);
  myOLED.print("Wide Font:");
  myOLED.setFontNum(OLEDFontType_Wide);
  myOLED.setCursor(0, 15);
  myOLED.print("123456 XYZABC");
  myOLED.setCursor(0, 35);
  myOLED.setTextSize(2);
  myOLED.print("T 13:57");
  myOLED.OLEDupdate();  // Write to the buffer
  busy_wait_ms(DisplayDelay1);
  myOLED.OLEDclearBuffer();

  // Test 10 tiny 
  myOLED.setFontNum(OLEDFontType_Default);
  myOLED.setTextSize(1);
  myOLED.setCursor(0, 0);
  myOLED.print("tiny Font:");
  myOLED.setFontNum(OLEDFontType_Tiny);
  myOLED.setCursor(0, 15);
  myOLED.print("123456 tiny");
  myOLED.setCursor(0, 35);
  myOLED.setTextSize(2);
  myOLED.print("T 11:52");
  myOLED.OLEDupdate();  // Write to the buffer
  busy_wait_ms(DisplayDelay1);
  myOLED.OLEDclearBuffer();

  // Test 11 homespun
  myOLED.setFontNum(OLEDFontType_Default);
  myOLED.setTextSize(1);
  myOLED.setCursor(0, 0);
  myOLED.print("Home Font:");
  myOLED.setFontNum(OLEDFontType_Homespun);
  myOLED.setCursor(0, 15);
  myOLED.print("123456 home");
  myOLED.setCursor(0, 35);
  myOLED.setTextSize(2);
  myOLED.print("T 19:24");
  myOLED.OLEDupdate();  // Write to the buffer
  busy_wait_ms(DisplayDelay1);
  myOLED.OLEDclearBuffer();
  
  // Test 12 big num
  myOLED.setFontNum(OLEDFontType_Default );
  myOLED.setTextSize(1);
  myOLED.setCursor(0, 0);
  myOLED.print("BigNums Font:");
  
  myOLED.setFontNum(OLEDFontType_Bignum);
  char myString[9] = {'1', '3', ':', '2', '6', ':', '1', '8'};
  myOLED.drawTextNumFont(0, 32, myString , BACKGROUND, FOREGROUND); // Test 10a drawTextBigNum
  myOLED.OLEDupdate();  // Write to the buffer
  busy_wait_ms(DisplayDelay1);
  myOLED.OLEDclearBuffer();

  // Test 12b 
  myOLED.drawCharNumFont(0, 0, '8', FOREGROUND, BACKGROUND); // Test 10b drawCharBigNum
  myOLED.drawCharNumFont(40, 32, '4', BACKGROUND, FOREGROUND);
  myOLED.OLEDupdate();  // Write to the buffer
  busy_wait_ms(2000);
  myOLED.OLEDclearBuffer();

  // Test 12c using print with bigNums font
  myOLED.setTextColor(FOREGROUND, BACKGROUND);
  myOLED.setCursor(0, 0);
  myOLED.print(314);
  myOLED.setCursor(0, 32);
  myOLED.print(19.87 , 1); // print 19.9
  myOLED.OLEDupdate();  // Write to the buffer
  busy_wait_ms(DisplayDelay1);
  myOLED.OLEDclearBuffer();

  // Test 14 Mednum Font
  myOLED.setFontNum(OLEDFontType_Default );
  myOLED.setTextSize(1);
  myOLED.setCursor(0, 0);
  myOLED.print("MedNums Font:");
  
  myOLED.setFontNum(OLEDFontType_Mednum);
  char myString1[9] = {'1', '2', ':', '1', '3', ':', '2', '9'};
  myOLED.drawTextNumFont(0, 16, myString1 , BACKGROUND, FOREGROUND); // Test 10a drawTextBigNum
  myOLED.OLEDupdate();  // Write to the buffer
  busy_wait_ms(DisplayDelay1);
  myOLED.OLEDclearBuffer();

  // Test 14b 
  myOLED.drawCharNumFont(0, 0, '9', FOREGROUND, BACKGROUND); // Test 10b drawCharBigNum
  myOLED.drawCharNumFont(40, 16, '2', BACKGROUND, FOREGROUND);
  myOLED.OLEDupdate();  // Write to the buffer
  busy_wait_ms(2000);
  myOLED.OLEDclearBuffer();

  // Test 14c using print with medNums font
  myOLED.setTextColor(FOREGROUND, BACKGROUND);
  myOLED.setCursor(0, 0);
  myOLED.print(314);
  myOLED.setCursor(0, 16);
  myOLED.print(19.288 , 2); // print 19299
  myOLED.OLEDupdate();  // Write to the buffer
  busy_wait_ms(DisplayDelay1);
  myOLED.OLEDclearBuffer();
} // end Display fonts

// Test 15 Function to display Graphics.
void  DisplayGraphics()
{
  //Q1 ||  Q2
  //---------
  //Q3 ||  Q4
  //
  bool colour = 1;
  uint8_t count = 0;

  myOLED.OLEDclearBuffer(); // Clear the buffer
  while (count < 50)
  {
    colour = !colour;

    // Draw the X
    myOLED.drawLine(64,  0, 64, 64, FOREGROUND);
    myOLED.drawFastVLine(62, 0, 64, FOREGROUND);
    myOLED.drawFastHLine(0, 32, 128, FOREGROUND);

    // Q1
    myOLED.fillRect(0, 10, 20, 20, colour);
    myOLED.fillCircle(40, 20, 10, FOREGROUND);

    // Q2
    myOLED.fillTriangle(80, 25, 90, 5, 100, 25, !colour);
    myOLED.drawRect(105, 10, 15, 15, FOREGROUND);
    // Q3
    myOLED.fillRoundRect(0, 40, 40, 20, 10, !colour);
    // Q4
    char i;
    for (i = 0; i < 10; i ++)
    {
      myOLED.drawRect(70 + i, 40 + i, 50 - i * 2, 20 - i * 2, FOREGROUND);
      myOLED.OLEDupdate();
      busy_wait_ms(50);
    }
    myOLED.OLEDclearBuffer();
    count++;
  }
  myOLED.OLEDclearBuffer();
  myOLED.OLEDupdate();
}

// Functions tests 16A-F
void DisplayMiscTests()
{
  myOLED.setFontNum(OLEDFontType_Default);
  myOLED.setTextSize(1);

  //  ** Test A OLED enable and disable **
  myOLED.setCursor(0, 30);
  myOLED.print("OLED Disable test 1");
  myOLED.OLEDupdate();

  busy_wait_ms(3000); //display message
  myOLED.OLEDEnable(0); //turn off display (sleep mode 500uA measured)
  busy_wait_ms(3000); //wait for 3
  myOLED.OLEDEnable(1); //turn on
  busy_wait_ms(2000); //display message
  myOLED.OLEDclearBuffer();

  // ** Test B inverse **
  myOLED.setCursor(0, 10);
  myOLED.print("OLED Inverse test 2");
  myOLED.OLEDupdate();
  busy_wait_ms(3000);
  myOLED.OLEDInvert(1); // Inverted
  busy_wait_ms(5000);
  myOLED.OLEDInvert(0);
  busy_wait_ms(1000);


  // ** Test C OLED flip **
  myOLED.OLEDclearBuffer();
  myOLED.setCursor(5, 5);
  myOLED.print("Flip test 3");
  myOLED.OLEDupdate();
  busy_wait_ms(4000);
  myOLED.OLEDFlip(1);
  busy_wait_ms(4000);
  myOLED.OLEDFlip(0);
  busy_wait_ms(2000);

  // ** Test D contrast **
  myOLED.OLEDclearBuffer();
  myOLED.setCursor(0, 0);
  myOLED.print("Contrast test 4");
  myOLED.OLEDupdate();
  busy_wait_ms(2500);
  myOLED.OLEDFillScreen(0x77, 0); // fill screen

  myOLED.OLEDContrast(0x00);
  busy_wait_ms(1000);
  myOLED.OLEDContrast(0x80);
  busy_wait_ms(1000);
  myOLED.OLEDContrast(0xFF);
  busy_wait_ms(1000);
  myOLED.OLEDContrast(OLEDcontrast);
  busy_wait_ms(1000);
  myOLED.OLEDclearBuffer();


  // ** Test E OLED scroll **
  myOLED.OLEDclearBuffer();
  myOLED.setCursor(0, 40);
  myOLED.print("Scroll test 5 ");
  myOLED.OLEDupdate();
  busy_wait_ms(2500);

  // See .cpp file for more info on these parameters.
  uint8_t timeInterval = 0x00; // 6 frames 0x00 - 0x07
  uint8_t scrollDirection = 0x26; //right 0x26 or 0x27
  uint8_t scrollMode = 0x28; // continuous 0x28-0x2A,
  myOLED.OLEDscrollSetup(timeInterval, scrollDirection , scrollMode);
  myOLED.OLEDscroll(1); //start scroll
  busy_wait_ms(15000);
  myOLED.OLEDscroll(0); // stop Scroll

  busy_wait_ms(1);

  timeInterval = 0x02; // 64 frames , 0x00 - 0x07
  scrollDirection = 0x27; // left , 0x26 or 0x27
  scrollMode = 0x29; // continuous 0x28 one shot 0x29 , one col 0x2A,
  myOLED.OLEDscrollSetup(timeInterval, scrollDirection , scrollMode);
  myOLED.OLEDscroll(1); //start
  busy_wait_ms(15000);
  myOLED.OLEDscroll(0); // stop

  // ** TEST F fade ~ breath effect **
  myOLED.OLEDclearBuffer();
  myOLED.OLEDclearBuffer();
  myOLED.setCursor(5, 5);
  myOLED.print("Fade effect test 6");
  myOLED.OLEDupdate();
  busy_wait_ms(3000);
  myOLED.OLEDfadeEffect();
  myOLED.OLEDFillScreen(0xE3, 0);
  busy_wait_ms(10000);
  myOLED.OLEDfadeEffect(0x00); // switch off fade

  myOLED.OLEDclearBuffer();
  myOLED.OLEDupdate();
}

// Test 5 FPS frames per second test 
void DisplayFPS(){

// Values to count frame rate per second
  long previousMillis  = 0;
  long lastFramerate = 0;
  long currentFramerate = 0;
  uint16_t count  = 0;
  uint16_t seconds  = 0;
  uint16_t fps = 0;
  bool colour = 1;

  while(1)
  {
    unsigned long currentMillis = to_ms_since_boot(get_absolute_time());

    if (currentMillis  - previousMillis  >= 1000) // every second
    {
      fps = currentFramerate - lastFramerate;
      lastFramerate = currentFramerate ;
      previousMillis  = currentMillis;
      seconds++;
      colour = !colour;
      if (count >= 3000)return; // end if count gets to 3000
    }
    currentFramerate++;
    count++;

    //  ** Code to test ** 
    myOLED.setCursor(0, 10);
    myOLED.print("1024 bytes");
    myOLED.setCursor(0, 20);
    myOLED.print("G Lyons");
    myOLED.setCursor(0, 30);
    myOLED.print(count);
    myOLED.setCursor(0, 40);
    myOLED.print(fps);
    myOLED.print(" fps");
    myOLED.setCursor(0, 50);
    myOLED.print("V 1.3.1");
    myOLED.drawFastVLine(64, 0, 63, FOREGROUND);

    myOLED.fillRect(70, 10, 20, 20, colour);
    myOLED.fillCircle(110, 20, 10, !colour);
    myOLED.drawRoundRect(70, 40, 40, 20, 10, FOREGROUND);
    myOLED.OLEDupdate();
  }

}
// ************** EOF **************
