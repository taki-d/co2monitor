#include "Arduino.h"
#include "M5Stack.h"

#include "mh_z19c.h"

HardwareSerial serial(0);
mh_z19c co2sensor(2);
int co2_ppm;
int prev_ppm_pos = 0;
const int display_width = 320;
const int display_hegiht = 240;
const int co2_graph_height = 210;
const int co2_ppm_max = 2000;
const int co2_ppm_min = 0;
const int co2_ppm_warning = 1500;
const int co2_ppm_caution = 1000;

TFT_eSprite co2_value = TFT_eSprite(&M5.Lcd);
TFT_eSprite co2_graph = TFT_eSprite(&M5.Lcd);

uint16_t getColor(uint8_t red, uint8_t green, uint8_t blue)
{
  return ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3);
}

int getPosition(int pos) {
  return  co2_graph_height - (int)((float)(co2_graph_height) / (co2_ppm_max - co2_ppm_min) * (pos - co2_ppm_min));
}

void updateDisplay()
{
  co2_value.setCursor(0, 0);
  co2_value.setTextSize(3);
  co2_value.fillSprite(TFT_BLACK);

  if (co2_ppm < 1000)
  {
    co2_value.setTextColor(TFT_WHITE);
  }
  else if (co2_ppm < 1500)
  {
    co2_value.setTextColor(TFT_ORANGE);
  }
  else
  {
    co2_value.setTextColor(TFT_RED);
  }

  co2_value.printf("CO2: %4d ppm", co2_ppm);

  co2_graph.drawFastVLine(display_width - 1, 0, getPosition(co2_ppm_caution) + 1, getColor(50, 50, 0));
  co2_graph.drawFastVLine(display_width - 1, 0, getPosition(co2_ppm_warning) + 1, getColor(50, 0, 0));

  int ppm_pos = getPosition(co2_ppm) - 1;

  if (prev_ppm_pos == 0)
  {
    prev_ppm_pos = ppm_pos;
  }
  

  if(prev_ppm_pos < ppm_pos) {
    co2_graph.drawFastVLine(display_width - 1, ppm_pos, ppm_pos - prev_ppm_pos + 1, TFT_WHITE);
  } else {
    co2_graph.drawFastVLine(display_width - 1, prev_ppm_pos, prev_ppm_pos - ppm_pos + 1, TFT_WHITE);
  }
  prev_ppm_pos = ppm_pos;

  co2_value.pushSprite(0, 0);
  co2_graph.pushSprite(0, display_hegiht - co2_graph_height);

  co2_graph.scroll(-1, 0);
}

void setup()
{
  M5.begin();
  M5.Power.begin();

  M5.Speaker.begin();
  M5.Speaker.mute();

  M5.Lcd.begin();
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
  M5.Lcd.setTextSize(2);

  serial.begin(115200);
  
  co2_value.setColorDepth(8);
  co2_value.createSprite(display_width, display_hegiht - co2_graph_height);
  co2_value.fillSprite(TFT_BLACK);

  co2_graph.setColorDepth(8);
  co2_graph.createSprite(display_width, co2_graph_height);
  co2_graph.fillSprite(TFT_BLACK);
  co2_graph.fillRect(0, 0, display_width, getPosition(co2_ppm_caution) + 1,  getColor(50, 50, 0));
  co2_graph.fillRect(0, 0, display_width, getPosition(co2_ppm_warning) + 1,  getColor(50, 0, 0));

  co2_graph.pushSprite(0, display_hegiht - co2_graph_height);

  
}

void loop()
{
  delay(1000);
  // serial.println(co2sensor.readPPM());
  co2_ppm = co2sensor.readPPM();
  // serial.println(co2_ppm);
  // serial.println(getPosition(co2_ppm));
  updateDisplay();
}
