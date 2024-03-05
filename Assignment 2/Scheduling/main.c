#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "LibQueue.h"
#include "Process.h"
#include "scanner.h"

int tick(Queue CPU, Queue IO) {
}

// Check which queue chould go first
// Account for the queues being empty or if they need to go at the same time
// 1 for CPU
// 2 for IO
// 3 for both (CPU then IO)
int check(Queue CPU, Queue IO) {
    int cpuFrontProcess = CPU.array[CPU.front]->process[CPU.array[CPU.front]->curr_proc];
    int ioFrontProcess = IO.array[IO.front]->process[IO.array[IO.front]->curr_proc];

    if (!isEmptyQueue(CPU) && isEmptyQueue(IO)) {
        return 1;
    } else if (isEmptyQueue(CPU) && !isEmptyQueue(IO)) {
        return 2;
    } else if (!isEmptyQueue(CPU) && !isEmptyQueue(IO)) {
        if (cpuFrontProcess > ioFrontProcess) {
            return 1;
        } else if (cpuFrontProcess < ioFrontProcess) {
            return 2;
        } else if (cpuFrontProcess == ioFrontProcess) {
            return 3;
        }
    }
}

int schedule(Process **processes, int num_procs) {
    Queue CPU = newQueue(num_procs);
    Queue IO = newQueue(num_procs);
    Process *cpuProcess = NULL;
    Process *ioProcess = NULL;
    int time = 0;
    int cpuEndTime = 0;
    int ioEndTime = -1;

    enqueue(processes[0], &CPU);
    while (!isEmptyQueue(CPU) || !isEmptyQueue(IO)) {
        int proverka = check(CPU, IO);
        // Do stuff on the CPU
        if (proverka == 1) {
            tick()
        }
    }
}

int main() {
    Process *processes = NULL;
    int num_procs;
    int time;

    num_procs = scanner(&processes);
    Queue test = newQueue(num_procs);

    for (int i = 0; i < num_procs; i++) {
        enqueue(&processes[i], &test);
    }
    for (int i = 0; i < num_procs; i++) {
        Process *p = dequeue(&test);
        print_process(*p);
    }

    freeQueue(test);

    for (int i = 0; i < num_procs; i++)
        free_process(processes[i]);
    free(processes);
    return 0;
}
