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

int scheduler_sjf() {
    // Sort PCBs in ready queue by file length
    sort_ready_queue();
    // Execution order is same as FCFS now since sorted...
    return scheduler_fcfs();
}

int scheduler_rr(int time_slice) {
    int errCode;

    while (get_process_count() > 0) {
        struct PCB *pcb = pop_process();

        for (int i = 0; i < time_slice; i++) {
            // Check if exhausted lines to execute
            if (pcb->program_counter == pcb->file_length) {
                break;
            }

            int address = pcb->addresses[pcb->program_counter];
            errCode = parseInput(mem_get_value(address));
            pcb->program_counter++;
        }

        // Add PCB back to queue if still instructions to execute
        if (pcb->program_counter == pcb->file_length) {
            pcb_deinit(pcb);
        } else {
            append_process(pcb);
        }
    }

    return errCode;
}

int scheduler_aging() {
    return 0;
}
