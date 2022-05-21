#pragma once
#include <systemc.h>

SC_MODULE(JKTriggerSync)
{
    //get in ...
    sc_in_clk CLK_pin;
    sc_in<bool> J_pin;
    sc_in<bool> K_pin;

    //and get out!
    sc_out<bool> Q_pin;
    sc_out<bool> NQ_pin;

    //inside
    bool state = false;

    void clock() {
        Q_pin.write(state);
        NQ_pin.write(!state);

        uint8_t pin_state = 0b0, j_state = 0b0, k_state = 0b0;

        k_state = K_pin.read();
        j_state = J_pin.read();
        pin_state = k_state + (j_state << 1);

        switch (pin_state) {
            case 0b00:

                break;
            case 0b01:
                state = false;
                break;
            case 0b10:
                state = true;
                break;
            case 0b11:
                state = !state;
                break;
        }
    }


    SC_CTOR(JKTriggerSync) : J_pin(), K_pin(), Q_pin(), NQ_pin()
    {
        SC_METHOD(clock);
        sensitive << CLK_pin.neg();
    }
};