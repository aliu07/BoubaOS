#define DIRECTORY_PERMS 0777
#define MAX_ARGS_SIZE 7
// Define maximum file size as 100 lines long
#define MAX_FILE_SIZE 100
// Maximum number of programs that can be passed into exec cmd at once
#define MAX_NUM_PROGRAMS 3
#define RR_TIMESLICE 2
#define RR30_TIMESLICE 30

int interpreter(char *command_args[], int args_size);
int help();
