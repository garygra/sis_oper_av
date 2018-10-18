#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <queue> 
#include <random>
#include <fstream>

#include "input_output.hpp"
#include "random.hpp"
#include "param_reader.hpp"
#include "processor.hpp"

using namespace std;

int main(int argc, char *argv[])
{
	
	read_parameter(argc, argv);

	if (params::debug_level > 1)
	{
		cout << "Using the following parameters:" << endl;
		cout << "Debugging: " << endl;
		cout << "\tdebug_level = " << params::debug_level << endl;
		cout << "\tprint_files = " << params::print_files << endl;
		cout << "\nGlobal: " << endl;
		cout << "\titerations = " << params::iterations << endl;
		cout << "\tseed = " << params::seed << endl;
		cout << "\nProcessor: " << endl;
		cout << "\tproc_window = " << params::proc_window << endl;
		cout << "\tmin_cycles_per_proc = " << params::min_cycles_per_proc << endl;
		cout << "\tmax_cycles_per_proc = " << params::max_cycles_per_proc << endl;
		cout << "\tnew_proc_prob = " << params::new_proc_prob << endl;
		cout << "\nI/Os: " << endl;
		cout << "\ttotal_io = " << params::total_io << endl;
		cout << "\tpacket_arrival_mean = " << params::packet_arrival_mean << endl;
		cout << "\tpacket_len_mean = " << params::packet_len_mean << endl;
		cout << "\tpacket_len_stddev = " << params::packet_len_stddev << endl;
		cout << "\tinterrupt_queue_size = " << params::interrupt_queue_size << endl;
	}

	if (params::debug_level > 0)
	{
		cout << "\nInitializing simulation..." << endl;
	}

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
	proc.set_max_queue_size(params::interrupt_queue_size);
	proc.set_new_process_prob(params::new_proc_prob);
	proc.set_min_max_proc_cycles(params::min_cycles_per_proc, params::max_cycles_per_proc);
	proc.set_proc_window(params::proc_window);

	if (params::debug_level > 0)
	{
		cout << "\nRunning simulation..." << endl;
	}

	for (int i = 0; i < params::iterations; ++i)
	{
		for (int j = 0; j < params::total_io; ++j)
		{
			ios[j].iterate();
			if (ios[j].raised_interruption())
			{
				// cout << "interruption raised" << endl;
				proc.set_new_interrupts(ios[j].get_next_packet());
			}
		}
		proc.round_robin();
	}

	proc.print_metrics();

	if (params::debug_level > 0)
	{
		cout << "\nSimulation finished..." << endl;
		cout << "\nPrinting results:" << endl;
		cout << "processor_H: ";
		proc.print_metrics_header();
		cout << "processor: ";
		proc.print_metrics();
		cout << endl;
		cout << "IO_H: ";
		ios[0].print_metrics_header();

		for (int i = 0; i < params::total_io; ++i)
		{
			cout << "IO" << i << ": ";
			ios[i].print_metrics();
			cout << "\t";
		}
		cout << endl;
	}


	if (params::print_files)
	{
		// output to file
		cout << "printing processor file..." << endl;
		ofstream myfile;
		stringstream ss;
		ss << "../data/proc_" << params::iterations << "_" << params::proc_window << "_" << params::min_cycles_per_proc << "_" << 
			params::max_cycles_per_proc << "_" << params::new_proc_prob << "_" << params::total_io << "_" << 
			params::packet_arrival_mean[0] << "_" << params::packet_len_mean[0] << "_" << params::packet_len_stddev[0] << ".txt" ; 
  		myfile.open (ss.str());
  		myfile << proc.metrics_tostring();
  		myfile.close();
  	
  		// cout << ios[0].print_metrics(); 
  		istringstream* iss = new istringstream[params::total_io];
  		for (int i = 0; i < params::total_io; ++i)
  		{
  			iss[i].str(ios[i].metrics_tostring());
  		}

  		bool end = true;
  		stringstream ss2;
  		string line;
		cout << "printing i/o's file..." << endl;
  		while (end)
  		{
  			end = false;
  			for (int i = 0; i < params::total_io; ++i)
  			{
  				if (getline(iss[i], line))
  				{
  					ss2 << line << " ";
  					end = true;
  				}
  			}
  			ss2 << endl;
  		}

	  	// cout << ss2.str();
		ss.str("");
		ss.clear(); 
		ss << "../data/ios_" << params::iterations << "_" << params::proc_window << "_" << params::min_cycles_per_proc << "_" << 
			params::max_cycles_per_proc << "_" << params::new_proc_prob << "_" << params::total_io << "_" << 
			params::packet_arrival_mean[0] << "_" << params::packet_len_mean[0] << "_" << params::packet_len_stddev[0] << ".txt" ; 
	  	myfile.open (ss.str());
	  	myfile << ss2.str();
	  	myfile.close();
  	}
}

