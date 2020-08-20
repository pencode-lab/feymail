#include <feymail.h>

void feymail_print_version()
{
    fprintf(stderr,"\r\n");
    fprintf(stderr,"%s\r\n\r\n",VERSION);
    fprintf(stderr,"***************Warning, Warning, Warning************************\r\n\r\n");
    fprintf(stderr,"%s","feymail - Similar mail system for High Performance Data transmission.\n");
    fprintf(stderr,"%s","Copyright (C) 2020 Pencode.net\r\n\r\n");
    fprintf(stderr,"*****************************************************************\r\n\r\n");

    fprintf(stderr,"\r\n\r\n");
}
