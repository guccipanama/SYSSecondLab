#ifndef UNTITLED_HELPER_H
#define UNTITLED_HELPER_H

#include <iostream>
#include <cstring>
#include <vector>
#include <algorithm>
#include <random>
#include <unistd.h>
#include <ctime>
#include <fstream>

#include <wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>

#include <csignal>
#include <pthread.h>

#include "check.h"

struct game_result
{
    unsigned g_time = 0.0;
    int array_size = 0;
    int att_count = 0;
};

void increment_game_count(int count);
void set_game_time(unsigned time);
//void str_display(const std::string& data);
//void mes_display(const std::string& data, const int value);
void stat_display(int count);
void assumption_display(int value);
void answer_display(bool flag);
void set_array_size(int size);
#endif //UNTITLED_HELPER_H
