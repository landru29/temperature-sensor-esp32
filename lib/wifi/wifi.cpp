#ifndef TESTING
#include "wifi.hpp"
#include <Arduino.h>
#include <WiFi.h>

void listWifi() {
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

void configureWifi(String ssid, String password) {
    Serial.print("Configuring wifi with ssid ");
    Serial.print(ssid);
    Serial.print(" and password ");
    Serial.println(password);

    Preferences preferences;

    preferences.begin("wifi-creds", false);

    preferences.putString("ssid", ssid);
    preferences.putString("password", password);

    preferences.end();
}

bool connectWifi() {
    Preferences preferences;

    preferences.begin("wifi-creds", false);

    if (!preferences.isKey("ssid")) {
        Serial.println("No wifi configured");

        return false;
    }

    String ssid = preferences.getString("ssid", "");
    String password = preferences.getString("password", "");

    preferences.end();

    Serial.print("Connecting to wifi ");
    Serial.println(ssid);

    WiFi.begin(ssid.c_str(), password.c_str());

    Serial.println("Waiting response from the network ...");

    for(int i = 0; i<20 && WiFi.status() != WL_CONNECTED; i++) {
        delay(500);
        Serial.print(".");
        Serial.flush();
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("");
        Serial.println("Connected to wifi");
        Serial.print("Adresse IP : ");
        Serial.println(WiFi.localIP());
        return true;
    }
    
    Serial.print("Fail to connect to [");
    Serial.print(ssid);
    Serial.println("]");
    return false;
}

void currentWifi() {
    Preferences preferences;

    preferences.begin("wifi-creds", false);

    if (!preferences.isKey("ssid")) {
        Serial.println("No wifi configured");

        return;
    }

    String ssid = preferences.getString("ssid", "");
    String password = preferences.getString("password", "");

    preferences.end();

    Serial.print("SSID: [");
    Serial.print(ssid);
    Serial.println("]");
    Serial.print("password: [");
    Serial.print(password);
    Serial.println("]");
}
#endif