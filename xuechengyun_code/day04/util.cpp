#include "util.h"

#include <stdio.h>
#include <stdlib.h>


void exit_if(bool condition, const char* errmsg) {
    if (condition) {
        perror(errmsg);
        exit(EXIT_FAILURE);
    }
}
