/////////////////////////////////////////////////////////////////
/*
  Button.cpp - Arduino Library to simplify working with buttons.
  Created by Lennart Hennigs.
*/
/////////////////////////////////////////////////////////////////

#include "Arduino.h"
#include "Button.h"

/////////////////////////////////////////////////////////////////

Button::Button(byte attachTo, byte buttonMode) {
  pin = attachTo;
  //setDebounceTime(50);

  pinMode(attachTo, buttonMode);
  //if ((pin == 0) || (pin == 2) || (pin == 4) || (pin == 15) || (pin == 12) || (pin == 13) || (pin == 14) || (pin == 27) || (pin == 33) || (pin == 32)) {
  /*if ( (pin == 32)) {
    capa = touchRead(pin);
    state = capa <  CAPACITIVE_TOUCH_THRESHOLD ? LOW : HIGH;
    } else {
    state = digitalRead(pin);
    }/**/
  state = digitalRead(pin);
}

/////////////////////////////////////////////////////////////////

bool Button::operator==(Button &rhs) {
  return (this == &rhs);
}

/////////////////////////////////////////////////////////////////

void Button::setPressedHandler(CallbackFunction f) {
  pressed_cb = f;
}

/////////////////////////////////////////////////////////////////

void Button::setTapHandler(CallbackFunction f) {
  tap_cb = f;
}

/////////////////////////////////////////////////////////////////

void Button::setLongHandler(CallbackFunction f) {
  long_cb = f;
}

/////////////////////////////////////////////////////////////////

/*void Button::setBeepHandler(Function f) {
  beep_cb = f;
}/**/

/////////////////////////////////////////////////////////////////


void Button::loop() {
  prev_state = state;

  capa = digitalRead(pin);
  if (prev_state != (capa)) {
    if (down_ms == 0) {
      down_ms = millis();
    } else if (millis() - down_ms > 50) {
      down_ms = 0;
      state = capa ;
    }
  } else
    down_ms = 0;
  /*if (beep && state == pressed) {
    if (millis() - prev_click > 2000) {
      //beep_cb();
      beep = false;
    }
  }/**/
  if (prev_state != state)
    if (state == pressed) {
      //beep = true;
      prev_click = millis();
      if (tap_cb != NULL) tap_cb (*this);
      if (pressed_cb != NULL) pressed_cb (*this);
    } else {
      //beep = false;
      click_ms = millis() - prev_click;
      if (tap_cb != NULL) tap_cb (*this);
      if (click_ms > 2000) long_cb (*this);
    }
}

/////////////////////////////////////////////////////////////////
