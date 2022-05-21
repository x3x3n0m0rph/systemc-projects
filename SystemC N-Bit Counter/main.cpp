#include <windows.h>
#include <debugapi.h>
#include "systemc.h"
#include "NBitCounterSync.h"
#include "../SystemC JK-Trigger/SignalControl.cpp"

#define WORD_LEN 4


int sc_main(int argc, char* argv[]) {
    sc_signal<bool> clk;
    sc_signal<bool> clr;
    sc_signal<sc_uint<WORD_LEN>> q;

    NBitSyncCounter<WORD_LEN> counter("nbittrigger");

    counter.CLK_pin(clk);
    counter.CLR_pin(clr);
    counter.Q_set(q);

    clk = false;
    clr = true;

    SignalControl clk_ctl(clk, clk.read(), 1, 1, 10);
    SignalControl clr_ctl(clr, clr.read(), 3, 10, 0);

    sc_start();
    sc_trace_file* trace_file = sc_create_vcd_trace_file("nbittrigger");

    sc_trace(trace_file, clk, "clock");
    sc_trace(trace_file, clr, "clr");
    sc_trace(trace_file, q, "q");

    for (size_t i = 0; i < 200; i++)
    {
        if (i < 10) clr_ctl.stage(i);

        clk_ctl.stage(i);
        sc_start(1, SC_PS);
    }

    sc_close_vcd_trace_file(trace_file);

    return 0;
}
