/////////////////////////////////////////////////////////////////
#pragma once

#ifndef Button_h
#define Button_h

/////////////////////////////////////////////////////////////////

#include "Arduino.h"

/////////////////////////////////////////////////////////////////

class Button {
  protected:
    byte prev_state;
    byte pressed;
    unsigned long down_ms;
    //bool beep = false;

    typedef void (*CallbackFunction) (Button&);
    //typedef void (*Function) ();

    CallbackFunction pressed_cb = NULL;
    //Function beep_cb = NULL;
    CallbackFunction tap_cb = NULL;
    CallbackFunction long_cb = NULL;

  public:
    Button(byte attachTo, byte buttonMode = INPUT_PULLUP);
    byte pin;
    byte capa;
    byte state;
    long click_ms = 0;
    long prev_click = 0;

    void setPressedHandler(CallbackFunction f);
    //void setBeepHandler(Function f);
    void setTapHandler(CallbackFunction f);
    void setLongHandler(CallbackFunction f);

    bool operator==(Button &rhs);

    void loop();
};
/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
