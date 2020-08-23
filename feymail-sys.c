#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <time.h>
#include <signal.h>

#include <feymail-sys.h>



static void sig_catch(int sig,void (*f)())
{
#ifdef HASSIGACTION
  struct sigaction sa;
  sa.sa_handler = f;
  sa.sa_flags = 0;
  sigemptyset(&sa.sa_mask);
  sigaction(sig,&sa,(struct sigaction *) 0);
#else
  signal(sig,f); /* won't work under System V, even nowadays---dorks */
#endif
}


void feymail_sig_pipeignore() { sig_catch(SIGPIPE,SIG_IGN); }
void feymail_sig_pipedefault() { sig_catch(SIGPIPE,SIG_DFL); }



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
