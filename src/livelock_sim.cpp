#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <queue> 
#include <random>

#include "input_output.hpp"
#include "random.hpp"
#include "param_reader.hpp"
#include "processor.hpp"

using namespace std;

int main(int argc, char *argv[])
{
	
	read_parameter(argc, argv);
	cout << "Using the following parameters:" << endl;
	cout << "\titerations = " << params::iterations << endl;
	cout << "\tseed = " << params::seed << endl;
	cout << "\tproc_window = " << params::proc_window << endl;
	cout << "\tmin_cycles_per_proc = " << params::min_cycles_per_proc << endl;
	cout << "\tmax_cycles_per_proc = " << params::max_cycles_per_proc << endl;
	cout << "\tnew_proc_prob = " << params::new_proc_prob << endl;
	cout << "\ttotal_io = " << params::total_io << endl;
	cout << "\tpacket_arrival_mean = " << params::packet_arrival_mean << endl;
	cout << "\tpacket_len_mean = " << params::packet_len_mean << endl;
	cout << "\tpacket_len_stddev = " << params::packet_len_stddev << endl;

	// init the i/o's
	input_output* ios = new input_output[params::total_io];
	for (int i = 0; i < params::total_io; ++i)
	{
		ios[i].set_seed(params::seed);
		ios[i].set_arrival_mean(params::packet_arrival_mean[i]);
		ios[i].set_packet_params(params::packet_len_mean[i], params::packet_len_stddev[i]);
	}

	// init processor
	processor proc = processor();

	proc.set_seed(params::seed);
	proc.set_new_process_prob(params::new_proc_prob);
	proc.set_min_max_proc_cycles(params::min_cycles_per_proc, params::max_cycles_per_proc);
	proc.set_proc_window(params::proc_window);

	for (int i = 0; i < params::iterations; ++i)
	{
		for (int j = 0; j < params::total_io; ++j)
		{
			ios[j].iterate();
			if (ios[j].raised_interruption())
			{
				cout << "interruption raised" << endl;
				proc.set_new_interrupts(ios[j].get_next_packet());
			}
		}
		proc.round_robin();
	}

	cout << proc.print_metrics();
}

