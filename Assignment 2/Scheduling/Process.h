#ifndef PROCESS_H
#define PROCESS_H

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Process {
    int *process;
    int num_procs;
    int arrival_time;
    int curr_proc;
    int wait_time;
} Process;

Process new_process(int num, int *procs);
Process new_empty_process();
void free_process(Process p);
void add_proc(Process *p, int proc);
void set_arrival_time(Process p, int t);
void add_wait_time(Process *p, int t);
void print_process(Process p);
void next_proc(Process *p);

#endif
