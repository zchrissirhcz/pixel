#include "gtest/gtest.h"
#include "px_compare.h"
#include "px_filesystem.h"

TEST(filesystem, is_file_exist)
{
#if __APPLE__ || __ANDROID__ || __linux__
    bool state = px_is_file_exist("/tmp");
#elif __MSC_VER
    bool state = px_is_file_exist("C:/Windows/system32/etc/driver/hosts")
#else
    #pragma message("TODO: add here")
#endif
    EXPECT_TRUE(state);
}

TEST(filesystem, get_pwd)
{
    char* pwd = px_getpwd();
    EXPECT_TRUE(strlen(pwd) > 0);
    free(pwd);
}

TEST(filesystem, file_not_exist)
{
    PX_FILE_STATE state = px_get_file_or_directory_state("/home/zz/123.txt");
    EXPECT_TRUE(state == PX_FILE_NOT_EXIST);
}