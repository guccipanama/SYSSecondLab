#include "pipe.h"
#include "signals.h"

int main()
{
    int game_mode = 0;
    std::cout << "1 - Signals" << std::endl;
    std::cout << "2 - Pipe" << std::endl;

    while (game_mode < 1 || game_mode > 2 )
        std::cin >> game_mode;

    switch(game_mode)
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
            int fd_to_send, fd_to_recv;
            pid_t p_id;
            {
                int fd[2];
                int fd_s[2];
                check(pipe(fd));
                check(pipe(fd_s));
//
                p_id = check(fork());
//
                bool isParent = (p_id != 0);

                if(isParent)
                {
                    fd_to_send = fd[1];
                    fd_to_recv = fd_s[0];
                    close(fd[0]);
                    close(fd_s[1]);
                }
                else
                {
                    fd_to_send = fd_s[1];
                    fd_to_recv = fd[0];
                    close(fd_s[0]);
                    close(fd[1]);
                }
            }

            do {
                if(count % 2 != 0)
                    game_flag = false;
                if (p_id)
                {
                    if(game_flag)
                    {
                        usleep(500000);
                        PIPE::guess(fd_to_send, fd_to_recv);
                    }

                    else
                    {
                        sleep(1);
                        PIPE::assumpt(fd_to_send, fd_to_recv);
                    }
                }
                else
                {
                    if(game_flag)
                    {
                        sleep(1);
                        PIPE::assumpt(fd_to_send, fd_to_recv);
                    }
                    else
                    {
                        usleep(500000);
                        PIPE::guess(fd_to_send, fd_to_recv);
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