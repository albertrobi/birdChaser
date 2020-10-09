
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Servo.h>
#include <EEPROM.h>
#include <ArduinoJson.h>
#include "index.h" //Our HTML webpage contents with javascripts

Servo myservoX;  // create servo object to control a servo
Servo myservoY;  // create servo object to control a servo
// twelve servo objects can be created on most boards

#define EEPROM_SIZE 32

// GPIO the servo is attached to
static const int servoPinX = D2;
static const int servoPinY = D3;

int startXaxis = 0;
int startYaxis = 0;
int endXaxis = 0;
int endYaxis = 0;
bool playOn = false;

// Replace with your network credentials
//const char* ssid = "BalazsEsAlbert";
//const char* password = "emeseesrobi87";

//const char* ssid = "Sirteq";
//const char* password = "091960Aa";

const char* ssid = "AlbertPanzio";
const char* password = "albertpanzio";

// config static IP
IPAddress ip(192, 168, 1, 188); // where 155 is the desired IP Address
IPAddress gateway(192, 168, 1, 1); // set gateway
IPAddress subnet(255, 255, 255, 0); // set subnet mask

// Set web server port number to 80
ESP8266WebServer server(80); //Server on port 80

//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void handleRoot() {
  String s = MAIN_page; //Read HTML contents
  server.send(200, "text/html", s); //Send web page
  Serial.println("Loaded main index.h ");
}

void handleNotFound()
{
  server.sendHeader("Location", "/", true); //Redirect to our html web page
  server.send(302, "text/plane", "");
}


void moveServoX() { 
   Serial.println("Move servo X ");
    //Rotate the servo
    if (server.arg("newServoPos") != ""){
      myservoX.write(server.arg("newServoPos").toInt());
      Serial.println(server.arg("newServoPos")); 
    }
   
   String json;
   server.send(200, "text/json", json);
}

void moveServoY() { 
   Serial.println("Move servo Y ");
    //Rotate the servo
    if (server.arg("newServoPos") != ""){
      myservoY.write(server.arg("newServoPos").toInt());
      Serial.println(server.arg("newServoPos")); 
    }
   
   String json;
   server.send(200, "text/json", json);
}

void setStartXaxis() { 
  Serial.println("startXaxis set to" + String(myservoX.read()));
  startXaxis = myservoX.read();
  eeWriteInt(1,startXaxis);
   String json;
   server.send(200, "text/json", json);
}

void setStartYaxis() { 
  Serial.println("startXaxis set to" + String(myservoY.read()));
   startYaxis = myservoY.read();
   eeWriteInt(10,startYaxis);
   String json;
   server.send(200, "text/json", json);
}

void setEndXaxis() { 
   Serial.println("startXaxis set to" + String(myservoX.read()));
   endXaxis = myservoX.read();
   eeWriteInt(5,endXaxis);
   String json;
   server.send(200, "text/json", json);
}

void setEndYaxis() { 
  Serial.println("startXaxis set to" + String(myservoY.read()));
   endYaxis = myservoY.read();
   eeWriteInt(15,endYaxis);
   String json;
   server.send(200, "text/json", json);
}

void playBeam() { 
  Serial.println("startXaxis set to" + String(startXaxis));
  Serial.println("endXaxis set to" + String(endXaxis));
  Serial.println("startYaxis set to" + String(startYaxis));
  Serial.println("endYaxis set to" + String(endYaxis));
  playOn = true;
  myservoX.write(startXaxis);
  myservoY.write(startYaxis);  
  String json;
  server.send(200, "text/json", json);
  playBeamRecursive();
}

void playBeamRecursive() {
  for (int i = startXaxis; i<endXaxis; i++) {
    for (int j=startYaxis; j<endYaxis; j++) {
       myservoY.write(j);
       delay(60);
       server.handleClient();
       if (!playOn) {
        break;
       }
    }
     myservoX.write(i);
     delay(20);
     server.handleClient();
     if (!playOn) {
        break;
       }
  }
  // start over
  playBeamRecursive();
}

void stopBeam() {
  Serial.println("stopBeam");
  playOn = false;
     String json;
   server.send(200, "text/json", json);
}

void readEEpromData() {
  startXaxis = eeGetInt(1);
  startYaxis = eeGetInt(10);
  endXaxis = eeGetInt(5);
  endYaxis = eeGetInt(15);
  Serial.println("startXaxis set to" + String(startXaxis));
  Serial.println("endXaxis set to" + String(endXaxis));
  Serial.println("startYaxis set to" + String(startYaxis));
  Serial.println("endYaxis set to" + String(endYaxis));
}

void getStartXvalue() {
    StaticJsonBuffer<300> JSONbuffer;
    JsonObject& JSONencoder = JSONbuffer.createObject();
    JSONencoder["startXaxis"] =  eeGetInt(1);
    JSONencoder["endXaxis"] =  eeGetInt(5);
    JSONencoder["startYaxis"] =  eeGetInt(10);
    JSONencoder["endYaxis"] =  eeGetInt(15);

    String json;
    JSONencoder.prettyPrintTo(json);
    server.send(200, "text/json", json);
}

// read write eeprom
void eeWriteInt(int pos, int val) {
    Serial.println("EEprom write" + String(val));
    byte* p = (byte*) &val;
    EEPROM.write(pos, *p);
    EEPROM.write(pos + 1, *(p + 1));
    EEPROM.write(pos + 2, *(p + 2));
    EEPROM.write(pos + 3, *(p + 3));
    EEPROM.commit();
}

int eeGetInt(int pos) {
  int val;
  byte* p = (byte*) &val;
  *p        = EEPROM.read(pos);
  *(p + 1)  = EEPROM.read(pos + 1);
  *(p + 2)  = EEPROM.read(pos + 2);
  *(p + 3)  = EEPROM.read(pos + 3);
  Serial.println("EEprom read" + String(val));
  return val;
}

void setup() {
  Serial.begin(115200);
  EEPROM.begin(EEPROM_SIZE);
  readEEpromData();
  
  myservoX.attach(servoPinX);  // attaches the servo on the servoPin to the servo object
  myservoY.attach(servoPinY);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);      //Which routine to handle at root location. This is display page
  server.onNotFound(handleNotFound);
  server.on("/moveServoX", moveServoX);
  server.on("/moveServoY", moveServoY);
  server.on("/startXaxis", setStartXaxis);
  server.on("/startYaxis", setStartYaxis);
  server.on("/endXaxis", setEndXaxis);
  server.on("/endYaxis", setEndYaxis);
  server.on("/playBeam", playBeam);
  server.on("/stopBeam", stopBeam);
  server.on("/getStartXvalue", getStartXvalue);
  
  
  
  server.begin();
  
}

void loop(){
  server.handleClient();
}
