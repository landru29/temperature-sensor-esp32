#include "sensor.hpp"

sensor_type_t sensorGetType() {
#ifndef TESTING 
    Preferences preferences;
    preferences.begin("sensor", true);
    String sensorTypeStr = preferences.getString("type", "");
    preferences.end();

    if (sensorTypeStr = "PT100") {
        return SENSOR_PT100;
    } else if (sensorTypeStr = "PT1000") {
        return SENSOR_PT1000;
    }

    return SENSOR_PT100;
#else
    return SENSOR_PT100;
#endif
}


#ifdef TESTING

Sensor::Sensor() {
}

float Sensor::readTemperature() {
    return 24.0;
}

#else

#include <Arduino.h>

Sensor::Sensor() {
    this->reloadConfig();
}

void Sensor::reloadConfig() {
    this->resistorNominal = 100.0;
    this->currentRatio = 0;
    this->currentResistance = 0;
    this->currentTemperature = 0;

    max31865_numwires_t maxNumWire = MAX31865_2WIRE;
    switch (this->sensorGetWireNum()) {
        case SENSOR_2WIRE:
            maxNumWire = MAX31865_2WIRE;
            break;
        case SENSOR_3WIRE:
            maxNumWire = MAX31865_3WIRE;
            break;
        case SENSOR_4WIRE:
            maxNumWire = MAX31865_4WIRE;
            break;
    }

    this->resistorRef = 430.0;

    if (this->sensorGetType() == SENSOR_PT1000) {
        this->resistorRef *= 10;
        this->resistorNominal *= 10;
    }

    this->resistorRef += this->getResistorError();

    this->max = new Adafruit_MAX31865(MAX_CS, MAX_MOSI, MAX_MISO, MAX_CLK);
    
    if (!this->max->begin(maxNumWire)) {
        Serial.println("Error: could not establish communication with the sensor.");
    }
}

void Sensor::adjustSensorResistor(float error) {
    Preferences preferences;
    preferences.begin("sensor", false);
    preferences.putFloat("resistorError", error);
    preferences.end();

    this->reloadConfig();
}

float Sensor::getResistorError() {
    Preferences preferences;
    preferences.begin("sensor", true);
    float error = preferences.getFloat("resistorError", 0.0);
    preferences.end();

    return error;
}

sensor_numwires_t Sensor::sensorGetWireNum() {
    Preferences preferences;

    preferences.begin("sensor", true);
    String wireNumStr = preferences.getString("wireNum", "");
    preferences.end();

    if (wireNumStr = "2") {
        return SENSOR_2WIRE;
    } else if (wireNumStr = "3") {
        return SENSOR_3WIRE;
    } else if (wireNumStr = "4") {
        return SENSOR_4WIRE;
    }
    
    return SENSOR_4WIRE;
}

sensor_type_t Sensor::sensorGetType() {
    Preferences preferences;
    preferences.begin("sensor", true);
    String sensorTypeStr = preferences.getString("type", "");
    preferences.end();

    if (sensorTypeStr = "PT100") {
        return SENSOR_PT100;
    } else if (sensorTypeStr = "PT1000") {
        return SENSOR_PT1000;
    }

    return SENSOR_PT100;
}

void Sensor::configureSensor(int wireCount, char* sensorType) {
    if (strcasecmp(sensorType, "PT100") == 0) {
        sensorType=(char*)"PT100";
    } else if (strcasecmp(sensorType, "PT1000") == 0) {
        sensorType=(char*)"PT1000";
    } else {
        Serial.println("unknown");
    }

    char wireCountStr[16];
    itoa(wireCount, wireCountStr, 10);   

    Preferences preferences;

    preferences.begin("sensor", false);

    preferences.putString("wireNum", wireCountStr);
    preferences.putString("type", sensorType);

    preferences.end();

    this->reloadConfig();
}


float Sensor::getRefResistance() {
    return this->resistorRef;
}   

float Sensor::readTemperature() {
    uint16_t rtd = this->max->readRTD();
    float ratio = rtd;

    this->currentRatio = ratio / 32768;
    this->currentResistance = this->resistorRef * ratio;

    this->currentTemperature = this->max->temperature(this->resistorNominal, this->resistorRef);

    return this->currentTemperature;
}

float Sensor::getRatio() {
    return this->currentRatio;
}

float Sensor::getResistance() {
    return this->currentResistance;
}


#endif