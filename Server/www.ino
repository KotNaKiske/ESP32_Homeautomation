String processor(const String& var) {
  //Serial.println("|var");
  //Serial.println(rele[(var.substring(1)).toInt()].b);
  //Serial.println(var);
  byte i;
  if (var[0] == 'n') {
    if (var[1] == 'r') {
      i = var.substring(2).toInt();
      return String(rele[i].s);
    }
    if (var[1] == 'b') {
      i = var.substring(2).toInt();
      return String(button[i].pin);
    }
  }
  if (var[0] == 'a') {
    i = var.substring(1).toInt();
    if (au[i]) {
      return F("1");
    }
    else
      return F("0");
  }
  if (var[0] == 'b') {
    i = var.substring(1).toInt();
    if (button[i].state) {
      return F("1");
    }
    else
      return F("0");
  }
  if (var[0] == 'r') {
    i = var.substring(1).toInt();
    if (rele[i].b) {
      if (rele[i].off > 0) {
        return String(rele[i].off);
      }
      else
        return F("-1");
    }
    else
      return F("0");
  }

  return String();

}
void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}
void wwwStart() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  server.on("/output.html", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/output.html", String(), false, processor);
  });
  server.on("/input.html", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/input.html", String(), false, processor);
  });
  server.on("/info.html", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/info.html", String(), false, processor);
  });
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/style.css", String(), false);
  });
  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/script.js", String(), false);
  });
  server.on("/d1", HTTP_GET, [](AsyncWebServerRequest * request) {
    wwwaction(request);
    request->send(SPIFFS, "/d1", String(), false, processor);
  });
  server.on("/d2", HTTP_GET, [](AsyncWebServerRequest * request) {
    wwwaction(request);
    request->send(SPIFFS, "/d2", String(), false, processor);
  });
  server.on("/d3", HTTP_GET, [](AsyncWebServerRequest * request) {
    wwwaction(request);
    request->send(SPIFFS, "/d3", String(), false, processor);
  });
  server.onNotFound(notFound);
  server.begin();
}
void wwwaction(AsyncWebServerRequest *request) {
  if (request->args() > 0) {
    String a = request->argName(0);
    if (a[0] == 'r') {
      bool b = (a[a.length() - 1] == '1');
      a = a.substring(1, a.length() - 1);
      swrele(a.toInt(), b);
    }
    /*if (a == "GM") {
      clickup(button[16]);
      }*/
    if (a[0] == 'a') {//
      if (a[1] == '0') {//кабинет светодиоды
        clickup(button[15]);
        //blesend(a.substring(1).toInt());
      }
      if (a[1] == '1') {//кабинет люстра
        clickup(button[17]);
      }
      if (a[1] == '2') {//кабинет подсветка
        blesend(a.substring(1).toInt());
      }
    }
  }
}
