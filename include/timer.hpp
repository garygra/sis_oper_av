#ifndef TIMER_HPP
#define TIMER_HPP

#include <sys/time.h>

class sys_timer_t
{
protected:
	struct timeval start;
	struct timeval finish;
	double elapsed;

public:
	sys_timer_t();
	virtual ~sys_timer_t();

	double get_time_in_secs();

	void reset();

	double measure();

	double measure_reset();

	void add_delay_user_clock(double delay);


};

#endif