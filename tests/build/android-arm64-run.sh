#!/bin/bash

BUILD_DIR=android-arm64

test_shift_right()
{
    DST_DIR=/data/local/tmp
    EXE_FILE=test_shift_right

    adb push $BUILD_DIR/$EXE_FILE $DST_DIR
    adb shell "cd $DST_DIR; chmod +x $DST_DIR/$EXE_FILE; ./$EXE_FILE"
}

test_arithmetic()
{
    DST_DIR=/data/local/tmp
    EXE_FILE=test_arithmetic

    adb push $BUILD_DIR/$EXE_FILE $DST_DIR
    adb shell "cd $DST_DIR; chmod +x $DST_DIR/$EXE_FILE; ./$EXE_FILE"
}

test_load_store()
{
    DST_DIR=/data/local/tmp
    EXE_FILE=test_load_store

    adb push $BUILD_DIR/$EXE_FILE $DST_DIR
    adb shell "cd $DST_DIR; chmod +x $DST_DIR/$EXE_FILE; ./$EXE_FILE"
}

test_arrays()
{
    DST_DIR=/data/local/tmp
    EXE_FILE=test_arrays

    adb push $BUILD_DIR/$EXE_FILE $DST_DIR
    adb shell "cd $DST_DIR; chmod +x $DST_DIR/$EXE_FILE; ./$EXE_FILE"
}

test_max_and_min()
{
    DST_DIR=/data/local/tmp
    EXE_FILE=test_max_and_min

    adb push $BUILD_DIR/$EXE_FILE $DST_DIR
    adb shell "cd $DST_DIR; chmod +x $DST_DIR/$EXE_FILE; ./$EXE_FILE"
}

test_conditionals()
{
    DST_DIR=/data/local/tmp
    EXE_FILE=test_conditionals

    adb push $BUILD_DIR/$EXE_FILE $DST_DIR
    adb shell "cd $DST_DIR; chmod +x $DST_DIR/$EXE_FILE; ./$EXE_FILE"
}

test_register_data_rearrange()
{
    DST_DIR=/data/local/tmp
    EXE_FILE=test_register_data_rearrange

    adb push $BUILD_DIR/$EXE_FILE $DST_DIR
    adb shell "cd $DST_DIR; chmod +x $DST_DIR/$EXE_FILE; ./$EXE_FILE"
}

test_add()
{
    DST_DIR=/data/local/tmp
    EXE_FILE=test_add

    adb push $BUILD_DIR/$EXE_FILE $DST_DIR
    adb shell "cd $DST_DIR; chmod +x $DST_DIR/$EXE_FILE; ./$EXE_FILE"
}

# test_shift_right
# test_arithmetic
# test_load_store
# test_arrays
# test_max_and_min
# test_conditionals
# test_register_data_rearrange
#test_add
test_shift_right