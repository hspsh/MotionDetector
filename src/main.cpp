#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

const char* ssid     = "eduram";         // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "zarazcipodam";     // The password of the Wi-Fi network

int inputPin = 4;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status

const char* address = "http://192.168.88.137/play/uwu";
int time_delay = 3*1000;

//This happens when movement is detected
void movement_detected() {
  Serial.println("Motion detected!");
  // Send an HTTP POST request depending on timerDelay
  //Check WiFi connection status
  if(WiFi.status()== WL_CONNECTED){
    WiFiClient client;
    HTTPClient http;
      
    // Your Domain name with URL path or IP address with path
    http.begin(client, address);
      
    // Send HTTP GET request
    int httpResponseCode = http.GET();
      
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
  delay(time_delay);
}
//This happens when movement just ended
void movement_ended() {
  Serial.println("Motion ended!");
}

void setup() {
  // put your setup code here, to run once:
  pinMode(inputPin, INPUT);     // declare sensor as input
  Serial.begin(115200);
  delay(10);
  Serial.println('\n');
  
  WiFi.begin(ssid, password);             // Connect to the network
  Serial.print("Connecting to ");
  Serial.print(ssid); Serial.println(" ...");
  int i = 0;
  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(1000);
    Serial.print(++i); Serial.print(' ');
  }
  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());         // Send the IP address of the ESP8266 to the computer
}



void action() {
  if (pirState == LOW) {
      // we have just turned on
      movement_detected();
      // We only want to print on the output change, not state
      pirState = HIGH;
    } 
    else {
    if (pirState == HIGH){
      // we have just turned of
      movement_ended();
      // We only want to print on the output change, not state
      pirState = LOW;
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  val = digitalRead(inputPin);  // read input value
  if (val == HIGH) {            // check if the input is HIGH
    action();
  }
}