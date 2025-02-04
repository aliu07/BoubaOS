#include <stdlib.h>
#include "process_table.h"

// Array to hold all active PCBs
static struct PCB *process_table[MAX_PROCESSES];
static int process_count = 0;
// Start PIDs at 1
static int next_pid = 1;

void process_table_init() {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        process_table[i] = NULL;
    }

    process_count = 0;
    next_pid = 1;
}

int add_process(struct PCB *pcb) {
    // Find empty slot
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (process_table[i] == NULL) {
            pcb->pid = next_pid++;
            process_table[i] = pcb;
            process_count++;
            return pcb->pid;
        }
    }

    // If not empty slot found...
    return -1;
}

void remove_process(int pid) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (process_table[i] != NULL && process_table[i]->pid == pid) {
            pcb_deinit(process_table[i]);
            process_table[i] = NULL;
            process_count--;
            return;
        }
    }
}

int get_process_count() {
    return process_count;
}
