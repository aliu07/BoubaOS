#include <string.h>
#include <stdlib.h>
#include<stdio.h>
#include "shellmemory.h"

struct memory_struct {
    char *var;
    char *value;
};

struct memory_struct shellmemory[MEM_SIZE];

// Helper functions
int match(char *model, char *var) {
    int i, len = strlen(var), matchCount = 0;

    for (i = 0; i < len; i++) {
        if (model[i] == var[i]) matchCount++;
    }

    if (matchCount == len) {
        return 1;
    } else {
        return 0;
    }
}



// === Shell memory functions ===

void mem_init() {
    int i;
    for (i = 0; i < MEM_SIZE; i++){
        shellmemory[i].var = NULL;
        shellmemory[i].value = NULL;
    }
}

// Set key value pair
void mem_set_value(char *var_in, char *value_in) {
    int i;

    for (i = 0; i < MEM_SIZE; i++) {
        if (shellmemory[i].var != NULL && strcmp(shellmemory[i].var, var_in) == 0) {
            free(shellmemory[i].value);
            shellmemory[i].value = strdup(value_in);
            return;
        }
    }

    // Value does not exist, need to find a free spot.
    for (i = 0; i < MEM_SIZE; i++) {
        if (shellmemory[i].var == NULL) {
            shellmemory[i].var = strdup(var_in);
            shellmemory[i].value = strdup(value_in);
            return;
        }
    }

    return;
}

// Get value based on input key
char *mem_get_value(char *var_in) {
    int i;

    for (i = 0; i < MEM_SIZE; i++) {
        if (strcmp(shellmemory[i].var, var_in) == 0) {
            return strdup(shellmemory[i].value);
        }
    }

    // If not found, return NULL
    return NULL;
}
