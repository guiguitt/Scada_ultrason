#include <hcsr04.h>
#include "config.h"
#include "wifi.h"

#define TRIG_PIN 3
#define ECHO_PIN 4
#define LED_ON 0
#define LED_WIFI_ON 1
#define LED_SCADA_FAIL 2

bool sent;

HCSR04 hcsr04(TRIG_PIN, ECHO_PIN, 20, 4000);

void setup(){
    Serial.begin(9600);
    while(!Serial){}
    pinMode(LED_ON, OUTPUT);
    pinMode(LED_WIFI_ON, OUTPUT);
    pinMode(LED_SCADA_FAIL, OUTPUT);
    
    digitalWrite(LED_ON,HIGH);
    digitalWrite(LED_WIFI_ON, LOW);
    digitalWrite(LED_SCADA_FAIL, LOW);
}

void loop() {
  
   Serial.print("Distance : ");
   
   int duration = hcsr04.echoInMicroseconds();
    int distance = (duration / 2) * 0.344;

    Serial.print(distance); 
   // Serial.print(hcsr04.distanceInMillimeters());
    Serial.print(" mm");
    Serial.println();
    
    // Sending data
     if (!wifiConnected()){
     wifiConnect(WIFI_SSID, WIFI_PASSWORD);
     digitalWrite(LED_WIFI_ON, HIGH);
     Serial.print("WIFI connected");
     sent = sendData(String(distance));
     digitalWrite(LED_WIFI_ON, LOW);
      }

      if(!sent){
      digitalWrite(LED_SCADA_FAIL, HIGH);
      delay(1000);
      NVIC_SystemReset();
      digitalWrite(LED_SCADA_FAIL, LOW);
      }
      
    delay(500);
}
