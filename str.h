#ifndef _STR_H_

#define _STR_H_

void str_trim_crlf(char *str);// qu chu \r\n
void str_split(const char *str, char *left, char *right, char c);//zi fu chuan fen ge
int str_all_space(const char *str);
void str_upper(char *str);
long long str_to_longlong(char *str);
unsigned int str_octal_to_uint(const char *str);

#endif

