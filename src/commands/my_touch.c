#include <stdio.h>

#include "../../include/commands/my_touch.h"
#include "../../include/utils/utils.h"
#include "../../include/utils/badcommand.h"

int my_touch(char *filename) {
    if (is_valid_name(filename) != 1) {
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
