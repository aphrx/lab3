# build an executable named solver from solver.c
all: solver.c 
	gcc -g -Wall -o solver solver.c -pthread

clean:
	$(RM) solver