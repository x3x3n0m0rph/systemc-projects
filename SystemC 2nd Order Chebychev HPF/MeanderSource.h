#pragma once
#include <systemc-ams.h>

SCA_TDF_MODULE(MeanderSource)
{
    sca_tdf::sca_out<double> out;

    double amplitude;
    double frequency;

    void set_attributes()
    {
        out.set_timestep(1.0, SC_US);
    }

    void processing()
    {
        double t = out.get_time().to_seconds();
        double x = amplitude * std::signbit(std::sin(2.0 * M_PI * frequency * t));
        out.write(x);
    }

    SCA_CTOR(MeanderSource) :
        out("out"), amplitude(1.0), frequency(1e3) {}
};
