#define BLYNK_TEMPLATE_ID "TMPL3y8Qjwwiu"
#define BLYNK_TEMPLATE_NAME "Eduexpose IOT"
#define BLYNK_AUTH_TOKEN "-NkXnaRAGySHI3v3uNKGLOHXl3ui_mtR"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "DHT.h"

// Blynk Auth
char auth[] = BLYNK_AUTH_TOKEN;

// WiFi credentials
char ssid[] = "Wokwi-GUEST";   // Wokwi default WiFi
char pass[] = "";

// Define pins
#define DHTPIN 15
#define DHTTYPE DHT22   // Change to DHT11 if needed
#define TRIG_PIN 5
#define ECHO_PIN 18
#define LDR_PIN 34
#define GAS_PIN 4
#define PIR_PIN 23
#define LED_PIN 2   // New LED controlled from Blynk

// Virtual Pins for Blynk
#define VPIN_TEMP V0
#define VPIN_HUM  V1
#define VPIN_DIST V2
#define VPIN_LDR  V3
#define VPIN_GAS  V4
#define VPIN_PIR  V5
#define VPIN_LED  V6  // Switch in Blynk app

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

// Blynk LED control
BLYNK_WRITE(VPIN_LED) {
  int ledState = param.asInt();
  digitalWrite(LED_PIN, ledState);
}

void setup() {
  Serial.begin(115200);

  dht.begin();

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LDR_PIN, INPUT);
  pinMode(GAS_PIN, INPUT);
  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  // Connect to Blynk
  Blynk.begin(auth, ssid, pass);

  Serial.println("ESP32 + Wokwi + Blynk Started...");
}

void loop() {
  Blynk.run();

  // ---- DHT Sensor ----
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (!isnan(temperature) && !isnan(humidity)) {
    Blynk.virtualWrite(VPIN_TEMP, temperature);
    Blynk.virtualWrite(VPIN_HUM, humidity);
    Serial.printf("Temp: %.2f °C | Hum: %.2f %%\n", temperature, humidity);
  }

  // ---- Ultrasonic Sensor ----
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  float distance = duration * 0.034 / 2; // cm
  Blynk.virtualWrite(VPIN_DIST, distance);
  Serial.printf("Distance: %.2f cm\n", distance);

  // ---- LDR Sensor (Digital) ----
  int ldrValue = digitalRead(LDR_PIN);
  Blynk.virtualWrite(VPIN_LDR, !ldrValue);
  Serial.printf("LDR: %s\n", ldrValue ? "Light" : "Dark");

  // ---- Gas Sensor (Digital) ----
  int gasValue = digitalRead(GAS_PIN);
  Blynk.virtualWrite(VPIN_GAS, !gasValue);
  Serial.printf("Gas: %s\n", gasValue ? "Safe" : "Detected!");

  // ---- PIR Sensor ----
  int pirValue = digitalRead(PIR_PIN);
  Blynk.virtualWrite(VPIN_PIR, pirValue);
  Serial.printf("PIR: %s\n", pirValue ? "Motion" : "No Motion");

  Serial.println("------------------------");
  delay(2000);
}


