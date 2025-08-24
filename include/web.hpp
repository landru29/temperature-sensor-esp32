#ifndef __web_h__
#define __web_h__

#include <WebServer.h>

extern WebServer *server;

void launchServer();
void stopServer();

#endif