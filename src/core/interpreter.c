#include <string.h>

#include "../../include/core/interpreter.h"
#include "../../include/core/shell.h"
#include "../../include/utils/badcommand.h"
#include "../../include/commands/help.h"
#include "../../include/commands/quit.h"
#include "../../include/commands/set.h"
#include "../../include/commands/print.h"
#include "../../include/commands/run.h"
#include "../../include/commands/echo.h"
#include "../../include/commands/my_ls.h"
#include "../../include/commands/my_mkdir.h"
#include "../../include/commands/my_touch.h"
#include "../../include/commands/my_cd.h"
#include "../../include/commands/my_fork.h"
#include "../../include/commands/exec.h"
#include "../../include/scheduling/ready_queue.h"

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

    } else if (strcmp(command_args[0], "exec") == 0) {

        if (args_size < 3) {
            return badcommandMissingArguments();
        }

        if (args_size > 5) {
            return badcommandTooManyTokens();
        }

        char *programs[MAX_NUM_PROGRAMS] = {NULL};
        int program_count = 0;

        for (int i = 1; i < args_size - 1; i++) {
            programs[program_count] = command_args[i];
            program_count++;
        }

        // Last arg is policy
        char *policy = command_args[args_size - 1];

        return exec(programs, policy);

    } else {
        return badcommand();
    }
}
