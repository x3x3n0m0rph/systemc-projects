#pragma once
#include <systemc.h>
#include "../SystemC JK-Trigger/JKTriggerSync.h"

SC_MODULE(TwoBitSyncCounter)
{
    //get in ...
    sc_in_clk CLK_pin;

    //and get out!
    //sc_out<sc_uint<2>> OUT_set;
    //sc_out<sc_uint<2>> ROUT_set;
    // 
    sc_out<bool> Q0_pin;
    sc_out<bool> Q1_pin;

    //inside
    JKTriggerSync c0, c1;

    //signals
    sc_signal<bool> JK_CLK_sig;
    sc_signal<bool> JK_HIGH_sig;

    sc_signal<bool> JK0_Q_sig;
    sc_signal<bool> JK1_Q_sig;

    //nc
    sc_signal<bool> JK0_NQ_sig;
    sc_signal<bool> JK1_NQ_sig;

    void clock() {
        JK_CLK_sig.write(CLK_pin.read());

        //reading state
        //sc_uint<2> counterState = (JK0_Q_sig.read() << 1) + JK1_Q_sig.read();
        //sc_uint<2> counterRState = (JK1_Q_sig.read() << 1) + JK0_Q_sig.read();
        //OUT_set.write(counterState);
        //ROUT_set.write(counterRState);

        Q0_pin.write(JK0_Q_sig.read());
        Q1_pin.write(JK1_Q_sig.read());
    }

    SC_CTOR(TwoBitSyncCounter) : c0("bit0"), c1("bit1")
    {

        //bind signals to pins
        //triggers clock pins both attached to CLK signal
        c0.CLK_pin(JK_CLK_sig);
        c0.J_pin(JK_HIGH_sig);
        c0.K_pin(JK_HIGH_sig);

        c0.Q_pin(JK0_Q_sig);
        c0.NQ_pin(JK0_NQ_sig);

        c1.CLK_pin(JK_CLK_sig);
        c1.J_pin(JK0_Q_sig);
        c1.K_pin(JK0_Q_sig);

        c1.Q_pin(JK1_Q_sig);
        c1.NQ_pin(JK1_NQ_sig);

        JK_HIGH_sig.write(true);

        SC_METHOD(clock);
        sensitive << CLK_pin;

    }
};
