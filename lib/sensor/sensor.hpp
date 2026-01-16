#ifndef __SENSOR_HPP__
#define __SENSOR_HPP__

#ifndef TESTING 
#include <Adafruit_MAX31865.h>
#include <Preferences.h>
#endif

typedef enum sensor_numwires {
  SENSOR_2WIRE = 2,
  SENSOR_3WIRE = 3,
  SENSOR_4WIRE = 4
} sensor_numwires_t;

typedef enum sensor_type {
    SENSOR_PT100 = 1,
    SENSOR_PT1000 = 2
} sensor_type_t;

// CS PIN
#define MAX_CS   2
#define MAX_MOSI 13
#define MAX_MISO 14
#define MAX_CLK  15


class Sensor {
    public:
        Sensor();
        float readTemperature();
        float getRatio();
        float getResistance();
        float getRefResistance();
        float getResistorError();
        void adjustSensorResistor(float resistance);
        sensor_numwires_t sensorGetWireNum();
        sensor_type_t sensorGetType();
        void configureSensor(int wireCount, char* sensorType);
    private:
#ifndef TESTING 
        Adafruit_MAX31865 *max;
        float resistorRef;
        float resistorNominal;

        float currentRatio;
        float currentResistance;
        float currentTemperature;
        void reloadConfig();
#endif
};

#endif