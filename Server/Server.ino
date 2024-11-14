#include <WiFi.h>
#include <ESPmDNS.h>
#include <ArduinoOTA.h>
#include "Button.h"
#include <ESP32Servo.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <BLEDevice.h>

AsyncWebServer server(80);
WiFiClient HTTPClient;

int lenbutton = 18;
Button button[] = {
  Button(20), Button(19), Button(8), Button(18), Button(10), Button(9), Button(46), Button(3), Button(12),
  Button(0), Button(35), Button(2), Button(21), Button(47), Button(48), Button(45), Button(13), Button(14)
};
struct structureservo {
  bool b;
  byte pin;
  byte off;
  byte on;
  Servo servo;
  long servoms;
  long offms;
};
struct structurebeep {
  byte pin;
  int freq;
  byte resolution;
  byte channel;
  long beepms;
  bool beep;
};
struct structure {
  byte pin;
  bool b;
  String s;
  long on;
  long off;
  bool blazer;
};
//ble[30];
long bletime = -1;
//long bleswitch = 0;
byte blemess;
bool blebool = false;

structureservo servo = {true , 11, 180, 50 };
structurebeep beep = { 1 , 2000, 12, 3};
bool disco = false;
long discoms;
int lenrele = 14;
bool au[] = {false,//кабинет 3 свето
             false,//кабинет люстра
             false,//каб подсветка
             false // движение корридор
            };
structure rele[] = {
  { 4, false, "Туалет", -1, -1, false },
  { 5, false, "Ванная", -1, -1, false },
  { 6, false, "Кухня", -1, -1, false },
  { 7, false, "Коридор", -1, -1, false },
  { 15, false, "Спальня", -1, -1, false },
  { 16, false, "Кабанет", -1, -1, false },
  { 17, false, "rele7", -1, -1, false },
  { 42, false, "Светодиод1", -1, -1, false },
  { 41, false, "Светодиод2", -1, -1, false },
  { 40, false, "Светодиод3", -1, -1, false },
  { 39, false, "Светодиод4", -1, -1, false },
  { 38, false, "rele12", -1, -1, false },
  { 37, false, "Зал-Люстра", -1, -1, false },
  { 36, false, "rele14", -1, -1, false }
};
TaskHandle_t xbutton, xstructure;
unsigned long previousMillisWiFi = 0;
void setup() {
  //neopixelWrite(RGB_BUILTIN, RGB_BRIGHTNESS, 0, 0);
  Serial.begin(115200);
  BLEDevice::init("BLE");
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
  }
  WiFiStart();
  OTAStart();
  wwwStart();

  for (byte i = 0; i < lenrele; i++) {
    pinMode(rele[i].pin, OUTPUT);
    rele[i].b = digitalRead(rele[i].pin);
  }
  //if (i == 12 || i == 13) {
  for (byte i = 0; i < 18; i++) {
    if (i >= 12) {
      button[i].setTapHandler(clickup);
    } else {
      button[i].setPressedHandler(clickup);
    }
    button[i].setLongHandler(clickLong);
  }
  pinMode(servo.pin, OUTPUT);
  pinMode(beep.pin, OUTPUT);
  servo.servo.attach(servo.pin);
  servo.servo.write(servo.off);
  servo.servoms = -1;
  beep.beepms = -1;
  beep.beep = false;
  //ledcSetup(10, 2000, 8);       //beep
  //ledcAttach(beep.channel, beep.freq, beep.resolution);       //beep
  ledcAttachChannel(beep.pin, beep.freq, beep.resolution, beep.channel);
  //ledcAttachPin(beep.pin, beep.channel);  //beep
  xTaskCreatePinnedToCore(task_button, "task_button", 10000, NULL, 1, &xbutton, 0);
  xTaskCreatePinnedToCore(task_structure, "task_structure", 20000, NULL, 1, &xstructure, 0);
  blesend(1);
  //neopixelWrite(RGB_BUILTIN, 0, 0, RGB_BRIGHTNESS);
  //swrele(5,1);//кабинет
  //swrele(12,1);//зал
}

void swrele(byte i, bool b) {
  Serial.print(i);
  Serial.print("|");
  Serial.println(b);
  digitalWrite(rele[i].pin, b);
  rele[i].b = b;
  if (!b) rele[i].off = -1;
}
void blazer() {
  //ledcWrite(beep.channel, 10);
  ledcWriteTone(beep.channel, beep.freq);
  beep.beep = true;
  beep.beepms = millis() + 300;
}
void loop() {
  ArduinoOTA.handle();
  if (disco && (millis() - discoms >= 1000)) {
    discoms = millis();
    swrele(7, random(2));
    swrele(8, random(2));
    swrele(9, random(2));
    swrele(10, random(2));
  }
  if ((WiFi.status() != WL_CONNECTED) && (millis() - previousMillisWiFi >= 60000)) {
    //neopixelWrite(RGB_BUILTIN, RGB_BRIGHTNESS, 0, 0);
    Serial.print(millis());
    Serial.println("Reconnecting to WiFi...");
    WiFi.disconnect();
    WiFi.reconnect();
    previousMillisWiFi = millis();
  } else {
    //neopixelWrite(RGB_BUILTIN, 0, RGB_BRIGHTNESS, 0);
  }
}
