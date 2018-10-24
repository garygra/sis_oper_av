#include "processor.hpp"

processor::processor()
{
	next_pid = 1;
	ss = new stringstream();
	interupt_cycles = 0;
	proc_cycles = 0;
	input_packets = 0;
	attended_packets = 0;
	queue_max_size = 1000000;
	proc_window = 5;
	clk_os = 0;
	clk = 0;
	delivering_cycles = 0;
	throughput = 0;
	polling = false; 
	current_int_num = 0;
	total_number_interrupts = 1;
	quota = 0;
	deliver_interrupt_flag = false;
	(*ss) << "interupt_cycles" << " " << "proc_cycles" << " " << "interrupts.size()" <<
		" " << "processes.size()" << endl;
}

processor::~processor()
{}

void processor::set_total_number_interrupts(int total_number_interrupts_in)
{
	total_number_interrupts = total_number_interrupts_in;
	interrupts = new queue<int>[total_number_interrupts];
	interrupts_to_deliver = new queue<int>[total_number_interrupts];
}

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

void processor::attend_os_process()
{
	proc_cycles++;
}

void processor::attend_process()
{
	if (processes.size())
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
			processes.pop();
			processes.push(curr_proc);
		}
	}
	proc_cycles++;
}

void processor::set_proc_window(int proc_window_in)
{
	proc_window = proc_window_in;
}

void processor::set_arrival_mean(double arrival_mean)
{
	poisson_dist = poisson_distribution<int>(arrival_mean);
}

void processor::attend_interrupts()
{
	interrupts[current_int_num].front()--;
	// cout << "curr_interrupt: " << interrupts.front() << endl; 
	if(!interrupts[current_int_num].front())
	{
		interrupts[current_int_num].pop();
		add_interrupt_to_deliver();
		attended_packets++;
	}
	interupt_cycles++;
}

void processor::add_interrupt_to_deliver()
{
	int next = poisson_dist(generator);
	// cout << "next delivering proc: " << next << endl;
	interrupts_to_deliver[current_int_num].push(next+1);
}

void processor::deliver_interrupt()
{
	// cout << __LINE__ << endl;
	interrupts_to_deliver[current_int_num].front()--;
	// cout << __LINE__ << endl;
	if(!interrupts_to_deliver[current_int_num].front())
	{
		// cout << __LINE__ << endl;
		interrupts_to_deliver[current_int_num].pop();
		throughput++;
	}
	delivering_cycles++;	
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

void processor::set_new_interrupts(int interrupt_len, int interrupt_number)
{
	input_packets++;
	if(interrupts[interrupt_number].size() < queue_max_size)
	{
		interrupts[interrupt_number].push(interrupt_len);
	}	
		
}

void processor::set_quota_limit(int quota_limit_in)
{
	quota_limit = quota_limit_in;
}

void processor::metrics_to_stream()
{
	(*ss) << interupt_cycles << " " << proc_cycles << " " << "interrupts.size() "<<
		" " << processes.size() << " " <<  input_packets << " " << attended_packets << endl;
}

void processor::print_metrics()
{
	cout << interupt_cycles << " " << proc_cycles << " " << "interrupts.size()" <<
		" " << processes.size() << " " <<  input_packets << " " << attended_packets << endl;
}

void processor::print_metrics_header()
{
	cout << "interupt_cycles" << " " << "proc_cycles" << " " << "interrupts.size()" <<
		" " << "processes.size()" << " input_packets" << " attended_packets" << endl;
}

string processor::metrics_tostring()
{
	return ss -> str();
}

void processor::round_robin()
{

	if (interrupts[current_int_num].size())
	{
		attend_interrupts();
	}
	else 
	{
		// cout << "clk: " << clk << "\tOS process\t" << "clk_os: " << clk_os << endl;
		if (interrupts_to_deliver[current_int_num].size())
		{
			deliver_interrupt();
		}
		else
		{
			attend_process();
		}
		
	}
	current_int_num = (current_int_num + 1) % total_number_interrupts;
	clk++;

	// cout << "clk: " << clk << "\tprocs: " << proc_cycles << "\tattend_procs: " << delivering_cycles <<
	// 	"\tthroughput: " << throughput << 
 // 		"\tinters: " << interupt_cycles << endl;

	create_new_process();
	metrics_to_stream();
}


void processor::mogul_algorithm()
{
	if (!polling && interrupts_to_deliver[current_int_num].size() < 0.8 * queue_max_size)
	{
		// cout << "round_robin" << endl;
		round_robin();
	}
	else
	{
		// cout << "polling" << endl;
		polling = true;
		if (deliver_interrupt_flag)
		{
			if (interrupts_to_deliver[current_int_num].size())
			{
				deliver_interrupt();
			}
		} 
		else
		{
			if (interrupts[current_int_num].size())
			{
				attend_interrupts();
			}	
		}
		quota++;
		// polling = interrupts_to_deliver[current_int_num].size() > 0.3 * queue_max_size; 
		if (interrupts_to_deliver[current_int_num].size() < 0.3 * queue_max_size)
		{
			polling = false;
		}
		// cout << __LINE__ << endl;
		if(quota >= quota_limit)
		{
			deliver_interrupt_flag = !deliver_interrupt_flag;
			// cout << "Limit reached, quota_limit: " << quota_limit << endl;
			current_int_num = (current_int_num + 1) % total_number_interrupts;
			quota = 0;
		}
		// cout << __LINE__ << endl;
	}
}

int processor::get_interupt_cycles()
{
	return interupt_cycles;
}

int processor::get_proc_cycles()
{
	return proc_cycles;
}

int processor::get_total_interrupts()
{
	int total = 0;
	for (int i = 0; i < total_number_interrupts; ++i)
	{
		total += interrupts[i].size();
	}
	return total;
}

int processor::get_total_processes() 
{
	return processes.size();
}

int processor::get_input_packets()
{
	return input_packets;
}

int processor::get_attended_packets()
{
	return attended_packets;
}

int processor::get_delivering_cycles()
{
	return delivering_cycles;
}

int processor::get_throughput()
{
	return throughput;
}
