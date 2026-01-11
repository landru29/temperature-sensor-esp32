#ifndef __chonometer_h__
#include <Arduino.h>
#include "display.hpp"


class Chronometer {
    public:
        Chronometer(Display *display);
        void setEnabled(float temperature);
        void reset();
        void tick();
        void display();
        void setThreshold(float threshold);
        float getThreshold();
        
    private:
        Display *displayer;
        uint32_t elapsedSeconds;
        bool enabled;
        float threshold;
};


#define __chronometer_h__
#endif