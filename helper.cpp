#ifndef UNTITLED_HELPER
#define UNTITLED_HELPER

#include "helper.h"
game_result object;

void stat_display(int count)
{
    std::cout << "Game number " << count << " is done" << std::endl;
    std::cout << "Range: 1 - " << object.array_size << std::endl;
    std::cout << "Attempts: " << object.att_count  << std::endl;
}

void increment_game_count(int count)
{
    object.att_count = count;
}

void set_game_time(unsigned time)
{
    object.g_time = time;
}

void set_array_size(int size)
{
    object.array_size = size;
}

void assumption_display(int value)
{
    std::cout << "Maybe " << value << "?" << std::endl;
}

void answer_display(bool flag)
{
    if (flag)
        std::cout << "You're right!" << std::endl;
    else
        std::cout << "You're not right!" << std::endl;
}

#endif