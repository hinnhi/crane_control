#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

//crane controller, http client on esp32
//Michele Missiroli
//5AEI 

//WiFi information
#define SSID  "****"
#define PASSWORD  "****"

//engines
#define m1ClockWise 12
#define m1AntiClockWise 14
#define m2ClockWise 27
#define m2AntiClockWise 26
#define m3ClockWise 25
#define m3AntiClockWise 23

//sensors
#define LimitSwitch1 32
#define LimitSwitch2 35
#define LimitSwitch3 34
#define LimitSwitch4 13

//switch automatic/manual
#define Automatic 16
#define Manual 4

String payload;   //payload of the HTTPS request
int intpayload = 0;   //payload converted to integer
int oldpayload = 0;

void setup() {
  Serial.begin(115200);
  connectWiFi(SSID, PASSWORD);
  setStatus();
  pinMode(m1ClockWise, OUTPUT); 
  pinMode(m1AntiClockWise, OUTPUT);
  pinMode(m2ClockWise, OUTPUT);
  pinMode(m2AntiClockWise, OUTPUT);
  pinMode(m3ClockWise, OUTPUT);
  pinMode(m3AntiClockWise, OUTPUT);
  stopEngines();
  pinMode(LimitSwitch1, INPUT);
  pinMode(LimitSwitch2, INPUT);
  pinMode(LimitSwitch3, INPUT);
  pinMode(LimitSwitch4, INPUT);
  pinMode(Automatic, INPUT);
  pinMode(Manual, INPUT);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED){
    Serial.println("Error with WiFi network");
    stopEngines();
    connectWiFi(SSID, PASSWORD);
  }
  while (WiFi.status() == WL_CONNECTED && digitalRead(Manual)==HIGH){
    if(digitalRead(LimitSwitch1)==HIGH || digitalRead(LimitSwitch2)==HIGH || digitalRead(LimitSwitch3)==HIGH || digitalRead(LimitSwitch4)==HIGH){
      stopEngines();
      setStatus();
      Serial.println("Physical limit reached, you can no longer turn in this direction!");
    }
    HTTPClient http;
    http.begin("****");
    int httpCode = http.GET();
    if (httpCode > 0){
      payload = http.getString();
      intpayload = payload.toInt();
      Serial.print(payload);
    }
    else{
      Serial.println("Error on HTTP request");
    }
    http.end();
    switch(intpayload){
      case 1:
        if(oldpayload != intpayload){
          stopEngines();
        }
        digitalWrite(m1ClockWise, HIGH);
        Serial.println(" --> engine 1 clockwise spinnng");
        oldpayload = intpayload;
        break;
      case 2:
        if(oldpayload != intpayload){
          stopEngines();
        }
        digitalWrite(m1AntiClockWise, HIGH);
        Serial.println(" --> engine 1 anti-clockwise spinnng");
        oldpayload = intpayload;
        break;
      case 3:
        if(oldpayload != intpayload){
          stopEngines();
        }
        digitalWrite(m2ClockWise, HIGH);
        Serial.println(" --> engine 2 clockwise spinnng");
        oldpayload = intpayload;
        break;
      case 4:
        if(oldpayload != intpayload){
          stopEngines();
        }
        digitalWrite(m2AntiClockWise, HIGH);
        Serial.println(" --> engine 2 anti-clockwise spinnng");
        oldpayload = intpayload;
        break;
      case 5:
        if(oldpayload != intpayload){
          stopEngines();
        }
        digitalWrite(m3ClockWise, HIGH);
        Serial.println(" --> engine 3 clockwise spinnng");
        oldpayload = intpayload;
        break;
      case 6:
        if(oldpayload != intpayload){
          stopEngines();
        }
        digitalWrite(m3AntiClockWise, HIGH);
        Serial.println(" --> engine 3 anti-clockwise spinnng");
        oldpayload = intpayload;
        break;
      case 7:
        Serial.println(" --> Reset");
        reset();
        break;
      default:
        Serial.println(" --> Idle state");
        stopEngines();
        break;
    }
  }
  while (WiFi.status() == WL_CONNECTED && digitalRead(Automatic)==HIGH){
    Serial.println("Automatic mode activated");
    reset();
    //calculate the times for automatic operation
    setStatus();
    delay(100);
  }
}

void reset(){
  stopEngines();
  //reset axes
  setStatus();
}

void stopEngines(){
  digitalWrite(m1ClockWise, LOW);
  digitalWrite(m1AntiClockWise, LOW);
  digitalWrite(m2ClockWise, LOW);
  digitalWrite(m2AntiClockWise, LOW);
  digitalWrite(m3ClockWise, LOW);
  digitalWrite(m3AntiClockWise, LOW);
}

void connectWiFi(const char* ssid, const char* password){
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.printf("\nDevice is connecting to WiFi using SSID %s and Passphrase %s.\n", ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      Serial.print("Connecting to wifi... ");
      delay(1000);
  }
  Serial.println("WiFi connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void setStatus(){
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin("****");
    int httpCode = http.GET();
    if (httpCode > 0){
      String payload = http.getString();
      Serial.println(payload);
    }
    else{
      Serial.println("Error on HTTP request");
    }
    http.end();
  }
  else{
    Serial.println("Error with WiFi network");
    stopEngines();
    connectWiFi(SSID, PASSWORD);
  }
}
