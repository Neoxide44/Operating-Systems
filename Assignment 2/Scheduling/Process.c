#include "Process.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

Process new_process(int num, int* procs) {
    Process p;
    p.process = calloc(num - 1, sizeof(int));
    assert(p.process != NULL);
    // Set the arrival time
    p.arrival_time = procs[0];
    // The current process at the start is the first one
    p.curr_proc = 0;
    // Number of processes
    p.num_procs = num - 1;

    // Set waiting time to 0;
    p.wait_time = 0;

    // Copy elems from array
    for (int i = 1; i < num; i++) {
        p.process[i - 1] = procs[i];
    }

    return p;
}

Process new_empty_process() {
    Process p;
    p.process = NULL;  // Initializing the pointer to NULL
    p.num_procs = 0;
    p.arrival_time = 0;
    p.curr_proc = 0;
    p.wait_time = 0;
    return p;
}

void set_arrival_time(Process p, int t) {
    p.arrival_time = t;
}

void add_wait_time(Process* p, int t) {
    p->wait_time += t;
}

void add_proc(Process* p, int proc) {
    p->num_procs++;
    p->process = realloc(p->process, p->num_procs * sizeof(int));
    p->process[p->num_procs - 1] = proc;
}

void free_process(Process p) {
    free(p.process);
}

void next_proc(Process* p) {
    p->curr_proc++;
}

void print_process(Process p) {
    printf("Process: \n");
    printf("Arrival time: %d\n", p.arrival_time);
    printf("Waiting time: %d\n", p.wait_time);
    printf("Processes:\n");
    for (int i = 0; i < p.num_procs; i++) {
        printf("%d ", p.process[i]);
    }
    printf("\n");
}