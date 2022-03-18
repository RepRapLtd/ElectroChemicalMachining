/*
 * Programmable constant current source.
 * 
 * Adrian Bowyer
 * Reprap Ltd.
 * 
 * https://reprapltd.com
 * 
 * Licence: GPL 3
 * 
 */

// Compile with Heltek WiFi Kit 32


// OLED graphics

#include <U8g2lib.h> 
char chBuffer[128];
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, 16, 15, 4);

const int aInPin = A5;      // Voltage read
const int aOutPin = 25;     // Voltage out
const int upPin = 13;       // Raise current
const int downPin = 12;     // Lower current
const int controlPin = 14;  // On/off
const int minV = 100;       // Lowest effective voltage (100/255)
const float r = 6.3;        // Ohms. Resistor in Darlington emitter follower
const float m = 1.181;      // Empirical current correction...
const float c = -4.5;       // ... Iout = m*Iin + c

int vD = minV;               // Voltage fed into the base of the Darlington n/255
float voltage = 0;          // V and ...
float current = 0;          // ... I now

void setup()
{
  pinMode(aInPin, INPUT);
  pinMode(aOutPin, OUTPUT);
  pinMode(upPin, INPUT_PULLUP);
  pinMode(downPin, INPUT_PULLUP);
  pinMode(controlPin, INPUT);
  
  // OLED graphics.
  
  u8g2.begin();
  u8g2.setFont(u8g2_font_6x10_tr);
  u8g2.setFontRefHeightExtendedText();
  u8g2.setDrawColor(1);
  u8g2.setFontPosTop();
  u8g2.setFontDirection(0);

  u8g2.clearBuffer();
  sprintf(chBuffer, "%s", "Const. I starting...");
  u8g2.drawStr(64 - (u8g2.getStrWidth(chBuffer) / 2), 0, chBuffer);
  u8g2.sendBuffer();
  delay(1000);
}

 
void loop()
{
  // Current on?
  
  if(!digitalRead(controlPin))
  {
    // No
    
    dacWrite(aOutPin, minV);
  }else
  {
    // Yes
    
    if(!digitalRead(downPin))
      vD -= 10;
    if(!digitalRead(upPin))
      vD += 10;
    if(vD < minV)
      vD = minV;
    if(vD > 255)
      vD = 255;
    dacWrite(aOutPin, vD);
  }

  // Measure the current
  
  voltage = (float)map(analogRead(aInPin), 0, 3000, 150, 2450);
  current = voltage/r;
  current = m*current + c;

  // Display it
  
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_crox5h_tf);  
  sprintf(chBuffer, "%.2f mA", current); 
  u8g2.drawStr(10, 10, chBuffer);
  u8g2.sendBuffer();
  
  delay(1000);
}
