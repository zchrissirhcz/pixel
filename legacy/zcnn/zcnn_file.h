#pragma once

#include <stdbool.h>

typedef enum ZN_FILE_STATE
{
    FILE_EXIST_AND_WRITABLE=1,       //!< file exist, and has write permission
    FILE_EXIST_AND_NOT_WRITABLE=2,   //!< file exist, but not has write permission
    FILE_NOT_EXIST=3                 //!< file not exist
} ZN_FILE_STATE;

#ifdef __cplusplus
extern "C" {
#endif

/// @param fname filename or directory name
ZN_FILE_STATE get_file_or_directory_state(const char* fname);

bool is_file_exist(const char* fname);

/// @brief Get the directory where you start executing this program
/// @retval
///     - NULL if cannot get valid directory
///     - valid directory
char* print_current_working_directory();

#ifdef __cplusplus
}
#endif