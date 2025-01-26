#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"
#include "interpreter.h"
#include "shellmemory.h"

int parseInput(char ui[]);

// Start of everything
int main(int argc, char *argv[]) {
    printf("Shell version 1.3 Created September 2024\n");
    help();

    int mode_flag = isatty(STDIN_FILENO);  // 1 = interactive mode, 0 = batch mode
    char prompt = '$';  				   // Shell prompt
    char userInput[MAX_USER_INPUT];		   // user's input stored here
    int errorCode = 0;					   // zero means no error, default

    //init user input
    for (int i = 0; i < MAX_USER_INPUT; i++) {
        userInput[i] = '\0';
    }

    //init shell memory
    mem_init();

    while (1) {
        // Check if we are in interactive mode... omit printing prompt char if we are in batch mode
        if (mode_flag) {
            printf("%c ", prompt);
        }

        // Check if reached end of file when in batch mode
        if (fgets(userInput, MAX_USER_INPUT - 1, stdin) == NULL) {
            break;
        }

        errorCode = parseInput(userInput);

        if (errorCode == -1) {
            exit(99);	// ignore all other errors
        }

        memset(userInput, 0, sizeof(userInput));
    }

    return 0;
}

int wordEnding(char c) {
    // You may want to add ';' to this at some point,
    // or you may want to find a different way to implement chains.
    return c == '\0' || c == '\n' || c == ' ';
}

int parseInput(char inp[]) {
    char tmp[200];
    // Array storing individual commands
    char *commands[100];
    // Array to store words of current command
    char *words[100];

    int cmdCount = 0;
    int wordlen;
    int errorCode;

    // Get first command
    char *cmd = strtok(inp, ";");

    while (cmd != NULL && cmdCount < MAX_NUM_COMMANDS) {
        commands[cmdCount] = strdup(cmd);
        // Get next command. We pass in NULL param to indicate we are still parsing same string 'inp'.
        cmd = strtok(NULL, ";");
        cmdCount++;
    }

    // Process each command
    for (int i = 0; i < cmdCount; i++) {
        int ix = 0;
        int wordCount = 0;

        // Skip white spaces
        for (ix = 0; commands[i][ix] == ' ' && ix < MAX_USER_INPUT; ix++);

        while (commands[i][ix] != '\n' && commands[i][ix] != '\0' && ix < MAX_USER_INPUT) {
            // Extract a word
            for (wordlen = 0; !wordEnding(commands[i][ix]) && ix < MAX_USER_INPUT; ix++, wordlen++) {
                tmp[wordlen] = commands[i][ix];
            }

            tmp[wordlen] = '\0';
            words[wordCount] = strdup(tmp);
            wordCount++;

            if (commands[i][ix] == '\0') {
                break;
            }

            ix++;
        }

        if (wordCount > 0) {
            errorCode = interpreter(words, wordCount);

            for (int j = 0; j < wordCount; j++) {
                free(words[j]);
            }
        }

        free(commands[i]);
    }

    return errorCode;
}
