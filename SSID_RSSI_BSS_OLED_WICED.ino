/*
The MIT License (MIT)

Copyright (c) 2016 John W. Turner

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */
 
 /* THIS WAS TESTED ON the Adafruit WICED board */

#include <adafruit_feather.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSans18pt7b.h>//load some cool fonts
#define OLED_RESET 3 //Pin of the OLED (this code used the OLED Feather wing sheild from Adafruit
Adafruit_SSD1306 display(OLED_RESET);

#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 
#define MAX_SCAN_NUM   20
wl_ap_info_t scan_result[MAX_SCAN_NUM];

String sssid;
uint8_t encryptionType;
int32_t RSSI;
uint8_t* BSSID;
int32_t channel;
bool isHidden; 
uint8_t curBss;
String targetSSID = "linksis54"; // change this to match your networks SSID
uint8_t prevRssi;

void setup() {
Serial.begin(9600);
display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
display.display();
  delay(200);
display.clearDisplay();
}

void loop() {

  byte available_networks = Feather.scanNetworks(scan_result, MAX_SCAN_NUM);
int netnum = 0;
prevRssi = 0;
//      Serial.print("Found networks ");
//      Serial.println (available_networks);
// first lets find the SSID of the network you are looking for
// by iterating through all of the avaialble networks
// since in an enterprise there may be more than one BSSID for the SSID
// Lets find the stringest one
  for (int network = 0; network < available_networks; network++) {
//    Serial.println (scan_result[network].ssid);
    if (String(scan_result[network].ssid)== targetSSID) {
//      Serial.print("Found one ");
//      Serial.println (scan_result[network].rssi);
//      Serial.println (scan_result[network].bssid[5],HEX);
      if ((uint8_t)scan_result[network].rssi > prevRssi) {
      netnum = network;
      prevRssi = (uint8_t)scan_result[network].rssi;
    }
    }
  }


if (scan_result[netnum].bssid[5] != curBss){
  Serial.println("New Bss!");
  curBss = scan_result[netnum].bssid[5];
  Serial.println(curBss,HEX);
  
}
  Serial.print("Signal strength: ");
  int bars;
//  int bars = map(RSSI,-80,-44,1,6); // this method doesn't refelct the Bars well
  // simple if then to set the number of bars
  
  if (scan_result[netnum].rssi > -55) { 
    bars = 5;
  } else if (scan_result[netnum].rssi < -55 & scan_result[netnum].rssi > -65) {
    bars = 4;
  } else if (scan_result[netnum].rssi < -65 & scan_result[netnum].rssi > -70) {
    bars = 3;
  } else if (scan_result[netnum].rssi < -70 & scan_result[netnum].rssi > -78) {
    bars = 2;
  } else if (scan_result[netnum].rssi < -78 & scan_result[netnum].rssi > -82) {
    bars = 1;
  } else {
    bars = 0;
  }

  display.clearDisplay();
// Do some simple loop math to draw rectangles as the bars
// Draw one bar for each "bar" Duh...
  for (int b=0; b <= bars; b++) {
    display.fillRect(59 + (b*5),33 - (b*5),3,b*5,WHITE); 
  }
// This is what all 5 bars would look like if you coded them manually
//  display.fillRect(64,28,3,5,WHITE);
//  display.fillRect(69,23,3,10,WHITE);
//  display.fillRect(74,18,3,15,WHITE);
//  display.fillRect(79,13,3,20,WHITE);
//  display.fillRect(84,8,3,25,WHITE);
display.setTextColor(WHITE);
display.setTextSize(1);
display.setCursor(0,26);
display.setFont(&FreeSans18pt7b); //I like fonts
display.print(scan_result[netnum].rssi);
display.setFont(); // change back to the default font to print the BSSID
display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(0,0);
for(int i=0;i<5;i++){ // loop through the first 5 BSSID values in the arrray
  display.print(scan_result[netnum].bssid[i],HEX);
  display.print(":");
  }
  display.println(scan_result[netnum].bssid[5],HEX); // then print the last BSSID value in the array
display.display();

//print this out on the Serial console for debugging
  Serial.print(scan_result[netnum].rssi);
  Serial.println("dBm");
  Serial.print("BSS: ");
  for(int i=0;i<5;i++){
  Serial.print(scan_result[netnum].bssid[i],HEX);
  Serial.print(":");
  }
Serial.println(scan_result[netnum].bssid[5],HEX);
  
  Serial.print(bars);
  Serial.println(" bars");
}
