# AsciiWars
**Simple Star Wars Asciimation Player for the ESP8266 + TFT Display**

![Breadboard](https://github.com/gotohellmann/AsciiWars/blob/main/images/image1.jpg) ![Projector](https://github.com/gotohellmann/AsciiWars/blob/main/images/image2.jpg) 


Code by        **F. Hellmann**  <www.gotohellmann.com>  
Animation by   **S. Jansen**    <www.asciimation.co.nz>  
Font by        **S. Weber**     <www.blogfonts.com/picopixel.font>  
 
 This sketch is playing back Simon Jansen's brilliant ASCII animation of 
 Star Wars on a tiny 2.2" LCD Screen. 

 ### Versions:
 
 - _AsciiWars_v04_  works with the standard TFT_eSPI library, but flickers a bit
 - _AsciiWars_v04_patched_  needs a patched TFT_eSPI library to reduce flicker, see [below](#flickering)
 
 
 ### Platform & Hardware needed:
 
 - This project needs the [Arduino IDE 1.8.10+](https://www.arduino.cc/en/software)
 - a **WEMOS D1 mini** Module (4MB Flash)
 - an **ILI9341** 240x320 Pixel RGB TFT LCD Display
 - and a few wires to connect everything up
 
 
 ### Usage:     
 
 - Connect LCD to Wemos D1 mini
   -> see connection infos in asciiwars.ino file or [below](#connections)
 - Start Arduino IDE
 - check you have the ESP8266FS tool under Tools
  -> if not: https://github.com/esp8266/arduino-esp8266fs-plugin
    
 - check that you have the necessary libs installed:
   - TFT_eSPI
      -> Don't forget to edit Library User_Settings.h
                 
 - Uncomment SPIFFS Formatting Block below
 - Arduino Board Select: Wemos D1 mini
 - Change to Flash Size: 4M (3M SPIFFS)
 - Open Serial Monitor and then Upload Sketch
 - Check for SPIFFS formatting progress
 - Comment out the SPIFFS Formatting Block
 
 - Close Serial Monitor 
 - Upload Data with Tools -> ESP8266FS tool
 - Upload Sketch _without_ active SPIFFS Formatting Block :smirk:
      
 **-> Have fun.**
 
 
 ### Connections:
 
 Connect the following Wemos D1 Mini pins to the ILI9341 LCD:

 Wemos D1  |  LCD
-----------|------------
 +3V3 VCC  |  VCC
 GND       |  GND
 D8        |  CS
 +3V3 VCC  |  RST
 D3        |  DC / RS
 D7        |  SDI (MOSI)
 D5        |  SCK 
 +3V3 VCC  |  LED
 D6        |  SDO (MISO)


 ### Flickering:
 
 Unfortunatly, clearing/redrawing the screen takes up a lot of time and can lead to visible flicker.
 To reduce this effect we can apply a patch that only clears the next character to be drawn just in time.

 - To reduce flicker -> patch TFT_eSPI Library (Line 2780):
 
 
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


    - ( This patched TFT_eSPI Lib is available on my github as well: [TFT_eSPI_patched.zip](https://github.com/gotohellmann/AsciiWars/blob/main/TFT_eSPI_patched.zip) )
            
