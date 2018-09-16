#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <queue> 
#include <random>

#include "input_output.hpp"
#include "random.hpp"

using namespace std;

int main(int argc, char *argv[])
{
	
	int iterations = 1000;

	input_output* io_arr = new input_output[10];
	
	for (int i = 0; i < 10; ++i)
	{
		io_arr[i] = input_output();
		io_arr[i].set_arrival_mean(100);
		io_arr[i].set_packet_params(3, 2);
	}

	for (int i = 0; i < iterations; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			io_arr[j].iterate();
		}

		int aux = uniform_random(0, 10);
		if (io_arr[aux].raised_interruption())
		{
			io_arr[aux].get_next_packet();
		}

		for (int j = 0; j < 10; ++j)
		{
			cout << "iter: " << i << "\tio" << j << 
				"\tinterrupt: " << io_arr[j].raised_interruption() << endl;
			
		}
	}
}