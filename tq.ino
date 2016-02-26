#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

int sensor = A0;  // the pin that the sensor is attached to
int state = LOW;  // by default, no motion detected
int val = 0;      // variable to store the sensor status (value)

bool isDEBUG = true;

void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  Serial.println(myWiFiManager->getConfigPortalSSID());
}

void setup() {
  // init serial
  Serial.begin(115200);

  // init wifi
  WiFiManager wifiManager;
  // wifiManager.resetSettings();
  wifiManager.setAPCallback(configModeCallback);

  wifiManager.setTimeout(300); // set timeout (5min) so esp doesn't wait around indefinately to be configured
  wifiManager.setMinimumSignalQuality(25); // set min signal strength
  wifiManager.setDebugOutput(isDEBUG); // set to false for production

  if (!wifiManager.autoConnect("TQ2", "teaisfortitties")) {
    if (isDEBUG) {
      Serial.println("Failed to connect, reached timeout");
      Serial.println("Resetting device. Try Again.");
    }
    ESP.reset();
    delay(1000);
  }

  if (isDEBUG) {
    Serial.println("Connected! Yay");
    Serial.print("Local IP: ");
    Serial.println(WiFi.localIP());
  }

  // init sensor as an input
  pinMode(sensor, INPUT);

  // wait a bit
  delay(3000);
}

void loop(){
  val = analogRead(sensor);   // read sensor value
  Serial.print("val: ");
  Serial.println(val);
  
  if (val >= 200) {
    delay(2000);
    
    if (state == LOW) {
      Serial.println("Motion detected!"); 
      state = HIGH;
    }
  } 
  else {
     delay(200);
      
    if (state == HIGH){
      Serial.println("Motion stopped!");
      state = LOW;
    }
  }
}
