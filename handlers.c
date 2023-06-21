#include "handlers.h"

extern volatile sig_atomic_t pgot_int;
extern volatile sig_atomic_t pgot_usr1;
extern volatile sig_atomic_t pgot_usr2;
extern volatile sig_atomic_t pgot_alrm;

/*HANDLERS*/
void pall_handler(int sig)
{
    switch (sig)
    {
    case SIGINT:
        pgot_int = 1;
        break;

    case SIGUSR1:
        pgot_usr1 = 1;
        break;
    
    case SIGUSR2:
        pgot_usr2 = 1;
        break;

    case SIGALRM:
        pgot_alrm = 1;
        break;

    default:
        break;
    }
}
