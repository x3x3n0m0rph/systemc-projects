#include <windows.h>
#include <debugapi.h>
#include "systemc.h"
#include "JKTriggerSync.h"
#include "SignalControl.h"

int sc_main(int argc, char* argv[]) {
    sc_signal<bool> clock;
    sc_signal<bool> j;
    sc_signal<bool> k;
    sc_signal<bool> q;
    sc_signal<bool> nq;

    JKTriggerSync trigger("trigger");

    trigger.CLK_pin(clock);
    trigger.J_pin(j);
    trigger.K_pin(k);
    trigger.Q_pin(q);
    trigger.NQ_pin(nq);


    SignalControl j_ctl(j, j.read(), 5, 20, 2);
    SignalControl k_ctl(k, k.read(), 5, 20, 12);
    SignalControl clk_ctl(clock, clock.read(),3, 3);


    sc_start();
    sc_trace_file* trace_file = sc_create_vcd_trace_file("jk-trigger");

    sc_trace(trace_file, clock, "clock");
    sc_trace(trace_file, j, "j");
    sc_trace(trace_file, k, "k");
    sc_trace(trace_file, q, "q");
    sc_trace(trace_file, nq, "nq");

    clock = true;
    for (size_t i = 0; i < 200; i++)
    {
        j_ctl.stage(i);
        k_ctl.stage(i);
        clk_ctl.stage(i);
        sc_start(1, SC_PS);
    }

    sc_close_vcd_trace_file(trace_file);

    return 0;
}
