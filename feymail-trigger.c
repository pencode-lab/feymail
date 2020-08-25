#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>


#include <feymail-trigger.h>


#ifndef O_NONBLOCK
#define O_NONBLOCK O_NDELAY
#endif

static int ndelay_on(int fd)
{
    return fcntl(fd,F_SETFL,fcntl(fd,F_GETFL,0) | O_NONBLOCK);
}

static int ndelay_off(int fd)
{
    return fcntl(fd,F_SETFL,fcntl(fd,F_GETFL,0) & ~O_NONBLOCK);
}

void feymail_triggerpull(char *fn) /*lock/trigger*/
{
    int fd;

    fd = open(fn,O_WRONLY | O_NDELAY);
    if (fd >= 0){
        ndelay_on(fd);
        if(write(fd,"",1)==-1); /* if it fails, bummer */
        close(fd);
    }else {
        /*if No tigger file，ignore trigger*/
        //fprintf(stderr,"open error:%s\n",strerror(errno));
    }

}
