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
#define m1CounterClockWise 14
#define m2ClockWise 27
#define m2CounterClockWise 26
#define m3ClockWise 25
#define m3CounterClockWise 23

//sensors
#define LimitSwitch1 32   //base(1) counter
#define LimitSwitch2 35   //base(2)
#define LimitSwitch3 34   //crane trolley(1)  counter
#define LimitSwitch4 13   //crane trolley(2)

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
  pinMode(m1CounterClockWise, OUTPUT);
  pinMode(m2ClockWise, OUTPUT);
  pinMode(m2CounterClockWise, OUTPUT);
  pinMode(m3ClockWise, OUTPUT);
  pinMode(m3CounterClockWise, OUTPUT);
  stopEngines();
  pinMode(LimitSwitch1, INPUT_PULLUP);  
  pinMode(LimitSwitch2, INPUT_PULLUP);  
  pinMode(LimitSwitch3, INPUT_PULLUP);  
  pinMode(LimitSwitch4, INPUT_PULLUP);  
  pinMode(Automatic, INPUT);
  pinMode(Manual, INPUT);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED){
    Serial.println("Error with WiFi network");
    stopEngines();
    connectWiFi(SSID, PASSWORD);
  }
  while (WiFi.status() == WL_CONNECTED && digitalRead(Manual) == HIGH){
    if(digitalRead(LimitSwitch1) == LOW || digitalRead(LimitSwitch2) == LOW || digitalRead(LimitSwitch3) == LOW || digitalRead(LimitSwitch4) == LOW){
      stopEngines();
      setStatus();
      Serial.println("Physical limit reached, you can no longer turn in this direction!");
    }
    HTTPClient http;
    http.begin("https://naoornever.it/crane-controller/");
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
        digitalWrite(m1CounterClockWise, HIGH);
        Serial.println(" --> engine 1 counter clockwise spinnng");
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
        digitalWrite(m2CounterClockWise, HIGH);
        Serial.println(" --> engine 2 counter clockwise spinnng");
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
        digitalWrite(m3CounterClockWise, HIGH);
        Serial.println(" --> engine 3 counter clockwise spinnng");
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
  while (WiFi.status() == WL_CONNECTED && digitalRead(Automatic) == HIGH){
    Serial.println("Automatic mode activated");
    stopEnigines();
    setStatus();
    //automatic mode, can be changed at will
    if(digitalRead(LimitSwitch1) == LOW){
      digitalWrite(m1CounterClockWise, HIGH);
      while(digitalRead(LimitSwitch1) == LOW && i<100){
        i++;
        delay(200);
      }
      digitalWrite(m1CounterClockWise, LOW);
      delay(50);
      digitalWrite(m1ClockWise, HIGH);
      while(digitalRead(LimitSwitch2) == LOW && i<100){
        i++;
        delay(200);
      }
      digitalWrite(m1ClockWise, LOW);
    }
    delay(50);
    if(digitalRead(LimitSwitch3) == LOW){
      digitalWrite(m2CounterClockWise, HIGH);
      while(digitalRead(LimitSwitch3) == LOW && i<100){
        i++;
        delay(200);
      }
      digitalWrite(m2CounterClockWise, LOW);
      delay(50);
      digitalWrite(m2ClockWise, HIGH);
      while(digitalRead(LimitSwitch4) == LOW && i<100){
        i++;
        delay(200);
      }
      digitalWrite(m2ClockWise, LOW);
    }
}

void reset(){
  stopEngines();
  //reset the axis of the base of the crane that allows it to turn on itself (in our case 340 degrees of autonomy)
  while(digitalRead(LimitSwitch1) == LOW){
    digitalWrite(m1CounterClockWise, HIGH);
  }
  digitalWrite(m1CounterClockWise, LOW);
  delay(50);
  digitalWrite(m1ClockWise, HIGH);
  delay(1);   //calculate the time to return to the centre
  digitalWrite(m1ClockWise, LOW);
  //reset the crane trolley
  while(digitalRead(LimitSwitch3) == LOW){
    digitalWrite(m2CounterClockWise, HIGH);
  }
  digitalWrite(m2CounterClockWise, LOW);
  delay(50);
  digitalWrite(m2ClockWise, HIGH);
  delay(100);   
  digitalWrite(m2ClockWise, LOW);
  setStatus();
  delay(50);
}

void stopEngines(){
  digitalWrite(m1ClockWise, LOW);
  digitalWrite(m1CounterClockWise, LOW);
  digitalWrite(m2ClockWise, LOW);
  digitalWrite(m2CounterClockWise, LOW);
  digitalWrite(m3ClockWise, LOW);
  digitalWrite(m3CounterClockWise, LOW);
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
    http.begin("https://naoornever.it/crane-controller/setStatus?status=idle");
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
