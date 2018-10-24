#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <queue> 
#include <random>
#include <fstream>
#include <iterator>
#include <algorithm>


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
		cout << "\tinterrupt_queue_size = " << params::interrupt_queue_size << endl;
		cout << "\tdeliver_interrupt_mean = " << params::deliver_interrupt_mean << endl;
		cout << "\tquota = " << params::quota_limit << endl;
		cout << "\nI/Os: " << endl;
		cout << "\ttotal_io = " << params::total_io << endl;
		cout << "\tpacket_arrival_mean = ";
			std::copy(params::packet_arrival_mean, params::packet_arrival_mean + params::total_io, std::ostream_iterator<double>(std::cout, " "));
			cout << endl;
			// params::packet_arrival_mean << endl;
		cout << "\tpacket_len_mean = ";
			std::copy(params::packet_len_mean, params::packet_len_mean + params::total_io, std::ostream_iterator<double>(std::cout, " "));
			cout << endl;
		cout << "\tpacket_len_stddev = " ;
			std::copy(params::packet_len_stddev, params::packet_len_stddev + params::total_io, std::ostream_iterator<double>(std::cout, " "));
			cout << endl;
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
		ios[i].set_max_queue_size(params::interrupt_queue_size);
	}

	// init processor
	processor proc = processor();

	proc.set_seed(params::seed);
	proc.set_total_number_interrupts(params::total_io);
	proc.set_quota_limit(params::quota_limit);
	proc.set_max_queue_size(params::interrupt_queue_size);
	proc.set_arrival_mean(params::deliver_interrupt_mean);
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
				proc.set_new_interrupts(ios[j].get_next_packet(), j);
			}
		}
		if(params::algorithm == 0)
		{
			proc.round_robin();
		}
		else if(params::algorithm == 1)
		{
			proc.mogul_algorithm();
		} 
		else
		{
			cout << "ERROR: Algorithm should be 0: round robin or 1: mogul";
			return -1;
		}

	}

	if(params::debug_level >= 0)
	{
		long long int total_input_packets = 0;
		long long int total_output_packets = 0;
		for (int i = 0; i < params::total_io; ++i)
		{
			total_input_packets += ios[i].get_total_input_packets();
			total_output_packets += ios[i].get_total_output_packets();
		}
		if (params::debug_level >= 1)
		{
			cout << 
			"interupt_cycles" << " " <<
			"proc_cycles" << " " <<
			"delivering_cycles" << " " <<
			"total_interrupts" << " " <<
			"total_processes" << " " <<
			"total_in" << " " <<
			"total_out" << " " <<
			"throughput" << " " <<
			endl;
		}
		cout << 
			proc.get_interupt_cycles() << " " <<
			proc.get_proc_cycles() << " " <<
			proc.get_delivering_cycles() << " " <<
			proc.get_total_interrupts() << " " <<
			proc.get_total_processes() << " " <<
			total_input_packets << " " <<
			total_output_packets << " " <<
			proc.get_throughput() << " " << 
			endl;

		// cout << interupt_cycles << " " << proc_cycles << " " << interrupts.size() <<
		// " " << processes.size() 
	}

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

