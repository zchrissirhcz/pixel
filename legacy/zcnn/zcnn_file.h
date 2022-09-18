#pragma once

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif


/// @brief Get the directory where you start executing this program
/// @retval
///     - NULL if cannot get valid directory
///     - valid directory
char* print_current_working_directory();

#ifdef __cplusplus
}
#endif