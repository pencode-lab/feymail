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


#ifndef FEYMAIL_STRING_H__
#define FEYMAIL_STRING_H__


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
feymail_string_equals("fuck,shit","Fuck,shit")  return:1
feymail_string_equals("fuck,shit","fuck,fuck")  return:0
*/     
extern int feymail_string_equals(char *s,char *t);


/*                                                                                                                                   
*Functionality:                                                                                                                      
*  from the string find the char that is The first match 
*Parameters:                                                                                                                         
*   [in] The string and The target character
*   [out] None.                                                                                                                      
*Returned value:                                                                                                                     
*   return the index 
*   if find target char, s[index]='the target char'
*   else s[index]='\0';
eg:
feymail_string_chr("fuck,shit",'B')  return:9    
feymail_string_chr("fucckkkkk",'k')  return:4
*/
extern unsigned int feymail_string_chr(char *s,int c);


/*                                                                                                                                   
*Functionality:                                                                                                                      
*  from the string find the char that is The last match 
*Parameters:                                                                                                                         
*   [in] The string and The target character
*   [out] None.                                                                                                                      
*Returned value:                                                                                                                     
*   return the index 
*   if find target char, s[index]='the target char'
*   else s[index]='\0';
eg:
feymail_string_rchr("FufccfkkkkkA",'k')  return:10
*/
extern unsigned int feymail_string_rchr(char *s,int n,int c);

#endif
