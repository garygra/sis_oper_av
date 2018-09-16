#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP

#include <queue>
#include <random>
#include <assert.h> 

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
	queue<process> processes;

	int time_window;
	default_random_engine generator;
	normal_distribution<double> normal_dist;



protected:


public:
	processor();
	virtual ~processor();

	void round_robin();


};

#endif