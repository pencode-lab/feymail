#include <feymail.h>
#include <feymail-string.h>
#include <feymail-fd.h>
#include <feymail-sys.h>
#include <feymail-alloc.h>


char inbuf[2048];
char outbuf[256];

datetime_sec starttime;
struct datetime dt;
unsigned long mypid;
unsigned long uid;
char *pidfn;
struct stat pidst;

int messfd;
int intdfd;

unsigned long messnum;
char *messfn;
char *todofn;
char *intdfn;

int flagmademess = 0;
int flagmadeintd = 0;



static int timeout = 1200;

static void print_debug()
{
    fprintf(stderr,"%s\n",strerror(errno));
}

static void die(e) int e; { print_debug();_exit(e);}


static int saferead(int fd,char *buf,int len)
{
    int r;

    //flush();                                                                                                                       
    r = feymail_timeoutread(timeout,fd,buf,len);                                                                                     
    if (r == -1) if (errno == ETIMEDOUT) die(10);                                                                                
    if (r <= 0) die(11);                                                                                                          
                                                                                                                                     
    return r;                                                                                                                        
}     

static int open_excl(char *fn)
{ 
    return open(fn,O_WRONLY | O_EXCL | O_CREAT,0644); 
}

static unsigned int pidfmt(char *s, unsigned long seq)
{
    unsigned int i;
    unsigned int len;

    len = 0;
    i = feymail_fmt_str(s,"pid/"); len += i; if (s) s += i;
    i = feymail_fmt_ulong(s,mypid); len += i; if (s) s += i;
    i = feymail_fmt_str(s,"."); len += i; if (s) s += i;
    i = feymail_fmt_ulong(s,starttime); len += i; if (s) s += i;
    i = feymail_fmt_str(s,"."); len += i; if (s) s += i;
    i = feymail_fmt_ulong(s,seq); len += i; if (s) s += i;
    ++len; if (s) *s++ = 0;

    return len;
}


static char *fnnum(char *dirslash,int flagsplit)
{
    char *s;

    s = feymail_alloc(feymail_fmtqfn((char *) 0,dirslash,messnum,flagsplit));
    if (!s) die(51);
    feymail_fmtqfn(s,dirslash,messnum,flagsplit);
    return s;
}


static void pidopen()
{
    unsigned int len;
    unsigned long seq;

    seq = 1;
    len = pidfmt((char *) 0,seq);
    pidfn = feymail_alloc(len);
    if (!pidfn) die(51);

    for (seq = 1;seq < 10;++seq){
        if (pidfmt((char *) 0,seq) > len) die(81); /* paranoia */
        pidfmt(pidfn,seq);
        messfd = open_excl(pidfn);
        if (messfd != -1) return;/*create success,return*/
    }

    fprintf(stderr,"open error:%s\n",strerror(errno));
    die(63);
}

static void create_queue_node()
{
    /*
    *1. use pid+
    *
    *
    */
}


int main()
{

    umask(033);
    if (chdir(feymail_homedir) == -1) die(61);
    if (chdir("queue") == -1) die(62);

    mypid = getpid();
    uid = getuid();
    starttime = feymail_now();
    feymail_datetime_tai(&dt,starttime);

    

    pidopen();
    if(fstat(messfd,&pidst) == -1) die(63);
    messnum = pidst.st_ino;

    messfn = fnnum("mess/",1);
    todofn = fnnum("todo/",0);
    intdfn = fnnum("intd/",0);

    fprintf(stderr,"pidfn[%s],messfn[%s]\n",pidfn,messfn);
    if (link(pidfn,messfn) == -1) die(64);
    if (unlink(pidfn) == -1) die(63);
    flagmademess = 1;

    fprintf(stderr,"messfn:%s\n",messfn);


    _exit(0);
}
