#ifndef SIGNAL
#define SIGNAL

#include "signals.h"
namespace SIGNALS
{
    volatile sig_atomic_t last_signal_id;
    volatile sig_atomic_t signal_value;

    void prepare()
    {
        struct sigaction
                real_time_action{},
                guesser_action{},
                assumpter_action{},
                quit_action{};

        sigset_t set;
        sigemptyset(&set);
        sigfillset(&set);


        sigprocmask(SIG_SETMASK, &set, NULL);

        real_time_action.sa_sigaction = SIGNALS::rt_sig_handler;
        real_time_action.sa_flags = SA_SIGINFO;
        check(sigaction(SIGALRM, &real_time_action, nullptr));

        guesser_action.sa_handler = sig_handler;
        check(sigaction(SIGUSR1, &guesser_action, nullptr));

        assumpter_action.sa_handler = sig_handler;
        check(sigaction(SIGUSR2, &assumpter_action, nullptr));

        quit_action.sa_handler = sig_handler;
        check(sigaction(SIGCHLD, &quit_action, nullptr));
    }

    void sig_handler(int signal_id)
    {
        last_signal_id = signal_id;
    }

    void rt_sig_handler(int signal_id, siginfo_t *signal_info, void *ctx)
    {
        last_signal_id = signal_id;
        signal_value = signal_info->si_value.sival_int;
    }

    void on_ended()
    {
        int stat;
        wait(&stat);
        if (last_signal_id == SIGCHLD) {
            printf("Parent got SIGCHLD! I'm exiting now\n");
            exit(EXIT_SUCCESS);
        } else {
            printf("Parent can't get SIGCHLD!\n");
        }
    }

    void try_send(pid_t p, int sig, int value) {

        check(sigqueue(p, sig, sigval{value}));
    }

    void try_send_result_guess(pid_t p, int sig) {
        check(kill(p, sig));
    }

    bool exists(pid_t p, bool isParent) {
        if (!isParent)
            return (getpgid(p) >= 0);
        int stat;
        return waitpid(p, &stat, WNOHANG) == 0;
    }

    void guess(pid_t ID_Guesser)
    {
        //sleep(1);

        int end_range = 12 + (int) random() % 22;
        int value = 1 + (int) random() % end_range;

        set_array_size(end_range);
        std::cout << "A number is guessed" << std::endl;
        std::cout << "Our range is: " << end_range << std::endl;
        std::cout << "Value is: " << value << std::endl;

        check(sigqueue(ID_Guesser, SIGALRM, sigval{end_range}));

        while (true)
        {
            sigset_t set;
            sigemptyset(&set);
            sigfillset(&set);
            sigdelset(&set, SIGALRM);
            sigsuspend(&set);
            if (signal_value == value)
            {
                try_send_result_guess(ID_Guesser, SIGUSR1);
                answer_display(true);
                return;
            }
            else
            {
                try_send_result_guess(ID_Guesser, SIGUSR2);
                answer_display(false);
            }
            printf("\n");
        }
    }

    void assumpt(pid_t p_id, bool isParent) {
        //sleep(1);
        int value = 0;
        unsigned int start_time = clock();

        {
            sigset_t set;
            sigemptyset(&set);
            sigfillset(&set);
            sigdelset(&set, SIGALRM);
            sigsuspend(&set);
        }

        int range = signal_value;
        std::vector<int> attempt = {};

        for (int i = 1; i < range + 1; i++)
        {
            attempt.push_back(i);
        }

        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(attempt.begin(), attempt.end(), g);

        while (true)
        {
            sigset_t set;
            sigfillset(&set);
            sigdelset(&set, SIGUSR1);
            sigdelset(&set, SIGUSR2);

            value = attempt[0];

            //sleep(1);
            assumption_display(value);

            if (exists(p_id, isParent))
                try_send(p_id, SIGALRM, value);

            do
            {
                sigsuspend(&set);

                if (last_signal_id == SIGUSR1)
                {
                    increment_game_count(range - attempt.size());
                    unsigned int end_time = clock();
                    set_game_time(end_time - start_time);
                    return;
                }
                else if (last_signal_id == SIGUSR2)
                {
                    attempt.erase(attempt.begin());
                    break;
                }
            } while (last_signal_id != SIGUSR1 && last_signal_id != SIGUSR2);
        }

    }
}
#endif