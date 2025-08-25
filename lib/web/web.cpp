#ifndef TESTING
#include "web.hpp"
#include "html.hpp"
#include <Arduino.h>
#include <WiFi.h>
#include "sensor.hpp"


HttpServer::HttpServer(Sensor *sensor) {
    this->server = new WebServer(80);
    this->sensor = sensor;
}

void HttpServer::launchServer() {
  WebServer *server = this->server;
  float temperature = this->sensor->readTemperature();

  server->on("/", [server]() {
      Serial.println("Page is requested");
      String htmlPage = String((const char*)html).substring(0, html_len);
      htmlPage.replace("%IP%", WiFi.localIP().toString());
      server->send(200, "text/html", htmlPage);
  });

  server->on("/temperature", [server, temperature]() {
      char jsonOut[200] = "{\"temperature\": ";
      char buffer[10];
      dtostrf(temperature, 10, 2, buffer);
      strcat(jsonOut, buffer);
      strcat(jsonOut, "}");
      server->send(200, "application/json", jsonOut);
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