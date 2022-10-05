void wwwindex() {
  String html =
    "<!DOCTYPE HTML><html>\n"
    "<head>\n"
    "<title>Home Server</title>\n"
    "<style>\n"
    "html {font-family: Arial; display: inline-block; text-align: center;}\n"
    "body {background-color: #121212;color: white;}\n"
    "button {background-color: #000;color: #fff;padding: 30px;font-weight: bold;width: 220px;border-radius: 10px;margin: 5px;}\n"
    ".r{box-shadow: 0 0 20px 0 crimson,0 0 10px 0 crimson,0 0 30px 0 crimson inset;}"
    ".g{box-shadow: 0 0 20px 0 greenyellow,0 0 10px 0 greenyellow,0 0 30px 0 greenyellow inset;}"
    "</style>\n"
    "</head>\n"
    "<body>\n"
    "<div>\n";
  for (byte i = 0; i < 12; i++) {
    if (i == 3 || i == 6)
      html = html + "</div><div>";
    if (i == 8) {
      html = html + "<button id=""12"" class=""";
      if (rele[8].b) {
        html = html + "g";
      } else {
        html = html + "r";
      }
      html = html + """ onclick=""bb(this)"">&#1047;&#1040;&#1051;(&#1057;&#1042;&#1045;&#1058;&#1054;&#1044;&#1048;&#1054;&#1044;&#1067;)</button></div><div>";
    }
    if (rele[i].s != "")
      if (rele[i].b) {
        html = html + "<button id=""" + String(i) + """ class=""g"" onclick=""bb(this)"">" + rele[i].s + "</button>";
      } else {
        html = html + "<button id=""" + String(i) + """ class=""r"" onclick=""bb(this)"">" + rele[i].s + "</button>";
      }
  }
  html = html + "</div><script>function bb(element) {\n"
         "var xhr = new XMLHttpRequest();\n"
         "xhr.open(\"GET\", \"/sw/\"+element.id+\"/\"+((element.classList[0]==\"r\")?\"1\":\"0\").toString(), true);\n"
         "xhr.send();}\n";
  html = html + "</script></body>\n</html>\n";
  www.send(200, "text/html", html);
}
void wwwstatus() {
  String s;
  s = "{\"stat\":[";
  for (int i = 0; i < 12; i++) {
    if (rele[i].b)
      s += "1";
    else
      s += "0";
    if (i != 15)
      s += ",";
  }
  s += "],\"off\":[";
  for (int i = 0; i < 12; i++) {
    if (rele[i].b)
      s += rele[i].timeroff;
    else
      s += "0";
    if (i != 15)
      s += ",";
  }
  s += "],\"millis\": \"" + String(millis()) + "\"}";
  www.send(200, "application/json", s);
}
void wwwsw() {
  int t = -1;
  if (www.pathArg(1) == "0") {
    if (www.pathArg(0).toInt() == 13) {
      swrele(5, false);
    }
    if (www.pathArg(0).toInt() > 11) {
      swrele(8, false);
      swrele(9, false);
      swrele(10, false);
      swrele(11, false);
    } else
      swrele(www.pathArg(0).toInt(), false);
    if (www.pathArg(0).toInt() == 4) {
      servo.write(50);
      servoms = millis();
    }
  } else {
    if (www.pathArg(0).toInt() == 13) swrele(5, true);
    if (www.pathArg(0).toInt() > 11) {
      swrele(8, true);
      swrele(9, true);
      swrele(10, true);
      swrele(11, true);
    } else
      swrele(www.pathArg(0).toInt(), true);
    if (www.pathArg(1) == "1")  t = -1;
    if (www.pathArg(1) == "2")  t = millis() +  600000;
    if (www.pathArg(1) == "3")  t = millis() +  3600000;
    if (www.pathArg(0).toInt() == 13) rele[5].timeroff = t;
    if (www.pathArg(0).toInt() > 11) {
      rele[8].timeroff = t;
      rele[9].timeroff = t;
      rele[10].timeroff = t;
      rele[11].timeroff = t;
    } else
      rele[www.pathArg(0).toInt()].timeroff = t;
  }
  www.send(200, "text/plain", "" + www.pathArg(0) + "|" + www.pathArg(1));
}
void wwwon() {
  digitalWrite(www.pathArg(0).toInt(), HIGH);
  /*for (int i = 0; i < 12; i++) {
    swrele(i, true);
    }*/
  www.send(200, "text/plain", "1");
}
void wwwoff() {
  digitalWrite(www.pathArg(0).toInt(), LOW);
  /*for (int i = 0; i < 12; i++) {
    swrele(i, false);
    }*/
  www.send(200, "text/plain", "0");
}
void wwws() {
  String s;
  for (int i = 0; i < 12; i++) {
    s += "" + String(button[i].pin);
    if (button[i].state)
      s += "|1|" + String(button[i].click_ms) + "\n";
    else
      s += "|0|" + String(button[i].click_ms) + "\n";
  }
  //blazer();
  www.send(200, "text/plain", s);
}
void wwwservo() {
  servo.write(0);
  servoms = millis();
  www.send(200, "text/plain", "'");
}
void wwwStart() {
  www.on("/", wwwindex);
  www.on("/status", wwwstatus);
  www.on("/servo", wwwservo);
  www.on("/on/{}", wwwon);
  www.on("/off/{}", wwwoff);
  www.on("/s", wwws);
  www.on(UriBraces("/sw/{}/{}"), wwwsw);
  www.begin();
}
