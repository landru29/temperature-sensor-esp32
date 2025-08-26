#ifndef __WEB_HPP__
#define __WEB_HPP__

#include <WebServer.h>
#include "sensor.hpp"

class HttpServer {
    public:
        HttpServer(Sensor *sensor);
        void launchServer(const char *hostname);
        void stopServer();
        void handleClient();
    
    private:
        WebServer *server;
        Sensor *sensor;
};

#endif