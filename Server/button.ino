void clickLong(Button& btn) {
  if (btn == button[10]) {//зал
    if (rele[5].b) {
      swrele(7, true);
      rele[5].timeroff = -1;
      rele[8].timeroff = -1;
      rele[9].timeroff = -1;
      rele[10].timeroff = -1;
      rele[11].timeroff = -1;
    } else {
      swrele(3, false);
      swrele(7, false);
      swrele(5, false);
      swrele(8, false);
      swrele(9, false);
      swrele(10, false);
      swrele(11, false);
    }
  }
  if (btn == button[11]) {//кухня ванная
    if (rele[5].b) {
      rele[5].timeroff = -1;
    }
  }
}
void clickup(Button& btn) {
  //unsigned int time = btn.wasPressedFor();
  if (btn == button[10]) {//зал
    if (!rele[5].b) {
      swrele(5, true);
      swrele(8, true);
      swrele(9, true);
      swrele(10, true);
      swrele(11, true);
      rele[5].timeroff = millis() + 600000;
      rele[8].timeroff = millis() + 600000;
      rele[9].timeroff = millis() + 600000;
      rele[10].timeroff = millis() + 600000;
      rele[11].timeroff = millis() + 600000;
    } else {
      swrele(5, false);
      swrele(8, false);
      swrele(9, false);
      swrele(10, false);
      swrele(11, false);
    }
  }
  if (btn == button[11]) {//кухня ванная
    if (!rele[5].b) {
      rele[5].timeroff = millis() + 600000;
      swrele(5, true);
      swrele(3, false);
    } else if ((rele[5].timeroff - millis()) < 30000) {
      rele[5].timeroff = millis() + 600000;
    } else {
      swrele(5, false);
      swrele(3, true);
    }
  }
  if (btn == button[9]) {//спальня
    swrele(1, !rele[1].b);
  }
  if (btn == button[0]) { //4//8//туалет кнопка
    servo.write(50);
    servoms = millis();
  }
  if (btn == button[7]) {//туалет
    swrele(4, true);
    rele[4].timeroff = millis() + 60000;
  }
  if (btn == button[6]) {//корридор
    swrele(2, true);
    rele[2].timeroff = millis() + 20000;
  }
}
void task_button( void * parameter ) {
  while (true)
  {
    if (beep) {
      if (millis() > beepms) {
        ledcWrite(10, 0);
        beep = false;
        beepms = -1;
      }
    }
    for (int i = 0; i < 12; i++) {
      button[i].loop();
    }
    vTaskDelay(1 / portTICK_RATE_MS);
  }
}
