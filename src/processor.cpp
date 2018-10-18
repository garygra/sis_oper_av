#include "processor.hpp"

processor::processor()
{
	next_pid = 1;
	ss = new stringstream();
	interupt_cycles = 0;
	proc_cycles = 0;
	input_packets = 0;
	output_packets = 0;
	(*ss) << "interupt_cycles" << " " << "proc_cycles" << " " << "interrupts.size()" <<
		" " << "processes.size()" << endl;
}

processor::~processor()
{}

void processor::set_seed(int seed)
{
	generator.seed(seed);	
}

void processor::set_max_queue_size(int queue_max_size_in)
{
	queue_max_size = queue_max_size_in;
}

void processor::set_new_process_prob(double prob)
{
	bernoulli_dist = bernoulli_distribution(prob);
}

void processor::set_min_max_proc_cycles(int min_cycles_in, int max_cycles_in)
{
	min_cycles = min_cycles_in;
	max_cycles = max_cycles_in;

	process* new_proc = new process();
	new_proc -> pid = next_pid;
	new_proc -> total_cycles = uniform_int_random(min_cycles, max_cycles);
	new_proc -> spent_cycles = 0;

	processes.push(new_proc);
	next_pid++;
}

void processor::attend_process()
{
	process* curr_proc = processes.front();
	curr_proc -> spent_cycles++;
	// cout << "pid: " << curr_proc -> pid << " total: " << 
	// 	curr_proc -> total_cycles << " spent: " << curr_proc -> spent_cycles <<
	// 	" window: " << proc_window << " proc_queue_size: " << processes.size() << endl;

	if (curr_proc -> total_cycles == curr_proc -> spent_cycles)
	{
		processes.pop();
	}
	else if (curr_proc -> spent_cycles % proc_window == 0)
	{
		// cout << __LINE__ << endl;
		processes.pop();
		processes.push(curr_proc);
	}
	proc_cycles++;
}

void processor::set_proc_window(int proc_window_in)
{
	proc_window = proc_window_in;
}


void processor::attend_interrupts()
{
	interrupts.front()--;
	// cout << "curr_interrupt: " << interrupts.front() << endl; 
	if(!interrupts.front())
	{
		interrupts.pop();
		output_packets++;
	}
	interupt_cycles++;
}

void processor::create_new_process()
{
	if (bernoulli_dist(generator))
	{
		process* new_proc = new process();
		new_proc -> pid = next_pid;
		new_proc -> total_cycles = uniform_int_random(min_cycles, max_cycles);
		new_proc -> spent_cycles = 0;

		processes.push(new_proc);
		next_pid++;
	}
}

void processor::set_new_interrupts(int interrupt_len)
{
	input_packets++;
	if(interrupts.size() < queue_max_size)
	{
		interrupts.push(interrupt_len);
	}	
		
}

void processor::metrics_to_stream()
{
	(*ss) << interupt_cycles << " " << proc_cycles << " " << interrupts.size() <<
		" " << processes.size() << " " <<  input_packets << " " << output_packets << endl;
}

void processor::print_metrics()
{
	cout << interupt_cycles << " " << proc_cycles << " " << interrupts.size() <<
		" " << processes.size() << " " <<  input_packets << " " << output_packets << endl;
}

void processor::print_metrics_header()
{
	cout << "interupt_cycles" << " " << "proc_cycles" << " " << "interrupts.size()" <<
		" " << "processes.size()" << "input_packets" << "output_packets" << endl;
}

string processor::metrics_tostring()
{
	return ss -> str();
}

void processor::round_robin()
{
	if (interrupts.size())
	{
		attend_interrupts();
	}
	else if (processes.size())
	{
		attend_process();
	}
	create_new_process();
	metrics_to_stream();
}