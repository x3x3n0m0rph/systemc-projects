#pragma once
#include <systemc.h>

struct counter_b : public sc_module
{
    sc_in_clk clock;
    sc_in<bool> reset;
    sc_in<bool> input;
    sc_out<sc_uint<4>> out;

    sc_uint<4> count = 0;

    void process_clock();

    SC_CTOR(counter_b)
    {
        SC_METHOD(process_clock);
        //dont_initialize();
        sensitive << reset << clock.pos();
    }
};