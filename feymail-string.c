#include <feymail-string.h>

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
