#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "config.h"

#ifndef RELAY_PIN
#define RELAY_PIN 26
#endif

WebServer server(80);
bool relayState = false;

String jsonStatus(const char* status) {
  String s = "{\"status\":\"";
  s += status;
  s += "\",\"relay_state\":\"";
  s += (relayState ? "on" : "off");
  s += "\"}";
  return s;
}

void handleRoot() {
  server.send(
    200, "text/plain",
    "Контроллер обогревателя по Wi-Fi. "
    "Используйте /api/relay?state=on|off|status"
  );
}

void handleRelay() {
  if (!server.hasArg("state")) {
    server.send(400, "application/json",
                "{\"status\":\"error\",\"error\":\"отсутствует параметр state\"}");
    return;
  }
  String state = server.arg("state");
  if (state == "on") {
    relayState = true;
    digitalWrite(RELAY_PIN, HIGH);
    server.send(200, "application/json", jsonStatus("success"));
  } else if (state == "off") {
    relayState = false;
    digitalWrite(RELAY_PIN, LOW);
    server.send(200, "application/json", jsonStatus("success"));
  } else if (state == "status") {
    server.send(200, "application/json", jsonStatus("success"));
  } else {
    server.send(400, "application/json",
                "{\"status\":\"error\",\"error\":\"неверное значение state\"}");
  }
}

void setupAP() {
  String apName = String("HeaterCtrl-") +
                  String((uint32_t)ESP.getEfuseMac(), HEX).substring(4);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(apName.c_str(), AP_PASSWORD);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("Режим точки доступа. SSID: "); Serial.println(apName);
  Serial.print("IP точки доступа: "); Serial.println(IP);
}

void setupSTA() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WLAN_SSID, WLAN_PASSWORD);
  Serial.print("Подключаюсь к Wi-Fi");
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED &&
         millis() - start < WIFI_CONNECT_TIMEOUT_MS) {
    delay(500);
    Serial.print(".");
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println();
    Serial.print("Подключено. IP: "); Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nТайм-аут подключения к Wi-Fi, переключаюсь в режим точки доступа.");
    setupAP();
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  delay(100);
  setupSTA();

  server.on("/", HTTP_GET, handleRoot);
  server.on("/api/relay", HTTP_GET, handleRelay);
  server.begin();
  Serial.println("HTTP-сервер запущен");
}

void loop() {
  server.handleClient();
}
