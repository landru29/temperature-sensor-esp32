#ifndef TESTING

#include <Arduino.h>
#include <WiFi.h>
#include "wifi.hpp"
#include "web.hpp"
#include "command.hpp"
#include "sensor.hpp"
#include "manager.hpp"


#define HOSTNAME "temperature"

String inputString = "";
bool stringComplete = false;
HttpServer *requestHandler;
Sensor *sensor;
CommandManager *manager;


void setup() {
  Serial.begin(9600);

  sensor = new Sensor(sensorGetWireNum(), sensorGetType());
  requestHandler = new HttpServer(sensor);

  inputString.reserve(200);

  if (connectWifi(HOSTNAME)) {
    requestHandler->launchServer(HOSTNAME);
  }

  manager = new CommandManager([](const char* data){
    Serial.write(data);
  });

  manager->on("restart", "Restart the system", [](Command *cmd){
      ESP.restart();
  });

  manager->on("wifi-scan", "Scan available wifi networks", [](Command *cmd){
      requestHandler->stopServer();
      currentWifi();
      listWifi();
  });

  manager->on("wifi-connect", "Connect to the wifi", [](Command *cmd){
      currentWifi();
      if (connectWifi(HOSTNAME)) {
        requestHandler->launchServer(HOSTNAME);
      }
  });

  manager->on("wifi-conf", "Configure wifi (<SSID> <password>)", [](Command *cmd){
      if (cmd->argumentCount()!=2) {
        Serial.println("ERROR: wifi-conf takes 2 arguments");
      }

      configureWifi(cmd->nextArgument(), cmd->nextArgument());
      connectWifi(HOSTNAME);
  });

  manager->on("sensor-conf", "Configure sensor (<wireNum[2, 3, 4]> <type[pt100, pt1000]>)", [](Command *cmd){
      if (cmd->argumentCount()!=2) {
        Serial.println("ERROR: sensor-conf takes 2 arguments");
      }

      requestHandler->setSensor(NULL);
      configureSensor(atoi(cmd->nextArgument()), cmd->nextArgument());
      free(sensor);
      sensor = new Sensor(sensorGetWireNum(), sensorGetType());
      requestHandler->setSensor(sensor);
  });

  manager->on("sensor-status", "Display sensor configuration", [](Command *cmd){
      currentSensor();
      Serial.printf("Measure: %f\n", sensor->readTemperature());
      Serial.printf("Ratio: %f\n", sensor->getRatio());
      Serial.printf("Resistance: %f\n", sensor->getResistance());
  });

  manager->displayHelp();
}

void loop() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar;

    // Si on reçoit un retour à la ligne, la commande est complète
    if (inChar == '\n') {
      stringComplete = true;
    }
  }

  // Traiter la commande si elle est complète
  if (stringComplete) {
    Command cmd(inputString.c_str());

    manager->setCommand(&cmd);

    // Réinitialiser pour la prochaine commande
    inputString = "";
    stringComplete = false;
  }

  requestHandler->handleClient();
}
#else
int main(){
    return 0;
}
#endif