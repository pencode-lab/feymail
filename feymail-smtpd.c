#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include <feymail.h>
#include <feymail-string.h>
#include <feymail-smtpd.h>
#include <feymail-fd.h>
#include <feymail-sys.h>


int timeout = 1200;
feymail m;

static int safewrite(int fd,char *buf,int len)
{
  int r;
  r = feymail_timeoutwrite(timeout,fd,buf,len);
  if (r <= 0) _exit(1);
  return r;
}


static void die_read()
{
    fprintf(stderr,"die read\n");
    return;
}

static void die_alarm()
{
    fprintf(stderr,"die alarm\n");
    return;
}

static int saferead(int fd,char *buf,int len)
{
    int r;

    //flush();
    r = feymail_timeoutread(timeout,fd,buf,len);
    if (r == -1) if (errno == ETIMEDOUT) die_alarm();
    if (r <= 0) die_read();

    return r;
}

static int allwrite(int fd,char *buf,int len)
{
    register int w;

    while (len) {
        w = safewrite(fd,buf,len);
        if (w == -1) {
            if (errno == EINTR) continue;
            return -1; /* note that some data may have been written */
        }
        if (w == 0) ; /* luser's fault */
        buf += w;
        len -= w;
    }

    return 0;
}

static int oneread(int fd,char *buf,int len)
{
    int r;

    for (;;) {
        r = saferead(fd,buf,len);
        if (r == -1) if (errno == EINTR) continue;
        return r;
    }
}


static void respond(const char* msg)
{
    fprintf(stdout,"%s\r",msg);
    fflush(stdout);
}

static int readline(int fd,char *line)
{

    int readbytes=0;

    memset(line,0,sizeof(line));
    for(;;){
        int i = oneread(fd,&(line[readbytes]),1);
        if(i!=1) return i;

        if(line[readbytes]=='\n') break;
        readbytes ++;
    }

   
    if (readbytes > 0) if (line[readbytes- 1] == '\r') --readbytes;

    line[readbytes] = 0;

    return (readbytes);
}


static int addrparse(char *arg,char *addr)
{
    int i;
    char ch;
    char terminator;
    int flagesc;
    int flagquoted;
    unsigned int addr_len =0;
 
    terminator = '>';
    i = feymail_string_chr(arg,'<');
    if (arg[i])
        arg += i + 1;
    else { /* partner should go read rfc 821 */
        terminator = ' ';
        arg += feymail_string_chr(arg,':');
        if (*arg == ':') ++arg;
        while (*arg == ' ') ++arg;
    }

    /* strip source route */
    if (*arg == '@') while (*arg) if (*arg++ == ':') break;


    flagesc = 0;
    flagquoted = 0;
    for (i = 0;ch = arg[i];++i) { /* copy arg to addr, stripping quotes */
        if (flagesc) {
            *addr++ = ch; addr_len++;
            flagesc = 0;
        }
        else {
            if (!flagquoted && (ch == terminator)) break;
            switch(ch) {
                case '\\': flagesc = 1; break;
                case '"': flagquoted = !flagquoted; break;
                default: { *addr++ = ch; addr_len++;}
            }
        }
    }

    /* could check for termination failure here, but why bother? */
    *addr ='\0';
  
    if (addr_len > 900) return 0;
    return addr_len;
}


static void err_unimpl(char *arg)
{ 
    return;
}

static void flush()
{
    return;
}

/*from remote get all data and deliver to queue success*/
static void acceptmessage(unsigned long pid)
{
    datetime_sec when;
  
    when = feymail_now();

    fprintf(stderr,"feymail accept data ok.\n");
}


/////////////////////////////////////standard command function for smtp///////////////////////////////
static void smtp_quit()
{
    fprintf(stderr,"in function:%s\n",__func__);                                                                                     
    _exit(0);
}

static void smtp_helo(char *argv)
{
    fprintf(stderr,"in function:%s\n",__func__);
    return ;
}

static void smtp_data(char *argv)
{
    char *s;
    fprintf(stderr,"in function:%s\n",__func__);
    feymail_open(&m);
    s = feymail_close(&m);

    if(!*s) acceptmessage(m.pid);
    else
        fprintf(stderr,"queue close,return:%s\n",s);

    return ;
}

commands smtpcommands[] = {
    { "helo", smtp_helo, 0 },
    { "data", smtp_data, flush },
    { "quit", smtp_quit,0},
    { 0, err_unimpl, flush }
} ;




static bool command(int fin,int fout,commands *c)
{
    int i;
    char *arg;
    char cmd[256]={0};

    for(;;){

        /*from fin read data then parse cmd and arg
        * line like: HELO stone
        * line like :rcpt to:<stone@bzline.com>
        */

        if(readline(fin,cmd) <0) die_read();
        i = feymail_string_chr(cmd,' ');
        arg = cmd + i;
        while (*arg == ' ') ++arg;
        cmd[i] = 0;

fprintf(stderr,"cmd=[%s] arg=[%s]\n",cmd,arg);

        for (i = 0;c[i].text;++i) 
            if (feymail_string_equals(c[i].text,cmd)) break;

        c[i].fun(arg);
        if (c[i].flush) c[i].flush();
    }


    return true;
}


int main(void)
{

    /*
    char *arg =" from: stone@bzline.cn";
    char addr[64]={0};

    unsigned int addr_len= addrparse(arg,addr);

    printf("addr = [%s][%d]\n",addr,addr_len);
    */

    command(0,1,smtpcommands); 
    return(0);
}
