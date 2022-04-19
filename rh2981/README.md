Lab two 
name: Ruixiang Han
netid : rh2981

## instruction 
open this project in terminal

$ make run

CFLAGS=-Iinclude -Wall
TASK = 2 2 3 3 2 3 2 3 2 3 2 (replace with the )

this will run the program with defualt setting Round Robin first serve with input 2 2 3 3 0 3 0 3 0 3 0

Input follows rules below
All time units are milliseconds. Each simulation begins by reading the parameters for the current run, Specifically
n The number of processes.
SA The scheduling algorithm to use. 1 signifies FIFO; 2 signifies RR with
quantum q.
q The quantum for RR scheduling. q is present only if SA=2.
R11, B11, R21, B21, ..., R1n, B1n, R2n, B2n The four running and blocking
times for each process. Note that all four values for Pi are given before
any values for Pi+1. Remember that each Pi runs a third time, in order
to terminate. Since that last run is always for 1 time unit, no input is
needed for its specification.


$ make build
=> in the Makefile
    => TASK = $(YOUR INPUT HERE)

if Round Robin needed to run
bin/sched-simulator $(task number) $(2 for roundrobin) $(quantum size) $(tasks' R1B1's)

if FCFS needed to run
bin/sched-simulator $(task number) $(2 for roundrobin) $(tasks' R1B1's)

## example output 
the example output contains the standard output of example out given in the class

- bin/sched-simulator 2 1 3 2 3 2 3 2 3 2
  - result in [fcfs.txt](./example-outputs/fcfs.txt)
- bin/sched-simulator 2 2 2 3 2 3 2 3 2 3 2
  - result in [rr.txt](./example-outputs/rr.txt) g