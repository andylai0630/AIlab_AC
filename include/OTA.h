#ifndef OTA_H
#define OTA_H

#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266mDNS.h>
void performOTAUpdate();
void OTAsetup();
void OTAhandleClient();
void OTAupdate();

#endif  // OTA_H
