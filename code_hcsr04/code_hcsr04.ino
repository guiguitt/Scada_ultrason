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

void setup() {
  Serial.begin(9600);
  while (!Serial) {}
  pinMode(LED_ON, OUTPUT);
  pinMode(LED_WIFI_ON, OUTPUT);
  pinMode(LED_SCADA_FAIL, OUTPUT);

  digitalWrite(LED_ON, HIGH);
  digitalWrite(LED_WIFI_ON, LOW);
  digitalWrite(LED_SCADA_FAIL, HIGH);
}

void loop() {

  Serial.print("Distance : ");
  double distance = 0;
  for (int i = 0; i < 10; i++) {
    delay(90);
    double duration = hcsr04.echoInMicroseconds();
    distance += (duration / 2) * 0.344;
  }
  distance = distance / 10;

  Serial.print(distance);
  //Serial.print(hcsr04.distanceInMillimeters());
  Serial.print(" mm");
  Serial.println();

  // Sending data
  if (!wifiConnected()) {
    digitalWrite(LED_WIFI_ON, LOW);
    wifiConnect(WIFI_SSID, WIFI_PASSWORD);
    digitalWrite(LED_SCADA_FAIL, LOW);
    Serial.print("WIFI connected");
  }
  sent = sendData(String(distance));

  digitalWrite(LED_WIFI_ON, HIGH);

  if (!sent) {
    NVIC_SystemReset();
    digitalWrite(LED_SCADA_FAIL, LOW);
  }

  delay(1000);
}
