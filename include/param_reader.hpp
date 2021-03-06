#ifndef PARAM_READER
#define PARAM_READER

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/program_options.hpp>

namespace params
{
	// Global
	extern int iterations;
	extern int seed;
	extern int algorithm;

	// Processor 
	extern int proc_window;
	extern int min_cycles_per_proc;
	extern int max_cycles_per_proc;
	extern double new_proc_prob;
	extern int interrupt_queue_size; 
	extern double deliver_interrupt_mean;
	extern int quota_limit;

	// I/O's
	extern int total_io;
	extern double* packet_arrival_mean;
	extern double* packet_len_mean;
	extern double* packet_len_stddev;

	// Debug
	extern bool print_files;
	extern int debug_level;
}

void read_parameter(int argc, char* argv[]);

#endif