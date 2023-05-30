#include <Ethernet.h>
#include <SPI.h>
#include <SD.h>

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include <IRsend.h>
#include <Wire.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266httpUpdate.h>
#include "MQTT.h"

#define _SSID "AILab"        // Your WiFi SSID
#define _PASSWORD "ailab120"    // Your WiFi Password
#define PROJECT_ID "aircondecode-default-rtdb"   // Your Firebase Project ID. Can be found in project settings.
const char* broker = "140.123.106.232"; //IP address of broker
const int port = 1883;
const String Ver = "V1.0";
//SD Card
File myFile;

const int chipSelect = D8;

// for wireless update
const char* host = "esp8266-webupdate";
ESP8266WebServer server(80);
const char* serverIndex = "<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>";

/*
  // FAST METHOD
  // REQUIRES PERIODIC UPDATE OF FINGERPRINT (EVERY 1 YEAR)
  // OUTDATED FINGERPRINT WILL STOP THE SERVICE
  // LATEST FINGERPRINT CAN BE FOUND AT- https://www.grc.com/fingerprints.htm?domain=test.firebaseio.com
  // REPLACE COLONS(:) WITH A SPACE(" ") AS SHOWN BELOW
  // 03:9E:4F:E6:83:FC:40:EF:FC:B2:C5:EF:36:0E:7C:3C:42:20:1B:8F ==> 03 9E 4F E6 83 FC 40 EF FC B2 C5 EF 36 0E 7C 3C 42 20 1B 8F
  #define FINGERPRINT "03 9E 4F E6 83 FC 40 EF FC B2 C5 EF 36 0E 7C 3C 42 20 1B 8F"
  Firebase firebase(PROJECT_ID, FINGERPRINT);
  // UNCOMMENT THIS BLOCK AND COMMENT THE LINE BELOW
*/
WiFiClient espClient;
String state_temp;
PubSubClient client(espClient);
IRsend irsend(4); // ESP8266 GPIO pin (GPIO 4) to use.
int temperature =26;
int function = 3;        //強0 弱1 微2 自動3 開關機4 韻律開1 關0
int rhythm = 0;
String firebase_search =String(temperature)+'_'+String(function)+'_'+String(rhythm);
int onoff_flag = 0;
HTTPClient httpClient;
void connect();

void HttpOTA(){
  
  t_httpUpdate_return ret = ESPhttpUpdate.update(httpClient,"http://140.123.106.232:8000/ac_sd_V1.0.ino.nodemcu.bin");

        switch(ret) {
            case HTTP_UPDATE_FAILED:
                Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
                break;

            case HTTP_UPDATE_NO_UPDATES:
                Serial.println("HTTP_UPDATE_NO_UPDATES");
                break;

            case HTTP_UPDATE_OK:
                Serial.println("HTTP_UPDATE_OK");
                break;
        }

}

void send(String tmp){
  if(onoff_flag){
    if(tmp =="AI_Smart_AC_121_B::temperature_up")temperature+=1;
    if(tmp =="AI_Smart_AC_121_B::temperature_down")temperature-=1;
    if(tmp =="AI_Smart_AC_121_B::rhythm")rhythm+=1;
    if(tmp =="AI_Smart_AC_121_B::auto")function=3;
    if(tmp =="AI_Smart_AC_121_B::breez")function=2;
    if(tmp =="AI_Smart_AC_121_B::weak")function=1;
    if(tmp =="AI_Smart_AC_121_B::strong")function=0;
    if(rhythm>1) rhythm=0;
    if(temperature>30) temperature=30;
    if(temperature<20) temperature=20;
    if(function==4) function=3;  
    firebase_search =String(temperature)+'_'+String(function)+'_'+String(rhythm);
    if(tmp =="AI_Smart_AC_121_B::power_onoff")firebase_search="on_off";
    Serial.println(firebase_search);
    //----------------SD Read---------------------
    String str;
    myFile = SD.open("AIR~1.JSO");
  if (myFile) {
    // read from the file until there's nothing else in it:
    int line_count = 0;
    while (myFile.available()) {
      str = myFile.readStringUntil('\n');
      if (str.substring(3,9) == firebase_search )//判斷是否為輸入的指令
      {
        Serial.println("find");
        break;
      }    
    }
    //Serial.println(str.substring(14,str.length()-2));//擷取字串中的訊號
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening AIR~1.JSO");
  }
  //--------------------------------------------
    
    String comm = str.substring(14,str.length()-1);
    //uint16_t data[343]={8944,4528,560,1732,532,1700,560,580,560,580,560,584,556,584,556,580,564,1732,528,580,556,1732,528,1732,532,584,556,580,560,580,560,584,556,584,560,584,556,584,556,1724,540,580,556,584,560,580,560,584,556,580,564,580,560,1724,536,580,560,584,556,584,560,1704,556,580,560,1724,532,584,556,588,556,584,560,576,564,580,560,580,556,588,556,584,556,584,556,584,560,580,564,576,560,584,556,584,556,584,560,580,560,7988,560,580,564,1728,532,580,560,580,560,584,556,584,560,580,560,1728,532,1704,556,584,560,1728,532,584,556,580,564,1724,532,584,556,588,560,580,560,580,560,580,564,580,556,584,556,580,564,584,556,584,560,584,556,580,560,580,560,584,556,584,556,580,564,580,564,580,560,576,564,576,564,580,556,588,556,584,556,580,560,580,564,584,556,584,560,576,564,580,560,580,556,588,556,580,560,580,560,588,556,584,556,580,564,580,556,584,556,584,560,584,556,580,560,588,556,1724,532,584,560,580,560,584,556,584,560,580,560,580,560,580,556,7992,560,580,560,580,560,588,552,584,556,580,564,580,560,580,568,572,564,1724,532,584,556,584,560,580,560,580,560,580,560,580,560,584,556,584,560,580,560,580,560,580,556,588,556,580,560,580,560,588,556,580,556,584,560,580,560,580,560,580,564,580,556,584,556,584,556,588,556,580,564,576,560,1728,532,584,556,580,564,580,560,580,560,580,560,580,560,584,556,584,560,576,560,584,556,584,556,588,556,1724,532,584,560,580,560,1700,560,580,556,588,556,584,556,580,556};
    uint16_t r=0, t=0;
    uint16_t data[343]={};
    for (int i=0; i < comm.length(); i++)//將讀入的訊號字串轉換成uint陣列格式
    { 
      if(comm.charAt(i) == ',') 
      { 
        data[t] = comm.substring(r, i).toInt();
        r=(i+1); 
        t++; 
      }
    }
    irsend.sendRaw(data, sizeof(data)/sizeof(data[0]), 38);  // Send a raw data capture at 38kHz.
    
    //Serial.println(tmp);
    Serial.println(comm);
    comm= "";
    }
    onoff_flag = 1; 
}
void callback(char* topic, byte* payload, unsigned int leg) {
    Serial.print("Message arrived on topic: ");
    Serial.print(topic);
    Serial.print(". Message: ");
    
    
    for (int i = 0; i < leg; i++) {
      Serial.print((char)payload[i]);
      
    }
    if((char)payload[leg-1] == '1'){
      Serial.println("flash");
      HttpOTA();
    }
    
    String tmp="";
    
    for (int i = 0; i < leg; i++) {
      //Serial.print((char)payload[i]);
       tmp+=(char)payload[i];
    }
    send(tmp);
    
} 
void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    irsend.begin();
    Serial.begin(9600);

    Serial.print("Initializing SD card...");

  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
   Serial.println("Version: " + Ver);
  
    if (WiFi.status() != WL_CONNECTED){
      connect();
    }
    if (WiFi.waitForConnectResult() == WL_CONNECTED) {
    MDNS.begin(host);
    server.on("/", HTTP_GET, []() {
      server.sendHeader("Connection", "close");
      server.send(200, "text/html", serverIndex);
    });
    server.on("/update", HTTP_POST, []() {
      server.sendHeader("Connection", "close");
      server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
      ESP.restart();
    }, []() {
      HTTPUpload& upload = server.upload();
      if (upload.status == UPLOAD_FILE_START) {
        Serial.setDebugOutput(true);
        WiFiUDP::stopAll();
        Serial.printf("Update: %s\n", upload.filename.c_str());
        uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
        if (!Update.begin(maxSketchSpace)) { //start with max available size
          Update.printError(Serial);
        }
      } else if (upload.status == UPLOAD_FILE_WRITE) {
        if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
          Update.printError(Serial);
        }
      } else if (upload.status == UPLOAD_FILE_END) {
        if (Update.end(true)) { //true to set the size to the current progress
          Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
        } else {
          Update.printError(Serial);
        }
        Serial.setDebugOutput(false);
      }
      yield();
    });
    server.begin();
    MDNS.addService("http", "tcp", 80);
    }
}
void loop(void) {
    
    if (WiFi.status() != WL_CONNECTED or !client.connected()){
      connect();
    }
    
    server.handleClient();
    MDNS.update();
    client.loop();
}

MQTT mqtt(broker, port);
void connect(){
    onoff_flag = 0;
    WiFi.mode(WIFI_AP_STA);
    WiFi.disconnect();
    delay(1000);
    // Connect to WiFi
    Serial.println();
    Serial.println();
    Serial.print("Connecting to: ");
    Serial.println(_SSID);
    WiFi.begin(_SSID, _PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print("-");
    }
    Serial.println("");
    Serial.println("WiFi Connected");
    Serial.print("IP:");
    Serial.println(WiFi.localIP());
    // MQTT conncted
    client.setServer(broker, port);
    client.setCallback(callback);
    mqtt.connect();
    mqtt.subscribe("AI_Smart_AC_121_B");
  }
