#include <windows.h>
#include <debugapi.h>
#include <iostream>
#include <systemc-ams>

#include "SinSource.h"
#include "MeanderSource.h"
#include "SecondOrderChebychevHPF.h"

int sc_main(int argc, char* argv[]) {

        sca_tdf::sca_signal<double> input;
        sca_tdf::sca_signal<double> output;


        //MeanderSource sinus("Sine");
        SinSource sinus("Sine");
        sinus.out(input);
        sinus.amplitude = 10.0;
        

        SecondOrderChebychevHPF filter("filter");
        filter.input(input);
        filter.output(output);

        for (double freq = 100.0; freq <= 100000.0; freq += std::pow(10, (int)std::log10(freq) - 1)) {
            std::string filename = "trace_s\\freq_";
            filename += std::to_string((long)freq);

            sca_trace_file* trace_file = sca_create_vcd_trace_file(filename.c_str());
            sinus.frequency = freq;

            sca_trace(trace_file, input, "input");
            sca_trace(trace_file, output, "output");

            std::cout << "Sim start at " << freq << " Hz" << std::endl;
            sc_start(10.0 / freq, SC_SEC); // ten periods of modulation for each freq
            std::cout << "Sim finish " << freq << " Hz" << std::endl;

            sca_close_vcd_trace_file(trace_file);
        }

    return 0;
}