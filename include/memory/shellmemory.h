#define SHELL_MEM_SIZE 1024
#define VAR_TABLE_SIZE 10

void mem_init();
int find_available_address();
int mem_set_value(char *value_in);
int mem_set_value_with_address(char *value_in, int address);
char *mem_get_value(int address);
void set_var_value(char *var_in, char *value_in);
char *get_var_value(char *var_in);
void free_memory_entry(int address);
