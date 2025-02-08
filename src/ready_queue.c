#include <stdlib.h>
#include "ready_queue.h"

static struct PCB *ready_queue_head = NULL;
// Start PIDs at 1
static int next_pid = 1;
static int process_count = 0;

void ready_queue_init() {
    ready_queue_head = NULL;
    next_pid = 1;
    process_count = 0;
}

int add_process(struct PCB *pcb) {
    pcb->pid = next_pid++;
    process_count++;

    // If queue empty, make head
    if (ready_queue_head == NULL) {
        ready_queue_head = pcb;
        return pcb->pid;
    }

    // Add to end of queue if not empty
    struct PCB *curr = ready_queue_head;

    while (curr != NULL) {
        curr = curr->next;
    }

    curr->next = pcb;
    return pcb->pid;
}

// Pops PCB at head of ready queue
struct PCB* pop_process() {
    // If queue empty
    if (ready_queue_head == NULL) {
        return NULL;
    }

    // Get PCB at head of queue
    struct PCB *popped = ready_queue_head;
    // Clear next ptr of popped PCB
    popped->next = NULL;

    process_count--;
    return popped;

}

int get_process_count() {
    return process_count;
}
