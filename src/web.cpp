#include "web.hpp"
#include "html.hpp"

WebServer *server = new WebServer(80);


String processor(const String& var) {
  if (var == "IP") {
    return WiFi.localIP().toString();
  }
  return String();
}


void launchServer() {
    server->on("/", []() {
        Serial.println("Page is requested");
        String htmlPage = String((const char*)html).substring(0, html_len);
        htmlPage.replace("%IP%", WiFi.localIP().toString());
        server->send(200, "text/html", htmlPage);
    });

    Serial.println("Launching server on port 80");

    server->begin();
}

void stopServer() {
    Serial.println("Stopping server");

    server->stop();
}