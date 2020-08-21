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

#ifndef FEYMAIL_FD_H__
#define FEYMAIL_FD_H__

////////////////////////////////function////////////////////////////////////////   

/*                                                                                                                                   
*Functionality:                                                                                                                      
*   make it be a copy of from and Don't close the from
*Parameters:                                                                                                                         
*   [in] from and to fd                                                                                                                       
*   [out] None.                                                                                                                      
*Return
*  On success, return 1. 
*  else return 0.
*/
extern int feymail_fd_copy(int to,int from);


/*                                                                                                                                   
*Functionality:                                                                                                                      
*   make it be a copy of from and Close the from
*Parameters:                                                                                                                         
*   [in] from and to fd                                                                                                                       
*   [out] None.                                                                                                                      
*Return
*  On success, return 1. 
*  else return 0.
*/
extern int feymail_fd_move(int to,int from);


extern int feymail_timeoutread(int t,int fd,char *buf,int len);
extern int feymail_timeoutwrite(int t,int fd,char *buf,int len);

#endif
