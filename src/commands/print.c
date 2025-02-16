#include <stdio.h>

#include "../../include/commands/print.h"
#include "../../include/memory/shellmemory.h"
#include "../../include/utils/badcommand.h"

int print(char *var) {
    char *memory_value = get_var_value(var);

    if (memory_value == NULL) {
        return badcommandVariableDoesNotExist();
    }

    printf("%s\n", memory_value);
    return 0;
}
