#include <stdio.h>

#include "../../include/commands/echo.h"
#include "../../include/commands/print.h"

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
