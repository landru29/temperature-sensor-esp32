#include <Arduino.h>
#include <WiFi.h>
#include "wifi.hpp"
#include "web.hpp"

String inputString = "";
bool stringComplete = false;


void setup() {
  Serial.begin(9600);
  inputString.reserve(200);

  if (connectWifi()) {
    launchServer();
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
    // Supprimer les espaces et le retour à la ligne
    inputString.trim();
    String command = inputString;
    String arguments;

    int espace = inputString.indexOf(' ');
    if (espace>0) {
      command = inputString.substring(0, espace);
      arguments = inputString.substring(espace + 1);
    }

    // Exécuter la commande
    if (command == "help") {
      Serial.println("Available commands :");
      Serial.println("- help :                        Display this help");
      Serial.println("- wifi-scan :                   Scan available wifi networks");
      Serial.println("- wifi-connect :                Connect to the wifi");
      Serial.println("- wifi-conf <SSID> <password> : Configure wifi");
    }
    else if (command == "wifi-scan") {
      stopServer();
      currentWifi();
      listWifi();
    }
    else if (command == "wifi-connect") {
      currentWifi();
      if (connectWifi()) {
        launchServer();
      }
    }
    else if (command == "wifi-conf") {
      int espace = arguments.indexOf(' ');
      if (espace<0) {
        Serial.println("ERROR: wifi-conf takes 2 arguments");
      }

      String ssid = arguments.substring(0, espace);
      String password = arguments.substring(espace + 1);

      configureWifi(ssid, password);
      connectWifi();
    }
    else {
      Serial.print("Unknown command : ");
      Serial.println(command);
      Serial.println("Type 'help' for the full command list.");
    }

    Serial.println("READY");

    // Réinitialiser pour la prochaine commande
    inputString = "";
    stringComplete = false;
  }

  server->handleClient();
}

