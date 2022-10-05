
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <LinkedList.h>
#include "Button.h"
#include <WebServer.h>
#include <uri/UriBraces.h>
#include <Servo.h>

Servo servo;
long servoms;
long beepms;
bool beep;
WebServer www(80);
Button button[12] = {
  Button(36, INPUT), Button(39), Button(34), Button(35), /*Button(22)*/Button(36), /*Button(23)*/Button(36),
  Button(13, INPUT), Button(32, INPUT), Button(21), Button(0), Button(2), Button(15)
};
struct structure {
  byte pin;
  bool b;
  String s;
  long timeron;
  long timeroff;
  bool blazer;
};
structure rele[12] =  {
  {12, false, "", -1, -1, false}, //туалет       нерабочий
  {14, false, "&#1057;&#1055;&#1040;&#1051;&#1068;&#1053;&#1071;", -1, -1, false}, //спальня
  {26, false, "&#1050;&#1054;&#1056;&#1048;&#1044;&#1054;&#1056;", -1, -1, false}, //коридор
  {33, false, "&#1042;&#1040;&#1053;&#1053;&#1040;&#1071;", -1, -1, false}, //ванная
  {25, false, "&#1058;&#1059;&#1040;&#1051;&#1045;&#1058;", -1, -1, false}, //туалет
  {27, false, "&#1050;&#1059;&#1061;&#1053;&#1071;", -1, -1, true}, //кухня

  {4, false, "", -1, -1, false}, //склад грязи
  {16, false, "&#1047;&#1040;&#1051;", -1, -1, false}, //зал люстра
  {17, false, "&#1047;&#1040;&#1051;", -1, -1, false},  //ЗАЛ(СВЕТОДИОДЫ)(&#1057;&#1042;&#1045;&#1058;&#1054;&#1044;&#1048;&#1054;&#1044;&#1067;)
  {5, false, "&#1047;&#1040;&#1051;", -1, -1, false}, //ЗАЛ(СВЕТОДИОДЫ)
  {18, false, "&#1047;&#1040;&#1051;", -1, -1, false}, //ЗАЛ(СВЕТОДИОДЫ)
  {19, false, "&#1047;&#1040;&#1051;", -1, -1, false} //ЗАЛ(СВЕТОДИОДЫ)

};
TaskHandle_t xbutton, xstructure;
unsigned long previousMillisWiFi = 0;
void setup() {
  Serial.begin(115200);



  WiFiStart();
  OTAStart();
  wwwStart();
  for (byte i = 0; i < 12; i++) {
    pinMode(rele[i].pin, OUTPUT);
    rele[i].b = digitalRead(rele[i].pin);
    if (i == 6 || i == 7) {
      button[i].setTapHandler(clickup);
    } else {
      button[i].setPressedHandler(clickup);
    }
    //button[i].setBeepHandler(blazer);
    button[i].setLongHandler(clickLong);
  }
  pinMode(23, OUTPUT);
  pinMode(22, OUTPUT);


  servo.attach(23);
  servo.write(180);
  servoms = -1;

  beepms = -1;
  beep = false;
  ledcSetup(10, 2000, 8);
  ledcAttachPin(22, 10);

  xTaskCreatePinnedToCore(task_button, "task_button", 10000, NULL, 1, &xbutton, 0);
  xTaskCreatePinnedToCore(task_structure, "task_structure", 20000, NULL, 1, &xstructure, 0);
}
void swrele(byte i, bool b) {
  digitalWrite(rele[i].pin, b);
  rele[i].b = b;
  if (!b) rele[i].timeroff = -1;
}
void blazer() {
  ledcWrite(10, 10);
  beep = true;
  beepms = millis() + 300;
}
void loop() {
  ArduinoOTA.handle();
  //wifiManager.process();
  www.handleClient();
  if ((WiFi.status() != WL_CONNECTED) && (millis() - previousMillisWiFi >= 60000)) {
    Serial.print(millis());
    Serial.println("Reconnecting to WiFi...");
    WiFi.disconnect();
    WiFi.reconnect();
    previousMillisWiFi = millis();
  }
}
