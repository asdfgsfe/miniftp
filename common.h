#ifndef _COMMON_H_
#define _COMMON_H_

#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include<unistd.h>
#include<sys/socket.h>
#include<fcntl.h>
#include<sys/types.h>
#include<errno.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<sys/select.h>
#include<sys/time.h>
#include<signal.h>

#include<pwd.h>
#include<ctype.h>
#include<shadow.h>
#include<crypt.h>

#include<dirent.h>
#include<time.h>
#include<sys/stat.h>


#define ERR_EXIT(m) \
do\
{\
        perror(m);\
        exit(EXIT_FAILURE);\
}       while(0)

#define MAX_COMMAND_LINE 1024
#define MAX_COMMAND 32
#define MAX_ARG 1024

#define MINIFTP_CONF "miniftpd.conf"

#endif
