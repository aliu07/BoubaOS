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
int help();
int quit();
int set(char *var, char *value);
int print(char *var);
int run(char *script);
int echo(char *var);
int my_ls();
int my_mkdir(char *dirname);
int my_touch(char *filename);
int my_cd(char *dirname);
int my_fork(char **args, int args_size);
int exec(char *programs[], char *policy);
