#include <windows.h>
#include <debugapi.h>
#include "systemc.h"
#include "TwoBitCounterSync.h"
#include "../SystemC JK-Trigger/SignalControl.cpp" //no way

int sc_main(int argc, char* argv[]) {
    sc_signal<bool>         CTR_CLK_sig;
    sc_signal<bool>         Q0_sig;
    sc_signal<bool>         Q1_sig;
    //sc_signal<sc_uint<2>>   CTR_OUT_sig;
    //sc_signal<sc_uint<2>>   CTR_ROUT_sig;

    TwoBitSyncCounter counter("counter");

    counter.CLK_pin(CTR_CLK_sig);
    counter.Q0_pin(Q0_sig);
    counter.Q1_pin(Q1_sig);
    //counter.OUT_set(CTR_OUT_sig);
    //counter.ROUT_set(CTR_ROUT_sig);

    SignalControl clk_ctl(CTR_CLK_sig, CTR_CLK_sig.read(), 2, 2);

    sc_start();
    sc_trace_file* trace_file = sc_create_vcd_trace_file("TwoBitCounter");

    sc_trace(trace_file, CTR_CLK_sig, "clock");
    sc_trace(trace_file, Q0_sig, "q0");
    sc_trace(trace_file, Q1_sig, "q1");

    CTR_CLK_sig = true;
    for (size_t i = 0; i < 100; i++)
    {
        clk_ctl.stage(i);
        sc_start(1, SC_PS);
    }

    sc_close_vcd_trace_file(trace_file);

    return 0;
}