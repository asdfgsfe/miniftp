#ifndef _SESSION_H_
#define _SESSION_H_

#include"common.h"

typedef struct session
{
	//kong zhi lian jie
	uid_t uid;
	int ctrl_fd;
	char cmdline[MAX_COMMAND_LINE];
	char cmd[MAX_COMMAND];
	char arg[MAX_ARG];

	//数据连接
	struct sockaddr_in *port_addr;
	int pasv_listen_fd;
	int data_fd;

	//fu zi jin cheng tong dao
	int parent_fd;
	int child_fd;

	//ftp协议状态
	int is_ascii;
} session_t;

void begin_session(session_t *sess);

#endif

