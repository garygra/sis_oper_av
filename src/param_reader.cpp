#include "param_reader.hpp"

namespace params
{
	// Global
	int iterations;
	int seed;
	int algorithm;

	// Processor 
	int proc_window;
	int min_cycles_per_proc;
	int max_cycles_per_proc;
	double new_proc_prob;
	int interrupt_queue_size;
	double deliver_interrupt_mean;

	// I/O's
	int total_io;
	double* packet_arrival_mean;
	double* packet_len_mean;
	double* packet_len_stddev;

	// Debug
	bool print_files;
	int debug_level;
}

// #include <boost/program_options.hpp>

namespace po = boost::program_options;
// using namespace std;

void read_parameter(int argc, char* argv[])
{
	std::string config_file_name;
	po::options_description opt_desc("Options");
	opt_desc.add_options()
	("help", "Print available options.") 
		("config", po::value<std::string>(&config_file_name) -> 
			default_value("../input/default.cfg"),
	"The name of a file to read for options. The default is ../input/default.cfg."
	"Cmd options override the ones in the config file. A config file may contain "
	"lines with syntax \n'long_option_name = value '\nand comment lines that begin "
	"with '#'.")
	("iterations", po::value<int>(&params::iterations), "Number of iterations to simulate")
	("seed", po::value<int>(&params::seed), "The seed for the random generator")
	("proc_window", po::value<int>(&params::proc_window), "Length of the window assign to each process")
	("min_cycles_per_proc", po::value<int>(&params::min_cycles_per_proc), "Min cycles a process can have")
	("max_cycles_per_proc", po::value<int>(&params::max_cycles_per_proc), "Max cycles a process can have")
	("total_io", po::value<int>(&params::total_io), "Total number of I/Os to simulate")
	("packet_arrival_mean", po::value<std::string>(), "The mean arrival of a packet for each I/O")
	("packet_len_mean", po::value<std::string>(), "The packet length (payload) mean for each I/O")
	("packet_len_stddev", po::value<std::string>(), "The packet length (payload) stddev for each I/O")
	("new_proc_prob", po::value<double>(&params::new_proc_prob), "The probability of spawing a new process")
	("interrupt_queue_size", po::value<int>(&params::interrupt_queue_size), "The size of the queue for each interruption")
	("print_files", po::value<bool>(&params::print_files), "Parameter to define if auxiliary files should be printed")
	("debug_level", po::value<int>(&params::debug_level), "Level of the debugger")
	("deliver_interrupt_mean", po::value<double>(&params::deliver_interrupt_mean), "Mean for the poisson dist that generates the proc len for delivering a packet")
	("algorithm", po::value<int>(&params::algorithm), "Algorithm to use: 0 => round robin or 1 => mogul")
	// ("", po::value<>(&params::), "")
	;

	po::variables_map varmap;
	po::store(po::parse_command_line(argc, argv, opt_desc), varmap);
	po::notify(varmap);

	if (varmap.count("help"))
	{
		std::cout << opt_desc << std::endl;
		exit(0);
	}

	if (varmap.count("config"))
	{
		// std::cout << "Loading options from " << config_file_name;
		// std::cout.flush();
		std::ifstream ifs( config_file_name.c_str());
		if (!ifs.is_open())
		{
			std::cout << "no such file." << std::endl;
		}
		else
		{
			po::store(po::parse_config_file(ifs, opt_desc), varmap);
			po::notify(varmap);
			// std::cout << " done." << std::endl;
		}
	}

	std::vector<double> arrival_mean_vec;
	if (varmap.count("packet_arrival_mean"))
	{
		std::stringstream stream(varmap["packet_arrival_mean"].as<std::string>());
		double n; 
		while(stream >> n)
		{
			arrival_mean_vec.push_back(n);
		}
		params::packet_arrival_mean = new double[arrival_mean_vec.size()];
		for (int i = 0; i < arrival_mean_vec.size(); ++i)
		{
			params::packet_arrival_mean[i] = arrival_mean_vec[i];
		}
	}

	std::vector<double> packet_mean_vec;
	if (varmap.count("packet_len_mean"))
	{
		std::stringstream stream(varmap["packet_len_mean"].as<std::string>());
		double n; 
		while(stream >> n)
		{
			packet_mean_vec.push_back(n);
		}
		params::packet_len_mean = new double[packet_mean_vec.size()];
		for (int i = 0; i < packet_mean_vec.size(); ++i)
		{
			params::packet_len_mean[i] = packet_mean_vec[i];
		}
	}

	std::vector<double> packet_stddev_vec;
	if (varmap.count("packet_len_stddev"))
	{
		std::stringstream stream(varmap["packet_len_stddev"].as<std::string>());
		double n; 
		while(stream >> n)
		{
			packet_stddev_vec.push_back(n);
		}
		params::packet_len_stddev = new double[packet_stddev_vec.size()];
		for (int i = 0; i < packet_stddev_vec.size(); ++i)
		{
			params::packet_len_stddev[i] = packet_stddev_vec[i];
		}
	}


}

