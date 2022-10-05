long  lastms = 0;
void task_structure( void * parameter ) {
  while (true)
  {
    if (millis() < 1000)
      lastms = 0;
    if (millis() > lastms) {
      lastms = millis() + 1000;
      if (servoms != -1)
        if (millis() - 6000 > servoms) {
          servo.write(180);
          servoms = -1;
        }
      for (byte i = 0; i < 12; i++) {
        if (rele[i].timeroff != -1) {
          if ((i == 5) && ((rele[i].timeroff - lastms) > 29000) && ((rele[i].timeroff - lastms) < 30000)) blazer();
          if (rele[i].timeroff < lastms) {
            rele[i].timeroff = -1;
            if (rele[i].b) {
              if (i == 4) {
                servo.write(50);
                servoms = millis();
              }
              swrele(i, false);
            }
          }
        }
        if (rele[i].timeron != -1)
          if (rele[i].timeron < lastms) {
            rele[i].timeron = -1;
            if (!rele[i].b) {
              swrele(i, true);
            }
          }
      }
    }
    vTaskDelay(100 / portTICK_RATE_MS);
  }
}
