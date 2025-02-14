#include<stdlib.h>
#include<string.h>
#include "shellmemory.h"
#include "pcb.h"

// Start PIDs at 1
static int next_pid = 1;

struct PCB *pcb_init(char *filename, char *file_contents[], int file_length) {
    struct PCB *pcb = malloc(sizeof(struct PCB));

    // Error allocating memory
    if (pcb == NULL) {
        return NULL;
    }

    pcb->pid = next_pid++;
    pcb->filename = strdup(filename);
    pcb->file_length = file_length;

    // Need logic to compute addresses
    for (int i = 0; i < file_length; i++) {
        int address = mem_set_value(file_contents[i]);
        pcb->addresses[i] = address;
    }

    pcb->program_counter = 0;
    // Our definition of job score will be the length of the file
    pcb->job_score = file_length;
    pcb->next = NULL;

    return pcb;
}

// Frees all dynamically allocated memory for the PCB struct
// See typedef in pcd.h to get a better idea of what to free
void pcb_deinit(struct PCB *pcb) {
    if (pcb != NULL) {
        // Free memory entries allocated to file contents
        for (int i = 0; i < pcb->file_length; i++) {
            free_memory_entry(pcb->addresses[i]);
        }

        // Free filename ptr
        free(pcb->filename);
        pcb->filename = NULL;
        // Free PCB ptr
        free(pcb);
        pcb = NULL;
    }
}
