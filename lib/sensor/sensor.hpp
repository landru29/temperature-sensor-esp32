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
#define MAX_CS   10
#define MAX_MOSI 11
#define MAX_MISO 12
#define MAX_CLK  13

void configureSensor(int wireCount, char* sensorType);
sensor_numwires_t sensorGetWireNum();
sensor_type_t sensorGetType();
void currentSensor();


class Sensor {
    public:
        Sensor(sensor_numwires_t wireCount, sensor_type typeOfSensor);
        float readTemperature();
        float getRatio();
        float getResistance();
    private:
#ifndef TESTING 
        Adafruit_MAX31865 *max;
        float resistorRef;
        float resistorNominal;

        float currentRatio;
        float currentResistance;
        float currentTemperature;
#endif
};

#endif