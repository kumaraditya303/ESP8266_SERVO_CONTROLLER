/*
  Project made and maintained by Kumar Aditya

  ESP8266 Servo Controller

  Create a ESP8266 Webserver for controlling the real-time position of the servo motor attached to ESP8266.
  
  Change these lines as per yours:
    const char *ssid = "REPLACE_WITH_YOUR_SSID";         // replace with your SSID
    const char *password = "REPLACE_WITH_YOUR_PASSWORD"; // replace with your Password
    const uint8_t servoPin = D4;                         // replace with servo pin

  Parts list:
  
  NodeMCU :
  https://amzn.to/397GzNe

  Servo Motor :
  https://amzn.to/2SmyFtJ
  
  The source code can be found at:
  https://git.io/JfOQv
  
  Download ESPAsyncWebServer library :
  https://github.com/me-no-dev/ESPAsyncWebServer/archive/master.zip

  Download ESPAsyncTCP library :
  https://github.com/me-no-dev/ESPAsyncTCP/archive/master.zip

  Download ESP8266 Filesystem Uploader :
  https://github.com/esp8266/arduino-esp8266fs-plugin/releases/download/0.5.0/ESP8266FS-0.5.0.zip
  
*/

// Import libraries 
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Servo.h>


const char *ssid = "REPLACE_WITH_YOUR_SSID";         // replace with your SSID
const char *password = "REPLACE_WITH_YOUR_PASSWORD"; // replace with your Password
const uint8_t servoPin = D4;                         // replace with servo pin
/* Create Servo Object */
Servo servo;
// Create Server instance
AsyncWebServer server(80);
void setup()
{
  // Attach Servo, start SPIFFS and Connect to WiFi
  Serial.begin(115200);
  servo.attach(servoPin);
  if (!SPIFFS.begin())
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi..");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.print("\nConnected to the WiFi network: ");
  Serial.print(WiFi.SSID());
  Serial.print("IP address:");
  Serial.print(WiFi.localIP());
  // Send home page from SPIFFS
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("Send index.html.");
    request->send(SPIFFS, "/index.html", "text/html");
  });
  // Receive Angle from client and process it 
  server.on("/angle", HTTP_POST, [](AsyncWebServerRequest *request) {
    String angle = request->arg("angle");
    Serial.println("Current Position: " + angle + "°");
    servo.write(angle.toInt());
    request->send(200);
  });
  // Send Favicon 
  server.serveStatic("/favicon.ico", SPIFFS, "/favicon.ico");
  // Begin Server
  server.begin();
}
void loop()
{
}
