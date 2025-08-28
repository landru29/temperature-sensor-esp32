#ifndef TESTING
#include "web.hpp"
#include "html.hpp"
#include "favicon.hpp"
#include <Arduino.h>
#include "sensor.hpp"


HttpServer::HttpServer(Sensor *sensor) {
    this->server = new WebServer(80);
    this->setSensor(sensor);
}

void HttpServer::setSensor(Sensor *sensor) {
    this->sensor = sensor;
}

void HttpServer::launchServer(const char *hostname) {
  WebServer *server = this->server;
  HttpServer* self = this;
  
  server->on("/", [server, hostname]() {
      Serial.println("Page is requested");
      String htmlPage = String((const char*)html);
      htmlPage.replace("%IP%", hostname);
      server->send(200, "text/html", htmlPage);
  });

  server->on("/temperature", [server, self]() {
      Sensor *sensor = self->sensor;
      float temperature = 0;
      if (sensor != NULL) {
        temperature = sensor->readTemperature();
      }
      
      char jsonOut[200] = "{\"temperature\": ";
      char buffer[15];
      dtostrf(temperature, 10, 2, buffer);
      strcat(jsonOut, buffer);
      strcat(jsonOut, "}");
      server->send(200, "application/json", jsonOut);
  });

  server->on("/favicon.ico", [server, hostname]() {
      server->sendHeader("Content-Type", "image/x-icon");
      server->sendHeader("Content-Length", String(favicon_len));
      server->send_P(200, "image/x-icon", (const char*)favicon, favicon_len);
  });

  Serial.println("Launching server on port 80");

  this->server->begin();
}

void HttpServer::stopServer() {
    Serial.println("Stopping server");

    this->server->stop();
}

void HttpServer::handleClient() {
    this->server->handleClient();
}

#endif