#include <windows.h>
#include "systemc.h"
#include "counter_b.h"
#include <debugapi.h>

SC_MODULE(counter_a)
{
    sc_in_clk clock;
    sc_in<bool> reset;
    sc_in<bool> input;
    sc_out<sc_uint<4>> out;

    sc_uint<4> count = 0;

    void process_clock()
    {
        if (reset.read())
        {
            count = 0;
            out.write(count);
        }
        else if (input.read())
        {
            count += 1;
            out.write(count);
        }
    }

    SC_CTOR(counter_a) : clock(), reset(), input(), out(), count()
    {
        SC_METHOD(process_clock);
        //dont_initialize();
        sensitive << reset << clock.pos();
    }
};

//struct counter_b : public sc_module
//{
//    sc_in_clk clock;
//    sc_in<bool> reset;
//    sc_in<bool> input;
//    sc_out<sc_uint<4>> out;
//
//    sc_uint<4> count = 0;
//
//    void process_clock()
//    {
//        if (reset.read())
//        {
//            count = 0;
//            out.write(count);
//        }
//        else if (input.read())
//        {
//            count += 1;
//            out.write(count);
//        }
//    }
//
//    //counter_b(const char* name) : sc_module(name) , clock(), reset(), input(), out(), count()
//    SC_CTOR(counter_b) : clock(), reset(), input(), out(), count()
//    {
//        SC_METHOD( process_clock );
//        //dont_initialize();
//        sensitive << reset << clock.pos();
//    }
//};

struct stage1 : sc_module {
    sc_in<double> in1;   //input 1
    sc_in<double> in2;   //input 2
    sc_out<double> sum;  //output 1
    sc_out<double> diff; //output 2
    sc_in<bool>    clk;  //clock

    void addsub()
    {
        double a;
        double b;

        a = in1.read();
        b = in2.read();
        sum.write(a + b);
        diff.write(a - b);

    }

    //Counstructor
    SC_CTOR(stage1) {
        SC_METHOD(addsub);     //Declare addsub as SC_METHOD and  
        dont_initialize();
        sensitive << clk.pos();  //make it sensitive to positive clock edge
    }

};

int sc_main(int argc, char* argv[]) {
    sc_signal<bool> clock;
    sc_signal<bool> reset;
    sc_signal<bool> input;
    sc_signal<sc_uint<4>> out;

    counter_b counter("counter");
    counter.clock(clock);
    counter.reset(reset);
    counter.input(input);
    counter.out(out);

    sc_start();
    sc_trace_file* trace_file = sc_create_vcd_trace_file("trace");

    sc_trace(trace_file, clock, "clock");
    sc_trace(trace_file, reset, "reset");
    sc_trace(trace_file, input, "input");
    sc_trace(trace_file, out, "out");

    reset = false;
    clock = false;
    input = false;

    for (size_t i = 0; i < 2; i++)
    {
        clock = false;
        sc_start(10, SC_NS);
        clock = true;
        sc_start(10, SC_NS);
    }

    input = true;
    for (size_t i = 0; i < 16; i++)
    {
        clock = false;
        sc_start(10, SC_NS);
        clock = true;
        sc_start(10, SC_NS);
    }

    input = false;
    reset = true;
    sc_start(10, SC_NS);

    reset = false;
    input = true;
    for (size_t i = 0; i < 16; i++)
    {
        clock = false;
        sc_start(10, SC_NS);
        clock = true;
        sc_start(10, SC_NS);
    }

    sc_close_vcd_trace_file(trace_file);
    return 0;
}

//int sc_main(int argc, char* argv[]) 
//{
//    sc_signal<double> in1;
//    sc_signal<double> in2;
//    sc_signal<double> sum;
//    sc_signal<double> diff;
//
//    sc_signal<bool> clock;
//
//    stage1 counter("counter");
//    counter(in1, in2, sum, diff, clock);
//
//    sc_start();
//    //sc_trace_file* trace_file = sc_create_vcd_trace_file("trace.vcd");
//
//    //sc_trace(trace_file, clock, "clock");
//    //sc_trace(trace_file, in1, "in1");
//    //sc_trace(trace_file, in2, "in2");
//    //sc_trace(trace_file, sum, "sum");
//    //sc_trace(trace_file, diff, "diff");
//
//    for (size_t i = 0; i < 16; i++)
//    {
//        clock = false;
//        sc_start(10, SC_NS);
//        clock = true;
//        sc_start(10, SC_NS);
//    }
//
//    //sc_close_vcd_trace_file(trace_file);
//    return 0;
//}