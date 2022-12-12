#ifndef UNTITLED_U_PIPE
#define UNTITLED_U_PIPE

#include "pipe.h"
namespace PIPE
{
    void guess(const int w_fd, const int r_fd)
    {
        int end_range = 1 + (int) random() % 20;
        int value = 1 + (int) random() % end_range;
        bool flag = false;

        set_array_size(end_range);
        std::cout << "A number is guessed" << std::endl;
        std::cout << "Our range is: " << end_range << std::endl;
        std::cout << "Value is: " << value << std::endl;
        check(write(w_fd, &end_range, sizeof(end_range)));

        do {
            int assumption;
            if (check(read(r_fd, &assumption, sizeof(int))))
            {
                if (assumption == value)
                    flag = true;

                check(write(w_fd, &flag, sizeof(flag)));
                answer_display(flag);
            }
            else
                _exit(EXIT_FAILURE);
        }
        while (!flag);

        return;
    }

    void assumpt(const int w_fd, const int r_fd)
    {
        int range;
        int value = 0;
        bool flag = false;
        std::vector<int> attempt = {};
        unsigned int start_time = clock();

        if (check(read(r_fd, &range, sizeof(int))))
        {
            for (size_t i = 1; i < range; i++) {
                attempt.push_back(i);
            }
            std::random_device rd;
            std::mt19937 g(rd());
            std::shuffle(attempt.begin(), attempt.end(), g);

            do {
                //usleep(100);
                value = attempt[0];
                assumption_display(value);
                check(write(w_fd, &value, sizeof(value)));

                if (check(read(r_fd, &flag, sizeof(bool))))
                {
                    if (!flag)
                    {
                        attempt.erase(attempt.begin());
                    }
                    else
                    {
                        increment_game_count(range - attempt.size());
                        unsigned int end_time = clock();
                        set_game_time(end_time - start_time);
                        return;
                    }
                }
                else
                {
                    _exit(EXIT_FAILURE);
                }
            }
            while (!flag);
        }
        _exit(EXIT_FAILURE);
        return;
    }
}
#endif