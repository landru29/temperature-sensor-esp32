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

void configureSensor(int wireCount, char* sensorType);
sensor_numwires_t sensorGetWireNum();
sensor_type_t sensorGetType();
void currentSensor();
void adjustSensorResistor(float error);
float getSensorResistorError();


class Sensor {
    public:
        Sensor(sensor_numwires_t wireCount, sensor_type typeOfSensor, float resistorError=0.0);
        float readTemperature();
        float getRatio();
        float getResistance();
        float getRefResistance();
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