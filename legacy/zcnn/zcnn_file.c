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


ZN_FILE_STATE get_file_or_directory_state(const char* fname)
{
    if(access(fname, 0)!=-1) {
        if(access(fname, 2)!=-1) {
            return FILE_EXIST_AND_WRITABLE;
        } else {
            return FILE_EXIST_AND_NOT_WRITABLE;
        }
    } else {
        return FILE_NOT_EXIST;
    }
}

bool is_file_exist(const char* fname)
{
    return get_file_or_directory_state(fname) != FILE_NOT_EXIST;
}

char* print_current_working_directory() {
    char* buffer;

    if( (buffer=getcwd(NULL, 0)) == NULL) {
        perror("failed to get current directory\n");
        return NULL;
    } else {
        return buffer;
    }
}
