#ifndef __wifi_h__
#include <Arduino.h>


class WifiManager {
    public:
        WifiManager();
        void configureWifi(String ssid, String password);
        bool connectWifi();
        void currentWifi();
        const char* hostname();
        void listWifi();
        void setHostName(const char* hostname);

    private:
        const char* _hostname;
};

#define __wifi_h__
#endif