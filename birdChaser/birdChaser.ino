
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Servo.h>
#include "index.h" //Our HTML webpage contents with javascripts

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

// GPIO the servo is attached to
static const int servoPin = D2;

// Replace with your network credentials
const char* ssid = "BalazsEsAlbert";
const char* password = "emeseesrobi87";

// config static IP
IPAddress ip(192, 168, 0, 188); // where 155 is the desired IP Address
IPAddress gateway(192, 168, 0, 1); // set gateway
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

void setup() {
  Serial.begin(115200);

  myservo.attach(servoPin);  // attaches the servo on the servoPin to the servo object

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
  server.on("/moveServo", moveServo);
  
  server.begin();

  
}

void loop(){
  server.handleClient();
}
