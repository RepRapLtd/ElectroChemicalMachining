

// Compile with Heltek WiFi Kit 32

#include <U8g2lib.h>  

char chBuffer[128];
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, 16, 15, 4);         // OLED graphics

const int aPin = A0;

void setup()
{
  pinMode(aPin, INPUT);
  
  // Serial.
  
  Serial.begin(115200);
  while(!Serial)
  {
    Serial.print('.');
  }

  // OLED graphics.
  
  u8g2.begin();
  u8g2.setFont(u8g2_font_6x10_tr);
  u8g2.setFontRefHeightExtendedText();
  u8g2.setDrawColor(1);
  u8g2.setFontPosTop();
  u8g2.setFontDirection(0);

  u8g2.clearBuffer();
  sprintf(chBuffer, "%s", "Starting up...");
  u8g2.drawStr(64 - (u8g2.getStrWidth(chBuffer) / 2), 0, chBuffer);
  u8g2.sendBuffer();
  delay(1000);
}

void loop()
{
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_crox5h_tf);
  int mv = map (analogRead(aPin), 0, 3000, 150, 2450);
  sprintf(chBuffer, "%d mV", mv);
  Serial.println(chBuffer);
  u8g2.drawStr(10, 10, chBuffer);
  u8g2.sendBuffer();
  delay(1000);
}
