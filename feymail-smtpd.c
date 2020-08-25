#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include <feymail.h>
#include <feymail-string.h>
#include <feymail-smtpd.h>
#include <feymail-fd.h>
#include <feymail-sys.h>


int timeout = 1200;
feymail mq;/*smtp to queue*/
static char mailfrom[256]={0};
static char rcptto[256]={0};
int seenmail = 0;
int flagbarf; /* defined if seenmail */


static void respond(const char* msg)                                                                                                 
{                                                                                                                                    
    fprintf(stdout,"%s",msg);                                                                                                        
    fflush(stdout);                                                                                                                  
}                                                                                                                                    
                                                                                                                                     
static void respond_delay(const char *msg)                                                                                           
{                                                                                                                                    
    fprintf(stdout,"%s",msg);                                                                                                        
}    


void straynewline() { 
    respond("451 See http://pobox.com/~djb/docs/smtplf.html.\r\n"); 
    _exit(1); 
}

static void die_read()
{
    //_exit(1);
}

static void die_alarm()
{
    respond("451 timeout (#4.4.2)\r\n");
    //_exit(1);
}

static void die_control()
{ 
    respond("421 feymail system error (#4.3.0)\r\n"); 
    _exit(1); 
}

static int safewrite(int fd,char *buf,int len)
{
    return feymail_timeoutwrite(timeout,fd,buf,len);                                                                                    
}   

static int saferead(int fd,char *buf,int len)
{
    int r;
    r = feymail_timeoutread(timeout,fd,buf,len);
    if (r == -1) if (errno == ETIMEDOUT) die_alarm();
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

    if(!arg || feymail_strlen(arg)==0) return 0;
 
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
    respond("502 unimplemented (#5.5.1)\r\n");
}

static void flush()
{
    fflush(stdout);
    return;
}

/*from remote get all data and deliver to queue success*/
char accept_buf[40];
static void acceptmessage(unsigned long pid)
{
    datetime_sec when;
  
    when = feymail_now();
    respond_delay("250 ok ");

    accept_buf[feymail_fmt_ulong(accept_buf,(unsigned long) when)] = 0;
    respond_delay(accept_buf);
    respond_delay(" qp ");
    accept_buf[feymail_fmt_ulong(accept_buf,pid)] = 0;
    respond_delay(accept_buf);
    respond("\r\n");
}


/////////////////////////////////////standard command function for smtp///////////////////////////////
static void smtp_quit(char *argv)
{
    respond("221 \r\n");
    _exit(0);
}

void smtp_greet(char *argv)
{
    respond("250 feymail welcome.\r\n");
}



static void smtp_helo(char *argv)
{
    respond("250 \r\n");
    seenmail = 0; 
}

void smtp_rset(arg) char *arg;
{
    seenmail = 0;
    respond("250 flushed\r\n");
}


void smtp_mail(arg) char *arg;
{

    unsigned int addr_len=0;

    addr_len= addrparse(arg,mailfrom);

    if(addr_len<=0){
        respond("555 syntax error (#5.5.4)\r\n"); 
        return; 
    }
    seenmail=1;
    respond("250 ok\r\n");
}

void smtp_rcpt(arg) char *arg; {

    if (!seenmail) { respond("503 MAIL first (#5.5.1)\r\n"); return; }

    int addr_len= addrparse(arg,rcptto);
    if (addr_len<=0) { respond("555 syntax error (#5.5.4)\r\n"); return; }
    
    respond("250 ok\r\n");
}

static void recv_message(feymail *mq)
{
    char ch;
    int state;
    int rbytes=0;

    state = 1;

    for(;;){
        saferead(0,&ch,1);

        switch(state) {
            case 0:
                if (ch == '\n') straynewline();
                if (ch == '\r') { state = 4; continue; }
                break;
            case 1: /* \r\n */
                if (ch == '\n') straynewline();
                if (ch == '.') { state = 2; continue; }
                if (ch == '\r') { state = 4; continue; }
                state = 0;
                break;
            case 2: /* \r\n + . */
                if (ch == '\n') straynewline();
                if (ch == '\r') { state = 3; continue; }
                state = 0;
                break;
            case 3: /* \r\n + .\r */
                if (ch == '\n') return;
                safewrite(mq->fdm,".",1);
                safewrite(mq->fdm,"\r",1);
                if (ch == '\r') { state = 4; continue; }
                state = 0;
                break;
            case 4: /* + \r */
                if (ch == '\n') { state = 1; break; }
                if (ch != '\r') { safewrite(mq->fdm,"\r",1);; state = 0; }
        }
        safewrite(mq->fdm,&ch,1);
    }//for

    return;
}

static void smtp_data(char *argv)
{
    char *qqx;

    
    if (!seenmail){
        respond("555 syntax error (#5.5.4)\r\n");  
        return; 
    }
    if (!feymail_strlen(rcptto)){ 
        respond("503 RCPT first (#5.5.1)\r\n"); 
        return; 
    }
    seenmail = 0;


    if(feymail_open(&mq)==false){
        die_control();
    }

    respond("354 go ahead\r\n");
    recv_message(&mq);

    feymail_from(&mq,mailfrom);
    feymail_to(&mq,rcptto);
    qqx= feymail_close(&mq);

    if(!*qqx) {
        acceptmessage(mq.pid);
        return;
    } else{
        if (*qqx == 'D') 
            respond_delay("554 "); 
        else 
            respond_delay("451 ");

        respond_delay(qqx + 1);
        respond("\r\n");
    }

    return;
}

commands smtpcommands[] = {
    { "mail", smtp_mail, 0 },
    { "rcpt", smtp_rcpt, 0 },
    { "helo", smtp_helo, flush },
    { "data", smtp_data, flush },
    { "quit", smtp_quit,flush},
    { "rset", smtp_rset, 0 },
    { 0,      err_unimpl, flush }
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

        for (i = 0;c[i].text;++i) 
            if (feymail_string_equals(c[i].text,cmd)) break;


        c[i].fun(arg);
        if (c[i].flush) c[i].flush();
    }


    return true;
}


int main(void)
{

    feymail_sig_pipeignore();
/*
    //note: feymail-queue dir...
    if (chdir(feymail_homedir) == -1){
         die_control();
    }
*/
    command(0,1,smtpcommands); 
    return(0);
}
