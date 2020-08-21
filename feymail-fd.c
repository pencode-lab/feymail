#include <fcntl.h>
#include <unistd.h>
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
