#include <windows.h>
#include <debugapi.h>
#include <iostream>
#include <systemc-ams>

#include "SinSource.h"
#include "SecondOrderChebychevHPF.h"

int sc_main(int argc, char* argv[]) {
    sca_tdf::sca_signal<double> input;
    sca_tdf::sca_signal<double> output;

    sca_trace_file* trace_file = sca_create_tabular_trace_file("Chebychev 2nd Order");



    SinSource sinus("Sine");
    sinus.out(input);
    sinus.amplitude = 10.0;
    sinus.frequency = 500.0;

    SecondOrderChebychevHPF filter("filter");
    filter.input(input);
    filter.output(output);

    sca_trace(trace_file, input, "input");
    sca_trace(trace_file, output, "output");

    std::cout << "Sim start" << std::endl;
    sc_start(20.0, SC_MS);
    sc_stop();
    std::cout << "Sim finish" << std::endl;

    sca_close_tabular_trace_file(trace_file);

    return 0;
}