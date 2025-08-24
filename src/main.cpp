#include <Arduino.h>
#include <WiFi.h>

String inputString = "";
bool stringComplete = false;


void setup() {
  Serial.begin(9600);
  inputString.reserve(200);
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
      Serial.println("- help :      Display this help");
      Serial.println("- wifi-scan : Scan available wifi networks");
    }
    else if (command == "wifi-scan") {
      Serial.println("Looking for available wifi ...");
      WiFi.mode(WIFI_STA);
      WiFi.disconnect();
      delay(100);

      int n = WiFi.scanNetworks();
      if (n==0) {
        Serial.println("No wifi network found.");
      } else {
        for (int i = 0; i<n; i++) {
          Serial.print(i+1);
          Serial.print(": ");
          Serial.print(WiFi.SSID(i));
          Serial.print(" (");
          Serial.print(WiFi.RSSI(i));
          Serial.println(" dBm)");
          delay(10);
        }
      }

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
}

