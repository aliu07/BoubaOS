#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>
#include <unistd.h>
#include "shellmemory.h"
#include "shell.h"

int MAX_ARGS_SIZE = 7;
int DIRECTORY_PERMS = 0777;

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

// HELPER FUNCTIONS
int is_string_alphanumeric(char *string);

// Interpret commands and their arguments
int interpreter(char* command_args[], int args_size) {
    // Preliminary validation
    if (args_size < 1) {
        return badcommand();
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
        char value_tokens[MAX_USER_INPUT];

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

    } else {
        return badcommand();
    }
}

int help() {

    // note the literal tab characters here for alignment
    char help_string[] = "COMMAND			DESCRIPTION\n \
help			Displays all the commands\n \
quit			Exits / terminates the shell with “Bye!”\n \
set VAR STRING		Assigns a value to shell memory\n \
print VAR		Displays the STRING assigned to VAR\n \
run SCRIPT.TXT		Executes the file SCRIPT.TXT\n ";
    printf("%s\n", help_string);
    return 0;
}

int quit() {
    printf("Bye!\n");
    exit(0);
}

int set(char *var, char *value) {
    char *link = "=";

    /* PART 1: You might want to write code that looks something like this.
         You should look up documentation for strcpy and strcat.

    char buffer[MAX_USER_INPUT];
    strcpy(buffer, var);
    strcat(buffer, link);
    strcat(buffer, value);
    */

    mem_set_value(var, value);

    return 0;
}

int print(char *var) {
    char *memory_value = mem_get_value(var);

    if (memory_value == NULL) {
        return badcommandVariableDoesNotExist();
    }

    printf("%s\n", memory_value);
    return 0;
}

int run(char *script) {
    int errCode = 0;
    char line[MAX_USER_INPUT];
    FILE *p = fopen(script, "rt");  // the program is in a file

    if (p == NULL) {
        return badcommandFileDoesNotExist();
    }

    fgets(line, MAX_USER_INPUT-1, p);
    while (1) {
        errCode = parseInput(line);	// which calls interpreter()
        memset(line, 0, sizeof(line));

        if (feof(p)) {
            break;
        }
        fgets(line, MAX_USER_INPUT-1, p);
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
        dirname = mem_get_value(dirname);

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
