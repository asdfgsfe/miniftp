#include"privparent.h"
#include"privsock.h"

static void privop_pasv_get_data_sock(session_t *sess);
static void privop_pasv_active(session_t *sess);
static void privop_pasv_listen(session_t *sess);
static void privop_pasv_accept(session_t *sess);

void handle_parent(session_t *sess)
{
	struct passwd *pw = getpwnam("nobody");
        if(pw == NULL)
        	return ;

        if(setgid(pw->pw_gid) < 0)
        	ERR_EXIT("setgid");

        if(setuid(pw->pw_uid) < 0)
        	ERR_EXIT("setuid");

	char cmd;
	while(1)
	{
		//read(sess->parent_fd, &cmd, 1);

		cmd = priv_sock_get_cmd(sess->parent_fd); 
		//解析内部命令
		//处理内部命令
		switch(cmd)
		{
			case PRIV_SOCK_GET_DATA_SOCK:
				privop_pasv_get_data_sock(sess);
				break;
			case PRIV_SOCK_GET_PASV_ACTIVE:
				privop_pasv_active(sess);
				break;
			case PRIV_SOCK_GET_DATA_LISTEN:
				privop_pasv_listen(sess);
				break;
			case PRIV_SOCK_GET_DATA_ACCEPT:
				privop_pasv_accept(sess);
				break;		
		}
	}
}

static void privop_pasv_get_data_sock(session_t *sess)
{
	/*nobody进程收到PRIV_SOCK_GET_DATA_SOCK命令
	进一步接收一个整数，也就是port
	接收一个字符串也就是ip
	fd=socket;
	bind(20);
	connect(port, ip);

	ok
	send_fd
	*/
}

static void privop_pasv_active(session_t *sess)
{}

static void privop_pasv_listen(session_t *sess)
{}

static void privop_pasv_accept(session_t *sess)
{}
