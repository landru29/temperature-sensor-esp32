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

  sensor = new Sensor();
  requestHandler = new HttpServer(sensor);

  inputString.reserve(200);

  if (connectWifi(HOSTNAME)) {
    requestHandler->launchServer(HOSTNAME);
  }

  manager = new CommandManager([](const char* data){
    Serial.write(data);
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

        // Réinitialiser pour la prochaine commande
        inputString = "";
        stringComplete = false;
        return;
      }

      configureWifi(cmd->nextArgument(), cmd->nextArgument());
      connectWifi(HOSTNAME);
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