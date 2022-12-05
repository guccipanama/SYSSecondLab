#ifndef SIGNAL_H
#define SIGNAL_H

#include "helper.h"

namespace SIGNALS
{
    void sig_handler(int signal_id);

    void rt_sig_handler(int signal_id, siginfo_t *signal_info, void *ctx);

    bool exists(pid_t p, bool is_parent);

    void assumpt(pid_t p_id, bool isParent);

    void guess(pid_t ID_Guesser);

    void prepare();

    void on_ended();
}
#endif