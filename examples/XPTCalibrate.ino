/*
 * Copyright (c) 2017 razorlt, Spiros Papadimitriou
 *
 * This file is part of github.com/spapadim/XPT2046 and is released
 * under the MIT License: https://opensource.org/licenses/MIT
 *
 * This software is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied.
 */
 //ILI9341_TFT 
 // A2 : SS(Slave Select)
 // A3 : SCK(Serial Clock)
 // A4 : MISO(Master In Slave Out)
 // A5 : MOSI(Master Out Slave In)
 // The other pins are: cs - Chip select (aka slave select), dc - D/C or A0 on the screen (Command/Data switch), rst - Reset
 
 #include "application.h"
 #include <Adafruit_mfGFX.h>
 #include "Adafruit_ILI9341.h"
 #include "XPT2046.h"
 
 Adafruit_ILI9341 tft = Adafruit_ILI9341(/*cs=*/A2, /*dc=*/A1, /*rst=*/A0);
 XPT2046 touch(/*cs=*/ D2, /*irq=*/ D7);
 
 static void calibratePoint(uint16_t x, uint16_t y, uint16_t &vi, uint16_t &vj) {
  // Draw cross
  tft.drawFastHLine(x - 8, y, 16, ILI9341_WHITE);
  tft.drawFastVLine(x, y - 8, 16, ILI9341_WHITE);
  while (!touch.isTouching()) {
    delay(10);
  }
  touch.getRaw(vi, vj);
  // Erase by overwriting with black
  tft.drawFastHLine(x - 8, y, 16, ILI9341_BLACK);
  tft.drawFastVLine(x, y - 8, 16, ILI9341_BLACK);
}

void calibrate() {
  uint16_t x1, y1, x2, y2;
  uint16_t vi1, vj1, vi2, vj2;
  touch.getCalibrationPoints(x1, y1, x2, y2);
  calibratePoint(x1, y1, vi1, vj1);
  delay(1000);
  calibratePoint(x2, y2, vi2, vj2);
  touch.setCalibration(vi1, vj1, vi2, vj2);

  char buf[80];
  snprintf(buf, sizeof(buf), "%d,%d,%d,%d", (int)vi1, (int)vj1, (int)vi2, (int)vj2);
  //ucg.setFont(ucg_font_helvR14_tr);
  //ucg.setColor(0xff, 0xff, 0xff);
  tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
  //ucg.setPrintPos(0, 25);
  tft.setCursor(160, 160);
  tft.print("setCalibration params:");
  tft.setCursor(0, 50);
  tft.print(buf);
}
void setup() {
  delay(300);
  tft.begin();
  delay(10);
  tft.setRotation(0);
  tft.fillScreen(ILI9341_BLACK);
  delay(100);
  touch.begin(ILI9341_TFTWIDTH, ILI9341_HEIGHT);
  touch.setRotation(touch.ROT0);

  calibrate();
}

void loop() {
  // Do nothing
  delay(1000);
}
