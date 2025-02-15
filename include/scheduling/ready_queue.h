#include "pcb.h"

void ready_queue_init();
int append_process(struct PCB *pcb);
int appendleft_process(struct PCB *pcb);
struct PCB* pop_process();
int get_process_count();
void sort_ready_queue();
void age_ready_queue();
int peek_ready_queue();
struct PCB* find_duplicate_script(char *script);
