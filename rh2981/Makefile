CFLAGS=-Iinclude -Wall
TASK = 2 2 2 3 2 3 2 3 2 3 2

all: build run

build: clean
	mkdir obj
	gcc $(CFLAGS) -c -o obj/policies.o src/sched-policies.c	
	gcc $(CFLAGS) -c -o obj/simulator.o src/sched-simulator.c
	mkdir bin
	gcc $(CFLAGS) -o bin/sched-simulator obj/simulator.o obj/policies.o

run:
	bin/sched-simulator $(TASK)

clean:
	rm -rf bin obj sched-run.txt
