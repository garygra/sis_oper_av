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
	queue<int> interrupts_to_deliver;
	queue<process*> processes;

	int proc_window;
	int clk;
	int clk_os;
	int next_pid;
	int min_cycles;
	int max_cycles;
	int queue_max_size;
	int attended_packets;
	int input_packets;
	bool polling;

	// metrics vars
	int interupt_cycles;
	int proc_cycles;
	int delivering_cycles;
	int throughput;
	stringstream* ss;


	default_random_engine generator;
	
	bernoulli_distribution bernoulli_dist;
	normal_distribution<double> normal_dist;
	poisson_distribution<int> poisson_dist;


	void attend_process();

	void attend_interrupts();

	void create_new_process();

	void metrics_to_stream();

	void attend_os_process();

	void add_interrupt_to_deliver();

	void deliver_interrupt();



protected:


public:

	processor();
	virtual ~processor();

	void round_robin();

	void mogul_algorithm();

	void set_new_process_prob(double prob);

	void set_min_max_proc_cycles(int min_cycles_in, int max_cycles_in);

	void set_seed(int seed);

	void set_max_queue_size(int queue_max_size_in);

	void set_new_interrupts(int interrupt_len);
	
	void set_proc_window(int proc_window_in);

	void set_arrival_mean(double arrival_mean);
	
	void print_metrics();
	
	void print_metrics_header();

	string metrics_tostring();

	int get_interupt_cycles();

	int get_proc_cycles();

	int get_total_interrupts();

	int get_total_processes();

	int get_input_packets();

	int get_attended_packets();

	int get_delivering_cycles();

	int get_throughput();

};

#endif