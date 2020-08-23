#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <time.h>

#include <feymail-sys.h>


int feymail_wait_pid(int *wstat,int pid)
{
    int r;

    do
        r = waitpid(pid,wstat,0);
    while ((r == -1) && (errno == EINTR));
    return r;
}


datetime_sec feymail_now()
{
    return time((long *) 0);
}
