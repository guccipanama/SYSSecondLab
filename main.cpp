#include "pipe.h"
#include "signals.h"

int main()
{
    int game_mode_var = 0;
    std::cout << "Task 1: Using signals" << std::endl;
    std::cout << "Task 2: Using unnamed pipe" << std::endl;

    while (game_mode_var < 1 || game_mode_var > 3 )
        std::cin >> game_mode_var;

    switch(game_mode_var)
    {
        case 1:
        {
            SIGNALS::prepare();
            pid_t main_id = getpid();
            srand(time(NULL));
            pid_t child_id = check(fork());
            bool isParent;
            pid_t IDParent;
            pid_t IDChild;
            IDParent = main_id;
            if (child_id > 0)
            {
                isParent = true;
                IDChild = child_id;
            }
            else
            {
                isParent = false;
                IDChild = getpid();
            }
            for (int i = 1; i < 11; i++)
            {
                if (i % 2 != 0)
                {
                    if (child_id == 0)
                    {
                        SIGNALS::assumpt(IDParent, isParent);
                    }
                    else
                    {
                        SIGNALS::guess(IDChild);
                    }
                }
                else
                {
                    if (child_id > 0)
                    {
                        SIGNALS::assumpt(IDChild, isParent);
                    }
                    else
                    {
                        SIGNALS::guess(IDParent);
                    }
                }
                stat_display(i);
            }
            if (child_id == 0)
            {
                std::cout << "I'm child. I'm exiting now!" << std::endl;
                exit(EXIT_SUCCESS);
            }
            else
            {
                atexit(SIGNALS::on_ended);
                exit(EXIT_SUCCESS);
            }
        }
        case 2:
        {
            int count = 0;
            bool game_flag = true;

            int fd[2];
            int fd_s[2];
            check(pipe(fd));
            check(pipe(fd_s));
            pid_t p_id = check(fork());

            do {
                if(count % 2 != 0)
                    game_flag = false;
                if (p_id)
                {
                    if(game_flag)
                    {
                        sleep(0.5);
                        PIPE::guess(fd_s[1], fd[0]);
                    }

                    else
                    {
                        sleep(1);
                        PIPE::assumpt(fd[1], fd_s[0]);
                    }
                }
                else
                {
                    if(game_flag)
                    {
                        sleep(1);
                        PIPE::assumpt(fd[1], fd_s[0]);
                    }
                    else
                    {
                        sleep(0.5);
                        PIPE::guess(fd_s[1], fd[0]);
                    }
                }
                stat_display(count);
                count++;

                if (!game_flag)
                    game_flag = true;

            } while(count < 11);

            exit(EXIT_SUCCESS);

        }
    }

}