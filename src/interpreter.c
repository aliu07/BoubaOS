#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>
#include <unistd.h>
#include "shellmemory.h"
#include "interpreter.h"
#include "shell.h"
#include "pcb.h"

int badcommand() {
    printf("Unknown Command\n");
    return 1;
}

int badcommandMissingArguments() {
    printf("Bad command: Missing arguments\n");
    return 2;
}

int badcommandTooManyTokens() {
    printf("Bad command: Too many tokens\n");
    return 3;
}

int badCommandErrorOccurred() {
    printf("Bad command: An error occurred\n");
    return 4;
}

// For run command only
int badcommandFileDoesNotExist() {
    printf("Bad command: File not found\n");
    return 5;
}

int badcommandVariableDoesNotExist() {
    printf("Bad command: Variable does not exist in shell memory\n");
    return 6;
}

int badcommandNameNotAlphanum() {
    printf("Bad command: Directory/file name not alphanumeric\n");
    return 7;
}

int badcommandDirDoesNotExist() {
    printf("Bad command: Directory name does not exist\n");
    return 8;
}

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

// HELPER FUNCTIONS
int is_string_alphanumeric(char *string);

// Interpret commands and their arguments
int interpreter(char* command_args[], int args_size) {
    // Preliminary validation
    if (args_size < 1) {
        return badcommand();
    }

    if (args_size > MAX_ARGS_SIZE) {
        return badcommandTooManyTokens();
    }

    int i;
    // Terminate args at newline chars
    for (i = 0; i < args_size; i++) {
        command_args[i][strcspn(command_args[i], "\r\n")] = 0;
    }

    if (strcmp(command_args[0], "help") == 0){
        //help
        if (args_size != 1) return badcommand();
        return help();

    } else if (strcmp(command_args[0], "quit") == 0) {
        //quit
        if (args_size != 1) return badcommand();
        return quit();

    } else if (strcmp(command_args[0], "set") == 0) {
        // SET COMMAND
        if (args_size < 3) {
            return badcommandMissingArguments();
        }

        if (args_size > MAX_ARGS_SIZE) {
            return badcommandTooManyTokens();
        }

        char *variable_name = command_args[1];
        char value_tokens[MAX_USER_INPUT] = "";

        for (int i = 2; i < args_size; i++) {
            strcat(value_tokens, command_args[i]);

            if (i < args_size - 1) {
                strcat(value_tokens, " ");
            }
        }

        return set(variable_name, value_tokens);

    } else if (strcmp(command_args[0], "print") == 0) {
        if (args_size != 2) return badcommand();
        return print(command_args[1]);

    } else if (strcmp(command_args[0], "run") == 0) {
        if (args_size != 2) return badcommand();
        return run(command_args[1]);

    } else if (strcmp(command_args[0], "echo") == 0) {
        // ECHO COMMAND
        if (args_size < 2) {
            return badcommandMissingArguments();
        }

        if (args_size > 2) {
            return badcommandTooManyTokens();
        }

        return echo(command_args[1]);
    } else if (strcmp(command_args[0], "my_ls") == 0) {

        if (args_size > 1) {
            return badcommandTooManyTokens();
        }

        return my_ls();

    } else if (strcmp(command_args[0], "my_mkdir") == 0) {

        if (args_size < 2) {
            return badcommandMissingArguments();
        }

        if (args_size > 2) {
            return badcommandTooManyTokens();
        }

        return my_mkdir(command_args[1]);

    } else if (strcmp(command_args[0], "my_touch") == 0) {

        if (args_size < 2) {
            return badcommandMissingArguments();
        }

        if (args_size > 2) {
            return badcommandTooManyTokens();
        }

        return my_touch(command_args[1]);

    } else if (strcmp(command_args[0], "my_cd") == 0) {

        if (args_size < 2) {
            return badcommandMissingArguments();
        }

        if (args_size > 2) {
            return badcommandTooManyTokens();
        }

        return my_cd(command_args[1]);

    }  else if (strcmp(command_args[0], "my_fork") == 0) {

        if (args_size < 2) {
            return badcommandMissingArguments();
        }

        if (args_size > MAX_ARGS_SIZE) {
            return badcommandTooManyTokens();
        }

        // Skip "exec" and pass in remaining arguments only
        return my_fork(command_args + 1, args_size - 1);

    } else {
        return badcommand();
    }
}

int help() {

    char help_string[] =
       "COMMAND          DESCRIPTION\n"
       "help             Displays all the commands\n"
       "quit             Exits / terminates the shell with \"Bye!\"\n"
       "set VAR STRING   Assigns a value to shell memory\n"
       "print VAR        Displays the STRING assigned to VAR\n"
       "run SCRIPT.TXT   Executes the file SCRIPT.TXT\n"
       "echo STRING      Displays STRING. If STRING starts with $, prints the value of the variable\n"
       "my_ls            Lists the contents of the current directory\n"
       "my_mkdir DIR     Creates a new directory with name DIR\n"
       "my_touch FILE    Creates a new empty file with name FILE\n"
       "my_cd DIR        Changes current directory to DIR\n"
       "my_fork CMD      Executes CMD with ARGS using fork-exec pattern\n";

    printf("%s\n", help_string);
    return 0;
}

int quit() {
    printf("Bye!\n");
    exit(0);
}

int set(char *var, char *value) {
    set_var_value(var, value);
    return 0;
}

int print(char *var) {
    char *memory_value = get_var_value(var);

    if (memory_value == NULL) {
        return badcommandVariableDoesNotExist();
    }

    printf("%s\n", memory_value);
    return 0;
}

int run(char *script) {
    int errCode = 0;
    // "rt" = read text mode
    FILE *p = fopen(script, "rt");

    if (p == NULL) {
        return badcommandFileDoesNotExist();
    }

    // Buffer file contents in array
    char *file_contents[MAX_FILE_SIZE];
    char buffer[MAX_USER_INPUT];
    int line_count = 0;

    while (fgets(buffer, MAX_USER_INPUT, p) != NULL && line_count < MAX_FILE_SIZE) {
        file_contents[line_count] = strdup(buffer);
        line_count++;
    }

    // Init PCB struct for process... includes writing file contents to shell memory
    struct PCB *pcb = pcb_init(script, file_contents, line_count);

    // Extract each line of file and execute
    for (int i = 0; i < pcb->file_length; i++) {
        int address = pcb->addresses[i];
        errCode = parseInput(mem_get_value(address));
    }

    fclose(p);

    return errCode;
}

int echo(char *var) {
    if (var[0] == '$') {
        // Get rid of dollar sign
        var++;
        // Fetch variable value from memory
        return print(var);
    } else{
        printf("%s\n", var);
    }

    return 0;
}

int my_ls() {
    system("ls");
    return 0;
}

int my_mkdir(char *dirname) {
    // Fetch from memory if preceded by '$' sign
    if (dirname[0] == '$') {
        dirname++;
        dirname = get_var_value(dirname);

        if (dirname == NULL) {
            return badcommandVariableDoesNotExist();
        }
    }

    if (is_string_alphanumeric(dirname) != 1) {
        return badcommandNameNotAlphanum();
    }

    // Error creating directory
    if (mkdir(dirname, DIRECTORY_PERMS) == -1) {
        return badCommandErrorOccurred();
    }

    return 0;
}

int my_touch(char *filename) {
    if (is_string_alphanumeric(filename) != 1) {
        return badcommandNameNotAlphanum();
    }

    FILE *fptr;
    fptr = fopen(filename, "w");

    // Error creating file
    if (fptr == NULL) {
        return badCommandErrorOccurred();
    }

    // Error closing file
    if (fclose(fptr) != 0) {
        return badCommandErrorOccurred();
    }

    return 0;
}

int my_cd(char *dirname) {
    if (is_string_alphanumeric(dirname) != 1) {
        return badcommandNameNotAlphanum();
    }

    if (chdir(dirname) != 0) {
        return badcommandDirDoesNotExist();
    }

    return 0;
}

int my_fork(char **args, int args_size) {
    // Returns 0 in child, positive value in parent
    pid_t pid = fork();

    // Fork failed
    if (pid == -1) {
        return badCommandErrorOccurred();
    }

    if (pid == 0) {
        // Child
        char *fork_args[args_size + 1];

        for (int i = 0; i < args_size; i++) {
            fork_args[i] = args[i];
        }

        // NULL terminate array of arguments for execvp
        fork_args[args_size] = NULL;

        execvp(fork_args[0], fork_args);

        exit(1);
    } else {
        // Parent
        int status;
        // Wait for child to complete
        waitpid(pid, &status, 0);

        return WEXITSTATUS(status);
    }
}


// === HELPER FUNCTIONS ===

/*
Checks if a string is alphanumeric.
Returns:
- 0 if not alphanumeric
- 1 if alphanumeric
*/
int is_string_alphanumeric(char *string) {
    if (string == NULL) {
        return 0;
    }

    int ix = 0;
    while (string[ix] != '\0') {
        if (isalnum(string[ix]) == 0) {
            return 0;
        }

        ix++;
    }

    return 1;
}
