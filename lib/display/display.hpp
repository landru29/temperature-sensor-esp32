#ifndef __display_h__
#include <Arduino.h>
#include <Adafruit_SSD1306.h>


class Display: public Print {
    public:
        Display();
        virtual size_t write(const uint8_t *buffer, size_t size) override;
        virtual size_t write(uint8_t character) override ;
        void setCursor(int16_t x, int16_t y);
        
    private:
        TwoWire *myWire;
        Adafruit_SSD1306 *screen;
};

#define __display_h__
#endif