#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP

#include <queue>
#include <random>
#include <assert.h> 
#include <sstream>
#include <iostream>

#include "random.hpp"

#define TIMER_REST_VAL -500

using namespace std;

struct process{
	int pid;
	int total_cycles;
	int spent_cycles;

};

class processor
{
private:

	queue<int>* interrupts;
	queue<int>* interrupts_to_deliver;
	queue<process*> processes;

	// global vars
	int proc_window;
	int clk;
	int clk_os;
	int next_pid;
	int min_cycles;
	int max_cycles;
	int queue_max_size;
	int attended_packets;
	int input_packets;
	int current_int_num;
	int total_number_interrupts;

	// mogul_vars
	bool polling;
	bool deliver_interrupt_flag;
	int quota;
	int quota_limit;
	float queue_avg;

	// limiting_interrupt_arrival_rate
	int sum_deltas;
	int tot_limited_cycles;
	int timer_liar;

	// Polling
	int polling_int_num;
	int total_polling_checks;
	int total_polling_ints;

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

	void calc_avg();

	void IO_polling();



protected:


public:

	processor();
	virtual ~processor();

	void run(int algorithm);

	void round_robin();

	void mogul_algorithm();

	void set_new_process_prob(double prob);

	void set_min_max_proc_cycles(int min_cycles_in, int max_cycles_in);

	void set_seed(int seed);

	void set_max_queue_size(int queue_max_size_in);

	void set_new_interrupts(int interrupt_len, int interrupt_number);
	
	void set_proc_window(int proc_window_in);

	void set_arrival_mean(double arrival_mean);

	void set_total_number_interrupts(int total_number_interrupts_in);
	
	void set_quota_limit(int quota_limit_in);
	
	void print_metrics();
	
	void print_metrics_header();

	void limiting_interrupt_arrival_rate();

	float get_total_limited_cycles();

	string metrics_tostring();

	int get_interupt_cycles();

	int get_proc_cycles();

	int get_total_interrupts();

	int get_total_processes();

	int get_input_packets();

	int get_attended_packets();

	int get_delivering_cycles();

	int get_throughput();

	float get_queue_avg();

	float get_polling_rate();


};

#endif