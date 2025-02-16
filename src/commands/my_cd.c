#include <unistd.h>

#include "../../include/commands/my_cd.h"
#include "../../include/utils/badcommand.h"
#include "../../include/utils/utils.h"

int my_cd(char *dirname) {
    if (is_valid_name(dirname) != 1) {
        return badcommandNameNotAlphanum();
    }

    if (chdir(dirname) != 0) {
        return badcommandDirDoesNotExist();
    }

    return 0;
}
