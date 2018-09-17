#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP

#include <queue>
#include <random>
#include <assert.h> 
#include <sstream>
#include <iostream>

#include "random.hpp"

using namespace std;

struct process{
	int pid;
	int total_cycles;
	int spent_cycles;

};

class processor
{
private:

	queue<int> interrupts;
	queue<process*> processes;

	int proc_window;
	int next_pid;
	int min_cycles;
	int max_cycles;

	// metrics vars
	int interupt_cycles;
	int proc_cycles;
	stringstream* ss;


	default_random_engine generator;
	bernoulli_distribution bernoulli_dist;
	
	normal_distribution<double> normal_dist;

	void attend_process();

	void attend_interrupts();

	void create_new_process();

	void metrics_to_stream();

protected:


public:

	processor();
	virtual ~processor();

	void round_robin();

	void set_new_process_prob(double prob);

	void set_min_max_proc_cycles(int min_cycles_in, int max_cycles_in);

	void set_seed(int seed);

	void set_new_interrupts(int interrupt_len);
	
	void set_proc_window(int proc_window_in);
	
	void print_metrics();
	
	void print_metrics_header();

	string metrics_tostring();

};

#endif