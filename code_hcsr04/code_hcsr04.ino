#include <hcsr04.h>
#include "config.h"
#include "wifi.h"

#define TRIG_PIN 3
#define ECHO_PIN 4
#define LED_ON 0
#define LED_WIFI_ON 1
#define LED_SCADA_FAIL 2

#define serial 0
#define network 1
bool sent;

HCSR04 hcsr04(TRIG_PIN, ECHO_PIN, 20, 4000);

float AverMeas(int num, int timing) {
  float dis = 0;
  for (int i = 0; i < num; i++) {
    double duration = hcsr04.echoInMicroseconds();
    dis += (duration / 2) * 0.344;
    delay(timing);
  }
  return dis / num;
}

void setup() {

  Serial.begin(9600);
#if serial
  while (!Serial) {}
#endif

  pinMode(LED_ON, OUTPUT);
  pinMode(LED_WIFI_ON, OUTPUT);
  pinMode(LED_SCADA_FAIL, OUTPUT);

  digitalWrite(LED_ON, HIGH);
  digitalWrite(LED_WIFI_ON, LOW);
  digitalWrite(LED_SCADA_FAIL, HIGH);
}

void loop() {
  float distance = AverMeas(200, 100);
#if serial
  Serial.print("Distance : ");
  Serial.print(distance);
  Serial.println(" mm");
#endif

  // Sending data
#if network
  if (!wifiConnected()) {
    digitalWrite(LED_WIFI_ON, LOW);
    wifiConnect(WIFI_SSID, WIFI_PASSWORD);
    digitalWrite(LED_SCADA_FAIL, LOW);
    Serial.print("WIFI connected");
  }
  sent = sendData(String(distance));

  digitalWrite(LED_WIFI_ON, HIGH);

  if (!sent) {
    //NVIC_SystemReset();
    digitalWrite(LED_SCADA_FAIL, HIGH);
  }
#endif

  delay(1000);
}
