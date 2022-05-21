#pragma once
#include <systemc.h>

SC_MODULE(JKPCTriggerSync)
{
    //get in ...
    sc_in_clk CLK_pin;
    sc_in<bool> J_pin;
    sc_in<bool> K_pin;

    sc_in<bool> CLR_pin;
    sc_in<bool> PRE_pin;

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

    //clear internal state 
    void clr() {
        if (CLR_pin.read() xor PRE_pin.read()) {
            if (CLR_pin.read()) state = false;
            if (PRE_pin.read()) state = true;
        }
    }

    SC_CTOR(JKPCTriggerSync) : J_pin(), K_pin(), Q_pin(), NQ_pin(), CLR_pin(), PRE_pin()
    {

        SC_METHOD(clock);
        dont_initialize();
        sensitive << CLK_pin.pos();

        SC_METHOD(clr);
        sensitive << CLR_pin.pos() << PRE_pin.pos();
    }
};