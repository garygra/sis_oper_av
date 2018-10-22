#include "input_output.hpp"

input_output::input_output()
{
	ss = new stringstream();
	total_new_packet = 0;
	total_packets_retrived = 0;
	total_packets_len = 0;
}

input_output::~input_output()
{}

void input_output::set_seed(int seed)
{
	generator.seed(seed);
}

void input_output::set_max_queue_size(int max_queue_size_in)
{
	max_queue_size = max_queue_size_in;
}

void input_output::set_arrival_mean(double arrival_mean)
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
	metrics_to_stream();
}

void input_output::get_input()
{
	next_arrival = poisson_dist(generator);
	if (next_arrival)
	{
	// 	assert(next_arrival > 0);
	// 	next_arrival--;
	// }
	// else 
	// {
		for (int i = 0; i < next_arrival; ++i)
		{
			int len = normal_dist(generator);
			if (len <= 0)
			{
				len = 1;
			}
			total_packets_len += len;
			if(buffer.size() < max_queue_size)
			{
				buffer.push(len);
	
			}
			total_new_packet++;
		}
	}
	// cout << "buffer_size:" << buffer.size() << " next_arrival: " << next_arrival << endl;
}

bool input_output::metrics_to_stream()
{
	(*ss) << total_new_packet << " " << total_packets_retrived << " " << total_packets_len << 
		" " << buffer.size() << endl;
}

bool input_output::raised_interruption()
{
	return buffer.size() > 0;
}

int input_output::get_next_packet()
{
	int res = buffer.front();
	buffer.pop();
	total_packets_retrived++;
	return res;
}

string input_output::metrics_tostring()
{
	return ss -> str();
}

void input_output::print_metrics()
{
	cout << total_new_packet << " " << total_packets_retrived << " " << total_packets_len << 
		" " << buffer.size() ;
}

void input_output::print_metrics_header()
{
	cout << "total_new_packet" << " " << "total_packets_retrived" << " " << "total_packets_len" << 
		" " << "buffer.size()" << endl;
}

int input_output::get_total_output_packets()
{
	return total_packets_retrived;
}

int input_output::get_total_input_packets()
{
	return total_new_packet;
}