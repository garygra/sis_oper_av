#ifndef RANDOM_HPP
#define RANDOM_HPP

void init_random(int seed);

double uniform_random();

double uniform_random(double min, double max);

int uniform_int_random(int min, int max);

#endif