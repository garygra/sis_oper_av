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
	sum_deltas = 0;
	tot_limited_cycles = 0;
	polling_int_num = 0;
	total_polling_ints = 0; 
	total_polling_checks = 0;
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

void processor::run(int algorithm)
{
	clk++;
	if (algorithm == 0)
	{
		round_robin();
	}
	else if (algorithm == 1)
	{
		mogul_algorithm();
	}
	else if (algorithm == 2)
	{
		limiting_interrupt_arrival_rate();
	} 
	else if (algorithm == 3)
	{
		IO_polling();
	}
	metrics_to_stream();
	calc_avg();
}

void processor::IO_polling()
{
	if (polling_int_num < total_number_interrupts)
	{
		for (int i = polling_int_num; i < total_number_interrupts; ++i)
		{
			polling_int_num = i + 1;
			total_polling_checks++;
			if (interrupts[i].size())
			{
				total_polling_ints++;
				current_int_num = i;
				attend_interrupts();
				break;		
			}
		}
	}
	if (total_number_interrupts <= polling_int_num && polling_int_num < total_number_interrupts * 2)
	{
		for (int i = (polling_int_num - total_number_interrupts); i < total_number_interrupts; ++i)
		{
			polling_int_num = polling_int_num + 1;
			total_polling_checks++;
			if (interrupts_to_deliver[i].size())
			{
				total_polling_ints++;
				current_int_num = i;
				deliver_interrupt();
				break;		
			}
		}
	}
	if (total_number_interrupts * 2 <= polling_int_num)
	{
		attend_process();
		polling_int_num = 0;
	}

}

void processor::limiting_interrupt_arrival_rate()
{
	float rel = (float) sum_deltas / ((float) clk);
	if( rel < 0.8 || timer_liar >= 0)
	{
		round_robin();
		timer_liar = TIMER_REST_VAL;
	}
	else
	{
		attend_process();
		tot_limited_cycles++;
		timer_liar++;
	}
}

void processor::round_robin()
{

	if (interrupts[current_int_num].size())
	{
		attend_interrupts();
		sum_deltas++;
	}
	else 
	{
		if (interrupts_to_deliver[current_int_num].size())
		{
			deliver_interrupt();
			sum_deltas++;
		}
		else
		{
			attend_process();
		}
		
	}
	current_int_num = (current_int_num + 1) % total_number_interrupts;

	create_new_process();
}


void processor::mogul_algorithm()
{
	if (!polling)
	{
		round_robin();
		for (int i = 0; i < total_number_interrupts; ++i)
		{
			polling = interrupts_to_deliver[i].size() > 0.8 * queue_max_size || polling;
		}
	}
	else
	{
		quota++;
		// polling = interrupts_to_deliver[current_int_num].size() > 0.3 * queue_max_size;
		polling = false; 
		for (int i = 0; i < total_number_interrupts; ++i)
		{
			polling = interrupts_to_deliver[i].size() > 0.3 * queue_max_size || polling;
		}
		if (deliver_interrupt_flag)
		{
			if (interrupts_to_deliver[current_int_num].size())
			{
				deliver_interrupt();
			}
			else 
			{
				current_int_num = (current_int_num + 1) % total_number_interrupts;
			}
		} 
		else
		{
			if (interrupts[current_int_num].size())
			{
				attend_interrupts();
			}	
			else
			{
				current_int_num = (current_int_num + 1) % total_number_interrupts;
			}
		}
		if(quota >= quota_limit)
		{
			deliver_interrupt_flag = !deliver_interrupt_flag;
			quota = 0;
		}
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

float processor::get_queue_avg()
{
	return queue_avg;
}

void processor::calc_avg()
{
	int aux = 0;
	for (int i = 0; i < total_number_interrupts; ++i)
	{
		aux += (interrupts[i].size() + interrupts_to_deliver[i].size());
	}
	queue_avg = (float) ((queue_avg * (clk - 1) + aux ) / ((float) clk));
}

float processor::get_total_limited_cycles()
{
	return (float) (tot_limited_cycles / (float) clk);
}

float processor::get_polling_rate()
{
	return (float) (total_polling_ints / (float) total_polling_checks);
}
