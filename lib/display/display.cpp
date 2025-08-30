#ifndef TESTING
#include "display.hpp"
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Display::Display() {
    this->_display = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

    if(!this->_display->begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("Could not initialize SSD1306"));
        while(1);
    }

    this->_display->clearDisplay();
    this->_display->setTextColor(SSD1306_WHITE);
    this->_display->setTextSize(1);
    this->_display->setCursor(0, 0);
}

size_t Display::write(const uint8_t *buffer, size_t size) {
    this->_display->write(buffer, size);
    this->_display->display();
    return size;
}


size_t Display::write(uint8_t character) {
    this->_display->write(character);
    return 1;
}


#endif