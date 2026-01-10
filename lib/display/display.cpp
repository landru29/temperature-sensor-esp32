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
    TwoWire myWire(0);
    this->myWire = new TwoWire(0);
    this->myWire->begin(5, 4);  // Configure SDA=5, SCL=4
    this->screen = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, this->myWire, OLED_RESET);

    if(!this->screen->begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("Could not initialize SSD1306"));
        while(1);
    }

    this->screen->clearDisplay();
    this->screen->setTextColor(SSD1306_WHITE);
    this->screen->setTextSize(1);
    this->screen->setCursor(0, 0);
    this->screen->clearDisplay();
    this->screen->display();
}

void Display::setCursor(int16_t x, int16_t y) {
    if (this->screen == nullptr) {
        return;
    }
    this->screen->setCursor(x, y);
}

size_t Display::write(const uint8_t *buffer, size_t size) {
    if (this->screen == nullptr) {
        return 0;
    }
    this->screen->write(buffer, size);
    this->screen->display();
    return size;
}


size_t Display::write(uint8_t character) {
    if (this->screen == nullptr) {
        return 0;
    }
    this->screen->write(character);
    return 1;
}

void Display::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color, bool render) {
    if (this->screen == nullptr) {
        return;
    }

    this->screen->fillRect(x, y, w, h, color);

    if (render) {
        this->screen->display();
    }
}


#endif