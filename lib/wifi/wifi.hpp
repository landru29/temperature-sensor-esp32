#ifndef __wifi_h__
#include <Arduino.h>


void listWifi();
void configureWifi(String ssid, String password);
bool connectWifi(const char* hostname);
void currentWifi();

#define __wifi_h__
#endif