// Import required libraries
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <WiFiClientSecure.h>
#include <ESP8266WebServer.h>
#include <Adafruit_Sensor.h>
#include <Servo.h>
#include "index.h" //Our HTML webpage contents with javascripts

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

// GPIO the servo is attached to
static const int servoPin = D2;

const char* ssid     = "Battery Meter";
const char* password = "123456789";

ESP8266WebServer server(80); //Server on port 80

void handleRoot() {
  String s = index_html; //Read HTML contents
  server.send(200, "text/html", s); //Send web page
  Serial.println("Loaded main index.h ");
}

void handleNotFound()
{
  server.sendHeader("Location", "/", true); //Redirect to our html web page
  server.send(302, "text/plane", "");
}


void moveServo() { 
   Serial.println("Move servo ");
    //Rotate the servo
    if (server.arg("newServoPos") != ""){
      myservo.write(server.arg("newServoPos").toInt());
      Serial.println(server.arg("newServoPos")); 
    }
   
   String json;
   server.send(200, "text/json", json);
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);

  myservo.attach(servoPin);  // attaches the servo on the servoPin to the servo object
  
  Serial.println("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.println("AP IP address: ");
  Serial.println(IP);

  // Print ESP8266 Local IP Address
  //Serial.println(WiFi.localIP());

  server.on("/", handleRoot);      //Which routine to handle at root location. This is display page
  server.onNotFound(handleNotFound);
  server.on("/moveServo", moveServo);
  
  server.begin();

  
}

void loop(){
  server.handleClient();
}
