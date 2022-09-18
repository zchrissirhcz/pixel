#include "zcnn_file.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(_MSC_VER)
#include <io.h>
#elif defined(__GNUC__)
#include <unistd.h>
#endif

#if defined(_MSC_VER)
#include <direct.h>
#define getcwd _getcwd
#elif defined(__GNUC__)
#include <unistd.h>
#endif


char* print_current_working_directory() {
    char* buffer;

    if( (buffer=getcwd(NULL, 0)) == NULL) {
        perror("failed to get current directory\n");
        return NULL;
    } else {
        return buffer;
    }
}
