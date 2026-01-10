#include "sensor.hpp"


void configureSensor(int wireCount, char* sensorType) {
#ifndef TESTING 
    Serial.print("Configuring sensor with ");
    Serial.print(wireCount);
    Serial.print(" and type ");

    if (strcasecmp(sensorType, "PT100") == 0) {
        Serial.println("PT100");
        sensorType=(char*)"PT100";
    } else if (strcasecmp(sensorType, "PT1000") == 0) {
        Serial.println("PT1000");
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
#endif
}

sensor_numwires_t sensorGetWireNum() {
#ifndef TESTING 
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
#else
    return SENSOR_4WIRE;
#endif
}

void currentSensor() {
#ifndef TESTING 
    Preferences preferences;

    preferences.begin("sensor", true);

    if (!preferences.isKey("wireNum")) {
        Serial.println("No wire num configured. Default 4");
    } else {
        Serial.print("Wire num: ");
        Serial.println(preferences.getString("wireNum"));
    }

    if (!preferences.isKey("type")) {
        Serial.println("No type configured. Default PT100");
    } else {
        Serial.print("Type: ");
        Serial.println(preferences.getString("type"));
    }

    preferences.end();
#endif
}

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

Sensor::Sensor(sensor_numwires_t wireCount, sensor_type typeOfSensor) {
}

float Sensor::readTemperature() {
    return 24.0;
}

#else

#include <Arduino.h>

Sensor::Sensor(sensor_numwires_t wireCount, sensor_type typeOfSensor) {
    this->max = new Adafruit_MAX31865(MAX_CS, MAX_MOSI, MAX_MISO, MAX_CLK);
    this->resistorRef = 430.0;
    this->resistorNominal = 100.0;
    this->currentRatio = 0;
    this->currentResistance = 0;
    this->currentTemperature = 0;

    if (typeOfSensor == SENSOR_PT1000) {
        this->resistorRef *= 10;
        this->resistorNominal *= 10;
    }

    max31865_numwires_t maxNumWire = MAX31865_2WIRE;
    switch (wireCount) {
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

    if (!this->max->begin(maxNumWire)) {
        Serial.println("Error: could not establish communication with the sensor.");

        while(1);
    }
}

float Sensor::readTemperature() {
    uint16_t rtd = this->max->readRTD();
    float ratio = rtd;

    Serial.print("RTD value: "); Serial.println(rtd);
    Serial.print("Ratio: "); Serial.println(ratio / 32768);
    Serial.print("Resistance: "); Serial.println(this->resistorRef * ratio);
    Serial.print("Temperature: "); Serial.println(this->max->temperature(this->resistorNominal, this->resistorRef));

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