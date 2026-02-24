#include <WiFi.h>
#include <WebServer.h>
#include <Servo.h>

// ===== WiFi =====
const char* ssid = "YOUR_WIFI";
const char* password = "YOUR_PASSWORD";

// ===== Pins =====
int lightRelay = 26;
int fanRelay = 27;
int servoPin = 14;

// ===== Objects =====
WebServer server(80);
Servo door;

// ===== Setup =====
void setup() {
  Serial.begin(115200);

  pinMode(lightRelay, OUTPUT);
  pinMode(fanRelay, OUTPUT);

  door.attach(servoPin);
  door.write(0); // Door Closed

  digitalWrite(lightRelay, HIGH);
  digitalWrite(fanRelay, HIGH);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected");
  Serial.println(WiFi.localIP());

  // ===== AI Commands =====
  server.on("/light/on", []() {
    digitalWrite(lightRelay, LOW);
    server.send(200, "text/plain", "Light ON");
  });

  server.on("/light/off", []() {
    digitalWrite(lightRelay, HIGH);
    server.send(200, "text/plain", "Light OFF");
  });

  server.on("/fan/on", []() {
    digitalWrite(fanRelay, LOW);
    server.send(200, "text/plain", "Fan ON");
  });

  server.on("/fan/off", []() {
    digitalWrite(fanRelay, HIGH);
    server.send(200, "text/plain", "Fan OFF");
  });

  server.on("/door/open", []() {
    door.write(90);
    server.send(200, "text/plain", "Door Open");
  });

  server.on("/door/close", []() {
    door.write(0);
    server.send(200, "text/plain", "Door Closed");
  });

  server.begin();
}

// ===== Loop =====
void loop() {
  server.handleClient();
}