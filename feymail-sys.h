/*
Copyright (c) 2019 Pencode.net
Author: Zero <zero@bzline.cn> 

Feymail, a simple MTA for   High Performance Data transmission.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/


#ifndef FEYMAIL_SYS_H__
#define FEYMAIL_SYS_H__


#define feymail_wait_crashed(w) ((w) & 127)
#define feymail_wait_exitcode(w) ((w) >> 8)
#define feymail_wait_stopsig(w) ((w) >> 8)
#define feymail_wait_stopped(w) (((w) & 127) == 127)


typedef struct datetime {
  int hour;
  int min;
  int sec;
  int wday;
  int mday;
  int yday;
  int mon;
  int year;
} datetime;

typedef long datetime_sec;


////////////////////////////////function////////////////////////////////////////   

/*                                                                                                                                   
*Functionality:                                                                                                                      
*  String comparison, ignoring case..                                                                                                           
*Parameters:                                                                                                                         
*   [in] string.                                                                                                                       
*   [out] None.                                                                                                                      
*Returned value:                                                                                                                     
*   if equals return 1  else return 0.                                                                                                                

eg:
*/     
extern int feymail_wait_pid(int *wstat,int pid);

extern datetime_sec feymail_now();



extern void feymail_sig_pipeignore();
extern void feymail_sig_pipedefault();


#endif
