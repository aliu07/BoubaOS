#include<stdlib.h>
#include "shellmemory.h"
#include "pcb.h"

struct PCB *pcb_init(char *filename, char *file_contents[], int file_length) {
    struct PCB *pcb = malloc(sizeof(struct PCB));

    // Error allocating memory
    if (pcb == NULL) {
        return NULL;
    }

    pcb->pid = 0;
    pcb->filename = filename;
    pcb->file_length = file_length;

    // Need logic to compute addresses
    for (int i = 0; i < file_length; i++) {
        int address = mem_set_value(file_contents[i]);
        pcb->addresses[i] = address;
    }

    pcb->program_counter = 0;
    pcb->next = NULL;

    return pcb;
}

// Frees all dynamically allocated memory for the PCB struct
// See typedef in pcd.h to get a better idea of what to free
void pcb_deinit(struct PCB *pcb) {
    if (pcb != NULL) {
        // Free filename ptr
        free(pcb->filename);
        // Free PCB ptr
        free(pcb);
    }
}
