#include <feymail-string.h>

static int auto_split=8;


unsigned int feymail_strlen(char *s)
{
    char *t;

    t = s;
    for (;;) {
        if (!*t) return t - s; ++t;
        if (!*t) return t - s; ++t;
        if (!*t) return t - s; ++t;
        if (!*t) return t - s; ++t;
    }
}



static int case_diffs(s,t)
register char *s;
register char *t;
{
  register unsigned char x;
  register unsigned char y;

  for (;;) {
    x = *s++ - 'A';
    if (x <= 'Z' - 'A') x += 'a'; else x += 'A';
    y = *t++ - 'A';
    if (y <= 'Z' - 'A') y += 'a'; else y += 'A';
    if (x != y) break;
    if (!x) break;
  }
  return ((int)(unsigned int) x) - ((int)(unsigned int) y);
}

static unsigned int str_chr(s,c)
register char *s;
int c;
{
  register char ch;
  register char *t;

  ch = c;
  t = s;
  for (;;) {
    if (!*t) break; if (*t == ch) break; ++t;
    if (!*t) break; if (*t == ch) break; ++t;
    if (!*t) break; if (*t == ch) break; ++t;
    if (!*t) break; if (*t == ch) break; ++t;
  }
  return t - s;
}

unsigned int str_rchr(s,n,c)
char *s;
register unsigned int n;
int c;
{
  register char ch;
  register char *t;
  register char *u;

  ch = c;
  t = s;
  u = 0;
  for (;;) {
    if (!n) break; if (*t == ch) u = t; ++t; --n;
    if (!n) break; if (*t == ch) u = t; ++t; --n;
    if (!n) break; if (*t == ch) u = t; ++t; --n;
    if (!n) break; if (*t == ch) u = t; ++t; --n;
  }
  if (!u) u = t;
  return u - s;
}



int feymail_string_equals(char *s,char *t) {
    return (!case_diffs((s),(t)));
}

unsigned int feymail_string_chr(char *s,int c)
{
    return str_chr(s,c);
}

unsigned int feymail_string_rchr(char *s,int n,int c)                                                                                       
{                                                                                                                                    
    return str_rchr(s,n,c);                                                                                                             
} 



unsigned int feymail_fmt_str(char *s,char *t)
{
    register unsigned int len;
    char ch;

    len = 0;
    if (s) { while (ch = t[len]) s[len++] = ch; }
    else 
        while (t[len]) len++; //only return target string's len

    return len;
}


unsigned int feymail_fmt_ulong(char *s,unsigned long u)
{
    register unsigned int len; register unsigned long q;

    len = 1; q = u;

    while (q > 9) { ++len; q /= 10; }
    if (s) {
        s += len;
        do { *--s = '0' + (u % 10); u /= 10; } while(u); /* handles u == 0 */
    }
    return len;
}


unsigned int feymail_fmtqfn(char *s,char *dirslash,unsigned long id,int flagsplit)
{
    unsigned int len;
    unsigned int i;

    len = 0;

    i = feymail_fmt_str(s,dirslash); len += i; if (s) s += i;
    if (flagsplit){
        i = feymail_fmt_ulong(s,id % auto_split); len += i; if (s) s += i;
        i = feymail_fmt_str(s,"/"); len += i; if (s) s += i;
    }
    i = feymail_fmt_ulong(s,id); len += i; if (s) s += i;
    if (s) *s++ = 0; ++len;
    return len;
}
