#ifndef _SYS_UTIL_H_
#define _SYS_UTIL_H_

#include"common.h"

int tcp_client(unsigned short port);
int tcp_server(const char *host, unsigned short port);

int getlocalip(char *ip);

void ativate_nonblock(int fd);
void deativate_nonblock(int fd);

int read_timeout(int fd, unsigned int wait_seconds);
int write_timeout(int fd, unsigned int wait_seconds);
int accept_timeout(int fd, struct sockaddr_in *addr, unsigned int wait_seconds);
int connect_timeout(int fd, struct sockaddr_in *addr, unsigned int wait_seconds);

ssize_t readn(int fd, void *buf, size_t count);
ssize_t writen(int fd, const void *buf, size_t count);
ssize_t recv_peeK(int sockfd, void *buf, size_t len);
ssize_t readline(int sockfd, void *buf, size_t maxline);

void send_fd(int sockfd, int fd);
int recv_fd(const int sockfd);

#endif






