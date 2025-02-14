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

int append_process(struct PCB *pcb) {
    process_count++;

    // If queue empty, make head
    if (ready_queue_head == NULL) {
        ready_queue_head = pcb;
        return pcb->pid;
    }

    // Add to end of queue if not empty
    struct PCB *curr = ready_queue_head;

    while (curr->next != NULL) {
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
    // Set new head of queue
    ready_queue_head = popped->next;
    // Clear next ptr of popped PCB
    popped->next = NULL;

    process_count--;
    return popped;

}

int get_process_count() {
    return process_count;
}

void sort_ready_queue() {
    struct PCB *queue[MAX_NUM_PROGRAMS];

    for (int i = 0; i < MAX_NUM_PROGRAMS; i++) {
        queue[i] = pop_process();
    }

    // Bubble sort PCBs by file length
    for (int i = 0; i < MAX_NUM_PROGRAMS; i++) {
        for (int j = i; j < MAX_NUM_PROGRAMS; j++) {
            if (queue[i] != NULL && queue[j] != NULL && queue[i]->file_length > queue[j]->file_length) {
                struct PCB *tmp = queue[i];
                queue[i] = queue[j];
                queue[j] = tmp;
            }
        }
    }

    // Insert sorted PCBs back into ready queue
    for (int i = 0; i < MAX_NUM_PROGRAMS; i++) {
        if (queue[i] != NULL) {
            append_process(queue[i]);
        }
    }
}
