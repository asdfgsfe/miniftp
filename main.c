#include"common.h"
#include"sysutil.h"
#include"session.h"
#include"str.h"
#include"tunable.h"
#include"parseconf.h"
#include"ftpproto.h"

int main(void)
{
	//list_comm测试
	//list_common();
	
	//字符串测试
	/*
	char str1[] = "    	a b";
	char str2[] = "        ";
	
	if(str_all_space(str1))
		printf("str1 is all space\n");
	else
	 	printf("str1 not all space\n");

	if(str_all_space(str2))
                printf("str2 is all space\n");
        else
                printf("str2 not all space\n");

	//char *str3 = "asdfge";// yun xing hui chu xian duan cuo wu you yu zhi zhen zhi xiang yi ge zi fu chuan chang liang,er chang liang bu neng bei xiu gai
	char str3[] = "abcdefg";
	str_upper(str3);
	printf("str3=%s\n", str3);

	long long result = str_to_longlong("1234567891234");
	printf("result=%lld\n", result);

	unsigned int re = str_octal_to_uint("12345");
	printf("%d\n", re);
	*/

	parseconf_load_file(MINIFTP_CONF);
	
	printf("tunable_pasv_enable=%d\n", tunable_pasv_enable);
	printf("tunable_port_enable=%d\n", tunable_port_enable);

	printf("tunable_listen_port=%u\n", tunable_listen_port);
	printf("tunable_max_clients=%u\n", tunable_max_clients);
	printf("tunable_accept_timeout=%u\n", tunable_accept_timeout);
	printf("tunable_connect_timeout=%u\n", tunable_connect_timeout);
	printf("tunable_idle_session_timeout=%u\n", tunable_idle_session_timeout);
	printf("tunable_data_connection_timeout=%u\n", tunable_data_connection_timeout);
	printf("tunable_local_umask=%u\n", tunable_local_umask);
	printf("tunable_upload_max_rate=%u\n", tunable_upload_max_rate);
	printf("tunable_download_max_rate=%u\n", tunable_download_max_rate);

	if(tunable_listen_address == NULL)
		printf("tunable_listen_address = NULL\n");
	else
		printf("tunable_listen_address = %s\n", tunable_listen_address);


	if(getuid() != (uid_t)0)
	{
		fprintf(stderr, "miniftpd:must be started as root\n");
		exit(EXIT_FAILURE);
	}

/*
typedef struct session
{
        //kong zhi lian jie
        uid_t uid;
        int ctrl_fd;
        char cmdline[MAX_COMMAND_LINE];
        char cmd[MAX_COMMAND];
        char arg[MAX_ARG];

        //fu zi jin cheng tong dao
        int parent_fd;
        int child_fd;

        //ftp协议状态
        int is_ascii;
} session_t;
*/

	session_t sess = 
	{
		0,-1, "", "", "",
		NULL, -1, -1,
		-1, -1,
		0
	};

	//处理僵尸进程
	signal(SIGCHLD, SIG_IGN);

	int listenfd = tcp_server(tunable_listen_address, tunable_listen_port);
	if(listenfd < 0)
		ERR_EXIT("tcp_server");

	int conn;
	pid_t pid;
	while(1)
	{
		conn = accept_timeout(listenfd, NULL, 0);
		if(conn == -1)
			ERR_EXIT("accept_timeout");

		pid = fork();
		if(pid < 0)
			ERR_EXIT("fork");
		if(pid == 0)
		{
			close(listenfd);
			sess.ctrl_fd = conn;
			begin_session(&sess);
		}
		else
			close(conn);

	}
	


	return 0;
}
