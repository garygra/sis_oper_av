#ifndef INPUT_OUTPUT_HPP
#define INPUT_OUTPUT_HPP

#include <sys/time.h>

#include <queue>
#include <random>
#include <assert.h> 
#include <iostream>
#include <sstream>


using namespace std;

class input_output
{
private:

	queue<int> buffer;
	int next_arrival;

	// metrics
	int total_new_packet;
	int total_packets_retrived;
	int total_packets_len;
	stringstream* ss;


	default_random_engine generator;
	poisson_distribution<int> poisson_dist;
	normal_distribution<double> normal_dist;


	void get_input();

	bool metrics_to_stream();

protected:


public:
	input_output();
	virtual ~input_output();

	void iterate();

	void set_seed(int seed);

	void set_arrival_mean(int arrival_mean_in);

	void set_packet_params(double mean, double stddev);

	bool raised_interruption();

	int get_next_packet();

	string metrics_tostring();

	void print_metrics();

	void print_metrics_header();

};

#endif