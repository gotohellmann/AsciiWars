/*******************************************************

                    AsciiWars

 code by        F. Hellmann  <www.gotohellmann.com>
 animation by   S. Jansen    <www.asciimation.co.nz>
 font by        S. Weber     <https://blogfonts.com/picopixel.font>

 This sketch is playing back an ASCII animation of 
 StarWars on a tiny 2.2" LCD Screen. 

 !!! This version needs a patched TFT_eSPI lib from my github !!!

 Platform: Arduino on WEMOS D1 mini Module (4MB Flash)
           ILI9341 240x320 Pixel RGB LCD Display


 Usage:     - Connect LCD to Wemos D1 mini
              -> see connection infos below
            - Start Arduino IDE
            - check you have the ESP8266FS tool under Tools
              -> if not: https://github.com/esp8266/arduino-esp8266fs-plugin
              
            - check that you have the necessary libs installed:
                - TFT_eSPI
                  Dont forget to edit Library User_Settings.h
                 
            - Uncomment SPIFFS Formatting Block below
            - Arduino Board Select: Wemos D1 mini
            - Change to Flash Size: 4M (3M SPIFFS)
            - Open Serial Monitor and Upload Sketch
            - Check for SPIFFS formatting progress
            - Comment out the SPIFFS Formatting Block
            
            - Close Serial Monitor 
            - Upload Data with  ESP8266FS tool
            - Upload Sketch _without_ SPIFFS Formatting Block
            
            -> Have fun.

 Performance:

            - To reduce flicker patch TFT_eSPI Library (Line 2780):
            
            ...   if(size > 1) {
                    xo16 = xo;
                    yo16 = yo;
                  }
            
                  // PATCH for AsciiWars:
                  if (bg != color) {
                    fillRect(x,y-5,6,7,bg);
                  }
                  
                  // GFXFF rendering speed up
                  uint16_t hpc = 0; // Horizontal foreground pixel count
                  for(yy=0; yy<h; yy++) {
                    for(xx=0; xx<w; xx++) {
            ...

            - A patched TFT_eSPI Lib is available on my github: 
              https://github.com/gotohellmann/
              
 *******************************************************/

// TFT Libraries
#include "SPI.h"
#include <TFT_eSPI_patched.h>
#include "iliPicopixel.h"

// SPIFFS filesystem
#include <FS.h>
#include <LittleFS.h>
File ani;               // File handle for animation

int wait;               // How long to wait til next frame
String line;            // Current line in animation file
char padded[80];        // Format String into
int frame;              // current frame

#define DELAY 60

// Hardware Pins for the ILI9341 LCD:
//
//         Wemos D1   -> LCD
//        -------------------------
// VCC     +3V3 VCC      VCC
// GND     GND           GND
// CS      D8            CS    
// RST     +3V3 VCC      RST
// DC      D3            DC / RS
// SDI     D7            SDI (MOSI)
// SCLK    D5            SCK 
// LED     +3V3 VCC      LED
// SDO     D6            SDO (MISO)
//
#define DARKGREEN 0x01E0 

TFT_eSPI tft = TFT_eSPI();

void setup() {
  Serial.begin(9600);
  Serial.println("AsciiWars V0.1"); 

  // LCD Module setup
  // we use an ILI9341 240x320 display
  tft.begin();
  Serial.println(F("TFT setup done."));
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(108, 16);
  tft.setTextColor(DARKGREEN, TFT_BLACK);
  tft.setTextSize(2);
  tft.println("AsciiWars");
  tft.setTextSize(1);
  tft.setCursor(20, 220);
  tft.println("Animation: Simon Jansen - Code: Frank Hellmann");

  // SPIFFS Filesystem setup
  SPIFFS.begin();

  //------------------------ Formatting -------------------------
  // Comment in to format a blank WEMOS Module SPIFFS with this.
  //                  !!! Do this only once !!! ;-)
  /*
  Serial.println("Please wait 30 secs for SPIFFS to be formatted");
  SPIFFS.format();
  Serial.println("SPIFFS formatted. Don't forget to transfer new files onto it.");
  */

  // Check that our asciimation.txt file is accessible
  Serial.println("Opening asciimation.txt file.");
  ani = SPIFFS.open("/asciimation.txt", "r");
  if (!ani) {
      // Not accessible -> notify and stop.      
      Serial.println("asciimation.txt is missing.");
      tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
      tft.setTextSize(1);
      tft.setCursor(40,120);
      tft.println("The asciimation.txt file is missing.");
      tft.setCursor(40,140);
      tft.println("Please upload file via Arduino IDE. ");
      tft.setCursor(40,160);
      tft.println("  System halted and catching fire.  ");
      while(1){
        yield();  // Do nothing and let the watchdog sleep...
      };
  }

  // Cool. Animation is loaded. Let's go...
  Serial.println("asciimation.txt is ready for playback.");

  tft.setFreeFont(&Picopixel);
  tft.setTextSize(1);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);  // Set Background to be overwritten with Black
}

void loop(void) {
  frame=0;
  // Run Animation
  while ( ani.available() ){
    line=ani.readStringUntil('\n');      // 1. Line of frame contains the delay
    wait=line.toInt();
    for (int i=1; i<=13; i++){
      // Read line, format  and print it
      line = ani.readStringUntil('\n');
      tft.setCursor(0,77+(i*7));
      sprintf(padded, "      %-68s", line.c_str());
      tft.println(padded);
    }
    frame++;
    delay(DELAY*wait);                   // delay between frames 
  }
  // Restart Animation
  ani.seek(0, SeekSet);
  Serial.println("Restart animation.");
}
