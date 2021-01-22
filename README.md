# AsciiWars
Simple Star Wars Asciimation Player for the ESP8266 + TFT Display

Code by        F. Hellmann  <www.gotohellmann.com>  
Animation by   S. Jansen    <www.asciimation.co.nz>  
Font by        S. Weber     <https://blogfonts.com/picopixel.font>  
 
 This sketch is playing back an ASCII animation of 
 Star Wars on a tiny 2.2" LCD Screen. 

 Platform: 
 Arduino on WEMOS D1 mini Module (4MB Flash)
 with an ILI9341 240x320 Pixel RGB TFT LCD Display
 
 
 Usage:     
 
 - Connect LCD to Wemos D1 mini
   -> see connection infos in .ino file
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
  https://github.com/gotohellmann/AsciiWars
              
