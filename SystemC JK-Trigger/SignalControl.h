#pragma once

#include "systemc.h"

class SignalControl {
private:
    sc_signal<bool>& _sig;
    bool _sigState;
    int _wavelength;
    int _delay;
    int _interval;

    void setState(bool state);

public:
    SignalControl() = delete;
    SignalControl(sc_signal<bool>& sig, bool sigState, int wavelength, int interval, int delay = 0);
    void stage(int step);

};