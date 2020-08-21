#include <feymail.h>
#include <feymail-fd.h>

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

bool feymail_open()
{
    int pim[2];
    int pie[2];
    pid_t pid=-1;

    setup_qqargs();

    if (pipe(pim) == -1) return false;
    if (pipe(pie) == -1) { close(pim[0]); close(pim[1]); return false; }
 
    switch(pid = vfork()) {
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

    close(pim[0]);
    close(pie[0]);

    return true;
}
