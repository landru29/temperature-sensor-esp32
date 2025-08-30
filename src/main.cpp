#ifndef TESTING

#include <Arduino.h>
#include <WiFi.h>
#include "wifi.hpp"
#include "web.hpp"
#include "command.hpp"
#include "sensor.hpp"
#include "manager.hpp"
#include "display.hpp"

String inputString = "";
bool stringComplete = false;
HttpServer *requestHandler;
Sensor *sensor;
CommandManager *manager;
WifiManager *wifiManager;
Display *display;
hw_timer_t *timer = NULL;

void IRAM_ATTR time_action();
void timer_init();
void manager_init();

void setup() {
  Serial.begin(9600);

  sensor = new Sensor(sensorGetWireNum(), sensorGetType());
  requestHandler = new HttpServer(sensor);
  wifiManager = new WifiManager();
  display = new Display();

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
}

void timer_init() {
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &time_action, true);
  timerAlarmWrite(timer, 1000000, true);
  timerAlarmEnable(timer);
}

void IRAM_ATTR time_action() {
  display->printf("%0.1f", sensor->readTemperature());
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
    sensor = new Sensor(sensorGetWireNum(), sensorGetType());
    requestHandler->setSensor(sensor);
  });

  manager->on("sensor-status", "Display sensor configuration", [](Command *cmd) {
    currentSensor();
    Serial.printf("Measure: %f\n", sensor->readTemperature());
    Serial.printf("Ratio: %f\n", sensor->getRatio());
    Serial.printf("Resistance: %f\n", sensor->getResistance());
  });

  manager->displayHelp();
}

#else
int main()
{
  return 0;
}
#endif