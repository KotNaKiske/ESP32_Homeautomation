void clickLong(Button& btn) {
  if (btn == button[6]) {  //зал
    if (rele[7].b) {
      swrele(12, true);
      rele[2].off = -1;
      swrele(7, false);
      swrele(8, false);
      swrele(9, false);
      swrele(10, false);
    }
  }
  if (btn == button[5]) {  //кухня ванная
    if (rele[2].b) {
      rele[2].off = -1;
      blazer();
    }
  }
}
void clickup(Button& btn) {
  if (btn == button[6]) {  //зал
    if (!rele[2].b) {
      swrele(2, true);
      swrele(7, true);
      swrele(8, true);
      swrele(9, true);
      swrele(10, true);
      rele[2].off = millis() + 600000;
      rele[7].off = millis() + 600000;
      rele[8].off = millis() + 600000;
      rele[9].off = millis() + 600000;
      rele[10].off = millis() + 600000;
    } else {
      swrele(2, false);
      swrele(7, false);
      swrele(8, false);
      swrele(9, false);
      swrele(10, false);
    }
  }
  if (btn == button[5]) {  //кухня ванная
    if (!rele[2].b) {
      rele[2].off = millis() + 600000;
      swrele(2, true);
      swrele(1, false);
    } else if ((rele[2].off - millis()) < 30000) {
      rele[2].off = millis() + 600000;
    } else {
      swrele(2, false);
      swrele(1, true);
    }
  }
  if (btn == button[4]) {  //спальня
    swrele(4, !rele[4].b);
  }
  if (btn == button[8]) {  //4//8//туалет кнопка
    servo.servo.write(servo.on);
    servo.servoms = millis();
  }
  if (btn == button[12]) {  //туалет
    swrele(0, true);
    rele[0].off = millis() + 60000;
  }
  if (btn == button[13]) {  //корридор
    swrele(3, true);
    rele[3].off = millis() + 20000;
  }
  if (btn == button[16]) {  //Кабинет верх Жалюзи
    if (HTTPClient.connect("orangepi5", 80)) {
      HTTPClient.println("GET /1.php HTTP/1.1");
      HTTPClient.println("Host: orangepi5");
      HTTPClient.println("Connection: close");
      HTTPClient.println();
      HTTPClient.stop();
    }
  }
  if (btn == button[15]) {  //Кабинет Лево 3 светодиода
    au[0] = !au[0];
    httpsend();
  }
  if (btn == button[17]) {  //Кабинет право люстра
    //au[1] = !au[1];
    Serial.print("B");
    Serial.println((au[1] ? '1' : '0'));
    blesend((au[1] ? 1 : 0));
  }
}
void task_button(void* parameter) {
  while (true) {
    if (beep.beep) {
      if (millis() > beep.beepms) {
        ledcWriteTone(beep.channel, 0);
        //ledcWrite(10, 0);
        beep.beep = false;
        beep.beepms = -1;
      }
    }
    for (int i = 0; i < lenbutton; i++) {
      button[i].loop();
    }
    vTaskDelay(1 / portTICK_RATE_MS);
  }
}
