#include "SignalControl.h"
void SignalControl::setState(bool state) {
    if (state != _sigState) {
        _sig.write(state);
        _sigState = state;
    }
}

SignalControl::SignalControl(sc_signal<bool>& sig, bool sigState, int wavelength, int interval, int delay)
    : _sig(sig), _sigState(sigState), _wavelength(wavelength), _delay(delay), _interval(interval)
{
}

void SignalControl::stage(int step)
{
    if (step < _delay) return;
    int realstep = step - _delay;
    int periodLen = _wavelength + _interval;

    int periodIndex = realstep % periodLen;
    if (periodIndex < _wavelength) {
        setState(true);
    }
    else {
        setState(false);
    }
    
}
