/*
Copyright (c) 2019 Pencode.net
Author: Zero <stone@bzline.cn> 

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

#ifndef FEYMAIL_H__ 
#define FEYMAIL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/signal.h>
#include <errno.h>
#include <sys/time.h>                                                                                                                
#include <sys/types.h>                                                                                                               
#include <sys/socket.h>                                                                                                              
#include <netinet/in.h>                                                                                                              
#include <arpa/inet.h>                                                                                                               
#include <netdb.h>                                                                                                                   
#include <sys/uio.h>                                                                                                                 
#include <sys/stat.h>                                                                                                                
#include <sys/mman.h>                                                                                                                
#include <strings.h>                                                                                                                 
#include <semaphore.h>                                                                                                               
#include <pthread.h>     

#define VERSION "feymail version is V0.01.20200820.01"



////////////////////////////////variable define///////////////////////////////////////////
typedef enum {
    false=0,
    true=1
}bool;



////////////////////////////////function////////////////////////////////////////
/*
*Functionality:
*   Print current version.
*Parameters:
*   [in] None.                                                                                 
*   [out] None.
*Returned value:                                                                                                                     
*   None.                                                                                                                            
*/
void feymail_print_version();




#ifdef __cplusplus
}
#endif



#endif
