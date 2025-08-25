#ifndef TESTING

#include <Arduino.h>
#include <WiFi.h>
#include "wifi.hpp"
#include "web.hpp"
#include "command.hpp"
#include "sensor.hpp"

String inputString = "";
bool stringComplete = false;
HttpServer *requestHandler;
Sensor *sensor;


void setup() {
  Serial.begin(9600);

  sensor = new Sensor();
  requestHandler = new HttpServer(sensor);

  inputString.reserve(200);

  if (connectWifi()) {
    requestHandler->launchServer();
  }

  Serial.println("Type 'help' for the full command list.");
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

    if (cmd.getCommand() == "help") {
      Serial.println("Available commands :");
      Serial.println("- help :                        Display this help");
      Serial.println("- wifi-scan :                   Scan available wifi networks");
      Serial.println("- wifi-connect :                Connect to the wifi");
      Serial.println("- wifi-conf <SSID> <password> : Configure wifi");
    } else if (cmd.getCommand() == "wifi-scan") {
      requestHandler->stopServer();
      currentWifi();
      listWifi();
    } else if (cmd.getCommand() == "wifi-connect") {
      currentWifi();
      if (connectWifi()) {
        requestHandler->launchServer();
      }
    } else if (cmd.getCommand() == "wifi-conf") {
      if (cmd.argumentCount()!=2) {
        Serial.println("ERROR: wifi-conf takes 2 arguments");

        // Réinitialiser pour la prochaine commande
        inputString = "";
        stringComplete = false;
        return;
      }

      configureWifi(cmd.nextArgument(), cmd.nextArgument());
      connectWifi();
    }   else {
      Serial.print("Unknown command : ");
      Serial.println(cmd.getCommand());
      Serial.println("Type 'help' for the full command list.");
    }

    Serial.println("READY");

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