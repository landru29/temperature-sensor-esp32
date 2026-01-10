#ifndef TESTING
#include "chonometer.hpp"
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Preferences.h>

void Chronometer::setEnabled(float temperature) {
  this->enabled = this->enabled || (temperature >= this->threshold);
}

float Chronometer::getThreshold() {
  return this->threshold;
}

void Chronometer::setThreshold(float threshold) {
  this->threshold = threshold;

  Preferences preferences;

  preferences.begin("chronometer", false);
  preferences.putFloat("threshold", threshold);
  preferences.end();
}

void Chronometer::reset() {
  this->elapsedSeconds = 0;
}

void Chronometer::tick() {
  if (this->enabled) {
    this->elapsedSeconds++;
  }
}

Chronometer::Chronometer(Display *display) {
  this->elapsedSeconds = 0;
  this->enabled = false;
  this->displayer = display;
  Preferences preferences;
  preferences.begin("chronometer", true);
  this->threshold = preferences.getFloat("threshold", 200.0);
  preferences.end();
}

void Chronometer::display() {
  this->displayer->setCursor(0, 40);

  this->displayer->fillRect(0, 36, 127, 16, SSD1306_BLACK, false);

  uint32_t hours = elapsedSeconds / 3600;
  uint32_t minutes = (elapsedSeconds % 3600) / 60;
  uint32_t seconds = elapsedSeconds % 60;

  displayer->printf("%02u:%02u:%02u", hours, minutes, seconds);
}




#endif