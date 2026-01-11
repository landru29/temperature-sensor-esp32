#ifndef TESTING

#include <Arduino.h>
#include <WiFi.h>
#include "wifi.hpp"
#include "web.hpp"
#include "command.hpp"
#include "sensor.hpp"
#include "manager.hpp"
#include "display.hpp"
#include "chonometer.hpp"

String inputString = "";
bool stringComplete = false;
HttpServer *requestHandler;
Sensor *sensor = NULL;
CommandManager *manager;
WifiManager *wifiManager;
Display *display;
hw_timer_t *timer = NULL;
volatile bool timerFlag = false;
float lastTemperature = 0.0;
Chronometer *chronometer;

void IRAM_ATTR time_action();
void timer_init();
void manager_init();
void displayTemparature();

void setup() {
  Serial.begin(9600);
  Serial.println("Temperature Sensor ESP32");

  sensor = new Sensor(sensorGetWireNum(), sensorGetType(), getSensorResistorError());
  requestHandler = new HttpServer(sensor);
  wifiManager = new WifiManager();
  display = new Display();

  display->println("Temperature Sensor");

  chronometer = new Chronometer(display);

  inputString.reserve(200);

  if (wifiManager->connectWifi()) {
    requestHandler->launchServer(wifiManager->hostname());
  }

  timer_init();

  manager_init();
}

void loop() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar;

    if (inChar == '\n')
    {
      stringComplete = true;
    }
  }

  if (stringComplete) {
    Command cmd(inputString.c_str());

    manager->setCommand(&cmd);

    inputString = "";
    stringComplete = false;
  }

  requestHandler->handleClient();

  if (timerFlag) {
    displayTemparature();
    chronometer->display();
  }
}

void timer_init() {
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &time_action, true);
  timerAlarmWrite(timer, 1000000, true);
  timerAlarmEnable(timer);
}

void IRAM_ATTR time_action() {
   timerFlag = true;
   chronometer->tick();
}

void displayTemparature() {
  display->setCursor(0, 20);

  display->fillRect(0, 16, 127, 16, SSD1306_BLACK, false);

  float currentTemp = sensor->readTemperature();
  chronometer->setEnabled(currentTemp);
  
  bool sign = (currentTemp - lastTemperature) >= 0;
  float diff = sign ? (currentTemp - lastTemperature) : (lastTemperature - currentTemp);

  char arrow = sign ? 0x18 : 0x19;

  if (sensor == NULL) {
    display->println("No sensor");
  } else {
    display->printf("%3.1f%c   %c %1.1f %c", currentTemp, 0xf7, arrow, diff, arrow);
  }

  lastTemperature = currentTemp;

  timerFlag = false;
}


void manager_init() {
  manager = new CommandManager(&Serial);

  manager->on("restart", "Restart the system", [](Command *cmd) {
    ESP.restart();
  });

  manager->on("wifi-scan", "Scan available wifi networks", [](Command *cmd) {
    requestHandler->stopServer();
    wifiManager->currentWifi();
    wifiManager->listWifi();
  });

  manager->on("wifi-connect", "Connect to the wifi", [](Command *cmd) {
    wifiManager->currentWifi();
    if (wifiManager->connectWifi()) {
      requestHandler->launchServer(wifiManager->hostname());
    }
  });

  manager->on("wifi-conf", "Configure wifi (<SSID> <password>)", [](Command *cmd) {
    if (cmd->argumentCount()!=2) {
      Serial.println("ERROR: wifi-conf takes 2 arguments");
      Serial.println("Current configuration:");
      wifiManager->currentWifi();
      return;
    }

    wifiManager->configureWifi(cmd->nextArgument(), cmd->nextArgument());
    wifiManager->connectWifi();
  });

  manager->on("chrono-threshold", "Set temperature to start the chronometer", [](Command *cmd) {
    if (cmd->argumentCount()!=1) {
      Serial.println("ERROR: chrono-threshold takes 1 argument");
      Serial.printf("current threshold: %.1f°c\n", chronometer->getThreshold());
      return;
    }

    float threshold = atof(cmd->nextArgument());
    chronometer->setThreshold(threshold);
    Serial.printf("New chronometer threshold : %.1f°c\n", threshold);
  });

  manager->on("Hostname", "Configure hostname (<hostname>)", [](Command *cmd) {
    if (cmd->argumentCount()!=1) {
      Serial.println("ERROR: Hostname takes 1 argument");
      Serial.printf("current hostname: %s\n", wifiManager->hostname());
      return;
    }

    wifiManager->setHostName(cmd->nextArgument());

    Serial.printf("New hostname : %s\nYou must restart the device\n", wifiManager->hostname());
  });

  manager->on("sensor-conf", "Configure sensor (<wireNum[2, 3, 4]> <type[pt100, pt1000]>)", [](Command *cmd) {
    if (cmd->argumentCount()!=2) {
      Serial.println("ERROR: sensor-conf takes 2 arguments");
    }

    requestHandler->setSensor(NULL);
    configureSensor(atoi(cmd->nextArgument()), cmd->nextArgument());
    free(sensor);
    sensor = new Sensor(sensorGetWireNum(), sensorGetType(), getSensorResistorError());
    requestHandler->setSensor(sensor);
  });

  manager->on("sensor-ref", "Modify reference resistance", [](Command *cmd) {
    if (cmd->argumentCount()!=1) {
      Serial.println("ERROR: sensor-ref takes 1 argument");
      Serial.printf("current resistor error: %.3f ohm\n", getSensorResistorError());
      return;
    }

    float error = atof(cmd->nextArgument());
    adjustSensorResistor(error);
    free(sensor);
    sensor = new Sensor(sensorGetWireNum(), sensorGetType(), getSensorResistorError());
    requestHandler->setSensor(sensor);
    Serial.printf("New resistor error : %.3f ohm\n", getSensorResistorError());
    Serial.printf("Ref Resistance: %f ohm\n", sensor->getRefResistance());
    Serial.printf("Measure: %f\n", sensor->readTemperature());
  });

  manager->on("sensor-status", "Display sensor configuration", [](Command *cmd) {
    currentSensor();
    Serial.printf("Measure: %f\n", sensor->readTemperature());
    Serial.printf("Ratio: %f\n", sensor->getRatio());
    Serial.printf("Resistance: %f\n", sensor->getResistance());
    Serial.printf("Ref Resistance: %f ohm\n", sensor->getRefResistance());
  });

  manager->displayHelp();
}

#else
int main()
{
  return 0;
}
#endif