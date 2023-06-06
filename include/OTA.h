#ifndef OTA_H
#define OTA_H

#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266mDNS.h>

class OTA
{
private:
public:
    void performOTAUpdate();
    void OTAsetup();
    void OTAhandleClient();
    void OTAupdate();
    void seturl(const char* url);
};
#endif // OTA_H
