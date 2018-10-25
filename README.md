# sis_oper_av
Proyecto de Sistemas Operativos Avanzados

## Compile
´´´
mkdir build
cd build
cmake ..
make
´´´

## Run
To run with the default parameters:
´´´
./livelock_sim
´´´

To see all the parameters:
´´´
./livelock_sim --help
´´´

To run with default parameters except for some:
´´´
./livelock_sim --algorithm=1
´´´

## Run Simulations
In the data directory there is a bash script for simulating running multiple times the simulation, changing the amount of inputs in each new simulation. The following bash commands run the script, save it to a file and plot the output. The plot is named EXAMPLE_TEXT_FILE.eps:
´´´
cd data
./sim.bash  > EXAMPLE_TEXT_FILE.TXT
gnuplot -c plots.gnuplot EXAMPLE_TEXT_FILE

´´´



## TESTS

* live_lock_test_1:  (using sim.bash) quota=-1, algorithm=0, total_io=5
* mogul_test_1:  (using sim.bash) quota=-1, algorithm=1, total_io=5
* mogul_test_2:  (using sim.bash) quota=90000, algorithm=1, total_io=5
* mogul_test_3:  (using sim.bash) quota=80000, algorithm=1, total_io=5
* mogul_test_4:  (using sim.bash) quota=4, algorithm=1, total_io=5
