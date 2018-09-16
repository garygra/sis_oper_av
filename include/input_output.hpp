#ifndef INPUT_OUTPUT_HPP
#define INPUT_OUTPUT_HPP

#include <sys/time.h>

#include <queue>
#include <random>
#include <assert.h> 

using namespace std;

class input_output
{
private:

	queue<int> buffer;
	int next_arrival;

	default_random_engine generator;
	poisson_distribution<int> poisson_dist;
	normal_distribution<double> normal_dist;


	void get_input();

protected:


public:
	input_output();
	virtual ~input_output();

	void iterate();

	void set_arrival_mean(int arrival_mean_in);

	bool raised_interruption();

	void set_packet_params(double mean, double stddev);

	int get_next_packet();


};

#endif