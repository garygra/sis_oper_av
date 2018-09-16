#include "input_output.hpp"

input_output::input_output()
{
	generator.seed(112392);
}

input_output::~input_output()
{}

void input_output::set_arrival_mean(int arrival_mean)
{
	poisson_dist = poisson_distribution<int>(arrival_mean);
	next_arrival = poisson_dist(generator);

}

void input_output::set_packet_params(double mean, double stddev)
{
	normal_dist = normal_distribution<double>(mean, stddev);
}

void input_output::iterate()
{
	get_input();
}

void input_output::get_input()
{
	if (next_arrival)
	{
		assert(next_arrival > 0);
		next_arrival--;
	}
	else 
	{
		next_arrival = poisson_dist(generator);
		buffer.push(normal_dist(generator));
	}

}

bool input_output::raised_interruption()
{
	return buffer.size() > 0;
}

int input_output::get_next_packet()
{
	int res = buffer.front();
	buffer.pop();
	return res;
}
