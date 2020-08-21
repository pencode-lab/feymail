#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <sys/select.h> /*select*/

#include <feymail-fd.h>

int feymail_fd_copy(int to,int from)
{
    if (to == from) return 1;
    if (fcntl(from,F_GETFL,0) == -1) return 0;
    close(to);

    if (fcntl(from,F_DUPFD,to) == -1) return 0;
    return 1;
}

int feymail_fd_move(int to,int from)
{
    if (to == from) return 1;
    if (feymail_fd_copy(to,from) == -1) return 0;

    close(from);

    return 1;
}



int feymail_timeoutread(int t,int fd,char *buf,int len) 
{
    fd_set rfds;
    struct timeval tv;

    tv.tv_sec = t;
    tv.tv_usec = 0;

    FD_ZERO(&rfds);
    FD_SET(fd,&rfds);

    if (select(fd + 1,&rfds,(fd_set *) 0,(fd_set *) 0,&tv) == -1) return -1;
    if (FD_ISSET(fd,&rfds)) return read(fd,buf,len);
    errno = ETIMEDOUT;

    return -1;
}


int feymail_timeoutwrite(int t,int fd,char *buf,int len) 
{
    fd_set wfds;
    struct timeval tv;

    tv.tv_sec = t;
    tv.tv_usec = 0;

    FD_ZERO(&wfds);
    FD_SET(fd,&wfds);

    if (select(fd + 1,(fd_set *) 0,&wfds,(fd_set *) 0,&tv) == -1) return -1;
    if (FD_ISSET(fd,&wfds)) return write(fd,buf,len);

    errno = ETIMEDOUT;

    return -1;
}
