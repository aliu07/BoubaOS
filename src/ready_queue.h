#include "pcb.h"

void ready_queue_init();
int add_process(struct PCB *pcb);
struct PCB* pop_process();
int get_process_count();
void sort_ready_queue();
