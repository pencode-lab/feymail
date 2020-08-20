#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include <feymail.h>
#include <feymail-string.h>


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

static void smtp_helo(char *argv)
{
    return ;
}

static void smtp_data(char *argv)
{
    return ;
}

commands smtpcommands[] = {
    { "helo", smtp_helo, 0 },
    { "data", smtp_data, flush },
    { 0, err_unimpl, flush }
} ;




static bool command(int fin,commands *c)
{
    char *cmd ="HELO";
    int i;
    char *arg;

    for(;;){

        /*from fin line then parse cmd and arg
        * line like: HELO stone
        * line like :rcpt to:<stone@bzline.com>
        */


        /**/

        for (i = 0;c[i].text;++i) 
            if (feymail_string_equals(c[i].text,cmd)) break;

        c[i].fun(arg);
        if (c[i].flush) c[i].flush();
    }


    return true;
}


int main(void)
{

    char *arg =" from: stone@bzline.cn";
    char addr[64]={0};

    unsigned int addr_len= addrparse(arg,addr);

    printf("addr = [%s][%d]\n",addr,addr_len);

    command(0,smtpcommands); 
    return(0);
}
