#include <unistd.h>
#include <feymail.h>
#include <feymail-fd.h>
#include <feymail-sys.h>
#include <feymail-string.h>

void feymail_print_version()
{
    fprintf(stderr,"\r\n");
    fprintf(stderr,"%s\r\n\r\n",VERSION);
    fprintf(stderr,"***************Warning, Warning, Warning************************\r\n\r\n");
    fprintf(stderr,"%s","feymail - Similar mail system for High Performance Data transmission.\n");
    fprintf(stderr,"%s","Copyright (C) 2020 Pencode.net\r\n\r\n");
    fprintf(stderr,"*****************************************************************\r\n\r\n");

    fprintf(stderr,"\r\n\r\n");
}




static char *binqqargs[2] = { 0, 0 } ;
static void setup_qqargs()
{
    /*default args*/
    if(!binqqargs[0])
        binqqargs[0] = "bin/feymail-queue";
}

bool feymail_open(feymail *m)
{
    int pim[2];
    int pie[2];
    pid_t pid=-1;

    setup_qqargs();

    if (pipe(pim) == -1) return false;
    if (pipe(pie) == -1) { close(pim[0]); close(pim[1]); return false; }
 
    switch(m->pid = vfork()) {
        case -1:
            close(pim[0]); close(pim[1]);
            close(pie[0]); close(pie[1]);
            return false;
        case 0:
            close(pim[1]);
            close(pie[1]);
            if (feymail_fd_move(0,pim[0]) == false) _exit(120);
            if (feymail_fd_move(1,pie[0]) == false) _exit(120);
            //if (chdir(auto_qmail) == -1) _exit(61);
            execv(*binqqargs,binqqargs);
            _exit(120);
    }


    m->fdm = pim[1]; close(pim[0]);
    m->fde = pie[1]; close(pie[0]);
    m->flagerr = 0;

    return true;
}



void feymail_from(feymail *mq,char *s)
{
    close(mq->fdm);
/*
    if(write(mq->fde,"F",1) !=1) fprintf(stderr,"%s:%s\n",__func__,strerror(errno));
    if(write(mq->fde,s,feymail_strlen(s))<0) fprintf(stderr,"%s:%s\n",__func__,strerror(errno));
    if(write(mq->fde,"",1) !=1) fprintf(stderr,"%s:%s\n",__func__,strerror(errno));
*/
}

void feymail_to(feymail *mq, char *s)
{
    if(write(mq->fde,"T",1)!=1) fprintf(stderr,"%s:%s\n",__func__,strerror(errno));
    if(write(mq->fde,s,feymail_strlen(s)) <0) fprintf(stderr,"%s:%s\n",__func__,strerror(errno));
    if(write(mq->fde,"",1) !=1) fprintf(stderr,"%s:%s\n",__func__,strerror(errno));
}


char *feymail_close(feymail *m)
{
    int wstat;
    int exitcode;

    close(m->fde);

    if (feymail_wait_pid(&wstat,m->pid) != m->pid)
        return "Zqq waitpid surprise (#4.3.0)";
    if (feymail_wait_crashed(wstat))
        return "Zqq crashed (#4.3.0)";
    exitcode = feymail_wait_exitcode(wstat);

fprintf(stderr,"exitcode = %d\n",exitcode);

    switch(exitcode) {
        case 0: if (!m->flagerr) return ""; /* fall through */

        /*some err*/
        case 63:
        case 64:
        case 65:
        case 66:
        case 74: return "exec feymail-queue,but failed (#4.4.2)";
        default:
            if ((exitcode >= 11) && (exitcode <= 40))
                return "Dm permanent problem (#5.3.0)";
            return "Zm temporary problem (#4.3.0)";
    }
}
