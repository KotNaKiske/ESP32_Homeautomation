void OTAStart() {
  ArduinoOTA.setHostname("Home Server");
  ArduinoOTA.begin();
}
void WiFiStart() {
  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);
  WiFi.hostname("HomeServer");
  WiFi.begin("A", "1234554321");
  Serial.println("WiFiStart");
}
