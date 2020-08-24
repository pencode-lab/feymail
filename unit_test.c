#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <feymail-string.h>

int main()
{
    char *s = "fuck,shit";
    char *t = "Fuck,shit";
    char *c = "fuck,fuck";
   

    int ret ;

    ret  = feymail_string_equals(s,t);
    printf("feymail_string_equals(\"%s\",\"%s\")  return:%d\n",s,t,ret);

    ret  = feymail_string_equals(s,c);
    printf("feymail_string_equals(\"%s\",\"%s\")  return:%d\n",s,c,ret);


    ret  = feymail_string_chr(s,'B');
    printf("feymail_string_chr(\"%s\",\'B\')  return:%d\n",s,ret);

    s="fucckkkkk\0";
    ret  = feymail_string_chr(s,'k');
    printf("feymail_string_chr(\"%s\",\'k\')  return:%d\n",s,ret);


    s="FufccfkkkkkA\0";
    ret  = feymail_string_rchr(s,strlen(s),'k');
    printf("feymail_string_rchr(\"%s\",\'k\')  return:%d\n",s,ret);

    int len =0;
    int i=0;
    char buffer[256]={0};
    char *sp=buffer;
    i = feymail_fmt_str(sp,"pid/"); len += i; if (sp) sp += i;

    i = feymail_fmt_ulong(sp,getpid());

    printf("feymail_fmt_str(\"%s\")  return:%d\n",buffer,len+i);
 


}
