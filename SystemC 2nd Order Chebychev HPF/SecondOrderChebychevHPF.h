#pragma once

#include <systemc.h>
#include <systemc-ams>

using namespace std;
//using namespace sc_core;


SC_MODULE(SecondOrderChebychevHPF)
{
	/* TDF I/O ports for data to be read in/out from TDF modules*/
	sca_tdf::sca_in<double> input;
	sca_tdf::sca_out<double> output;

	/* TDF-ELN, ELN-TDF converter voltage sources */
	sca_eln::sca_tdf::sca_vsource v_in;
	sca_eln::sca_tdf::sca_vsink v_out;
	//sca_eln::sca_vsource v_5v;
	sca_eln::sca_vsource v_offset;

	/* Ideal OP-AMPS */
	sca_eln::sca_nullor nullor;

	/* Capacitors */
	sca_eln::sca_c c1, c2;//, c_bypass;

	/* Resistors */
	//vector<sca_eln::sca_r> r = vector<sca_eln::sca_r>(5);
	sca_eln::sca_r r1, r2;

	/* Electrical netlist nodes */
	vector<sca_eln::sca_node> n = vector<sca_eln::sca_node>(10);

	/* Ground node*/
	sca_eln::sca_node_ref gnd;

	/* Set I/O port parameters */
	void set_attributes()
	{
		input.set_rate(1);
		output.set_timestep(1.0, SC_US);
		output.set_rate(1);
	}

	/*Constructor */
	SC_CTOR(SecondOrderChebychevHPF) : 
		v_in("v_in", 1.0), 
		v_out("v_out", 1.0),

		//v_5v("v_5v", 0, 5.0),
		v_offset("v_offset", 0.0, 2.5),

		nullor("nullor"),
		c1("c1", 68.0E-9),
		c2("c2", 10.0E-9),
		//c_bypass("c_bypass", 10.0E-9),

		r1("r1", 19600.0),
		r2("r2", 2940.0)
	{
		v_in.n(gnd);
		//v_5v.n(gnd);
		v_offset.n(gnd);
		v_out.n(gnd);

		v_in.inp(input);
		v_in.p(n.at(0));

		c1.p(n.at(0));
		c1.n(n.at(1));

		c2.p(n.at(1));
		c2.n(n.at(2));

		//c_bypass.p(n.at(5));
		//c_bypass.n(gnd);

		r1.p(n.at(2));
		r1.n(n.at(4));

		r2.p(n.at(1));
		r2.n(n.at(3));

		v_out.p(n.at(3));
		v_out.outp(output);
		//v_5v.p(n.at(5));
		v_offset.p(n.at(4));


		nullor.nip(n.at(2));
		nullor.nin(n.at(3));
		nullor.nop(n.at(3));
		nullor.non(gnd);


	}

	~SecondOrderChebychevHPF() {}
};
