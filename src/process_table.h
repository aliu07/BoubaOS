#include "pcb.h"

#define MAX_PROCESSES 3

void process_table_init();
int add_process(struct PCB *pcb);
void remove_process(int pid);
struct PCB* get_process(int pid);
int get_process_count();
