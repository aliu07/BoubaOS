#include <stdio.h>
#include <stdlib.h>

#include "../../include/commands/quit.h"
#include "../../include/memory/backing_store.h"

int quit() {
    backing_store_deinit();
    printf("Bye!\n");
    exit(0);
}
