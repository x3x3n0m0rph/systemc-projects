#pragma once
#include <systemc.h>
#include <array>
#include <list>
#include "JKPCTriggerSync.h"

template<size_t width>
SC_MODULE(NBitSyncCounter)
{
    //get in ...
    sc_in_clk CLK_pin;                              //clocking pin
    sc_in<bool> CLR_pin;                            //state clearing pin

    //and get out!
    sc_out<sc_uint<width>> Q_set;                   //outcoming port for module state (flipflops' Q)

    //inside
    std::list<JKPCTriggerSync> JK_bits;             //list of flipflops
    sc_uint<width> state;                           //internal state of flipflops' Q

    //signals
    sc_signal<bool> JK_HIGH_sig;                    //pull up for J, K, PRE pins of all flipflops
    sc_signal<bool> CLR_sig;                        //bus signal for clearing pin of all flipflops
    sc_signal<bool> CLK_sig;                        //signal for clock pin of first flipflop

    std::array<sc_signal<bool>, width> JK_Qn_sig;   //array of connections between flipflops (Q of i-1 to CLK of i)

    //nc
    std::array<sc_signal<bool>, width> JK_NQn_sig;  //not connected, just for integrity

    void clock() {
        //forwarding CLK from pin to connection with first flipflop
        //this call clock() in all flipflops in module
        CLK_sig.write(CLK_pin.read());

        //recalculation of state value
        state = 0;
        size_t bit_i = 0;
        for (auto iter = JK_Qn_sig.rbegin(); iter != JK_Qn_sig.rend(); iter++) {
            state += iter->read();
            bit_i++;
            if (bit_i < width) state <<= 1;
        }

        //send module state to outcoming pin
        Q_set.write(state);
    }

    void clr() {
        //forwarding CLR from pin to bus 
        CLR_sig.write(CLR_pin.read());
    }

    SC_CTOR(NBitSyncCounter) {

        //bind signals to pins
        
        for (size_t i = 0; i < width; i++)
        {
            //constructing new flipflop in list
            JK_bits.emplace_back(sc_module_name(std::string(std::string("bit") + std::to_string(i)).c_str()));

            //connecting it to buses
            JK_bits.back().CLR_pin(CLR_sig);
            JK_bits.back().PRE_pin(JK_HIGH_sig);
            JK_bits.back().J_pin(JK_HIGH_sig);
            JK_bits.back().K_pin(JK_HIGH_sig);

            //connecting it to it's output signals
            JK_bits.back().Q_pin(JK_Qn_sig[i]);
            JK_bits.back().NQ_pin(JK_NQn_sig[i]);

            //connecting it to previous flipflop
            //first ff connected to clock pin of module
            if (i == 0) JK_bits.back().CLK_pin(CLK_sig);
            else JK_bits.back().CLK_pin(JK_Qn_sig[i - 1]);
        }

        //pull up HIGH bus
        JK_HIGH_sig.write(true);

        SC_METHOD(clock);
        dont_initialize();
        sensitive << CLK_pin;

        SC_METHOD(clr);
        sensitive << CLR_pin.pos();      
    }
};
