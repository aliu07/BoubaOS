#include "shell.h"
#include "shellmemory.h"
#include "ready_queue.h"

// First-come, first-server policy
// Execute each script in queue until completion sequentially
int scheduler_fcfs() {
    int errCode;

    while (get_process_count() > 0) {
        struct PCB *pcb = pop_process();

        while (pcb->program_counter < pcb->file_length) {
            int address = pcb->addresses[pcb->program_counter];
            errCode = parseInput(mem_get_value(address));
            pcb->program_counter++;
        }

        // Cleanup
        pcb_deinit(pcb);
    }

    return errCode;
}

void scheduler_sjf() {
    return;
}

void scheduler_rr() {
    return;
}
