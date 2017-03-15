#include"sysutil.h"

int tcp_client(unsigned short port)
{
	int sock;
        if((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0)
                ERR_EXIT("tcp_client");

	if(port > 0)
	{
		int on = 1;
        	if((setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&on, sizeof(on))) < 0)
                	ERR_EXIT("setsockopt");
		
		char ip[16] = {0};
		getlocalip(ip);

		struct sockaddr_in localaddr;
	        memset(&localaddr, 0, sizeof(localaddr));
        	localaddr.sin_family = AF_INET;
	        localaddr.sin_port = htons(port);
		localaddr.sin_addr.s_addr = inet_addr(ip);	
	
        	if((bind(sock, (struct sockaddr*)&localaddr, sizeof(localaddr))) < 0)
                	ERR_EXIT("bind");
	}
	return sock;
}

int tcp_server(const char *host, unsigned short port)
{
	int listenfd;
	if((listenfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
		ERR_EXIT("tcp_server");

	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;

	if(host != NULL)
	{
		if(inet_aton(host, &servaddr.sin_addr) == 0)
		{
			struct hostent *hp;
			hp = gethostbyname(host);
			if(hp == NULL)
				ERR_EXIT("gethostbyname");
			
			servaddr.sin_addr = *(struct in_addr*)hp->h_addr;
		}
	}
	else 
		servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	servaddr.sin_port = htons(port);
	
	//di zhi chong fu li yong
	int on = 1;
	if((setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&on, sizeof(on))) < 0)
		ERR_EXIT("setsockopt");

	if((bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) < 0)
		ERR_EXIT("bind");

	if((listen(listenfd, SOMAXCONN)) < 0) 
		ERR_EXIT("listen");

	return listenfd;
}

int getlocalip(char *ip)
{
	char hostIp[100] = {0};
	if(gethostname(hostIp,sizeof(hostIp)) < 0)
		return -1;
	struct hostent *hp;
	if((hp = gethostbyname(hostIp)) == NULL)
		return -1;
	strcpy(ip,inet_ntoa(*(struct in_addr*)hp->h_addr));
	printf("localIp=%s\n",ip);

	return 0;
}

void activate_nonblock(int fd)
{
	int ret;	
	int flags = fcntl(fd, F_GETFL);
	if(flags == -1)
		ERR_EXIT("fcntl");
		
	flags |= O_NONBLOCK;
	
	ret = fcntl(fd, F_SETFL, flags);
	if(ret == -1)
		ERR_EXIT("fcntl");
}

void deActivate_nonblock(int fd)
{
	int ret;	
	int flags = fcntl(fd, F_GETFL);
	if(flags == -1)
		ERR_EXIT("fcntl");
		
	flags &= ~O_NONBLOCK;
	
	ret = fcntl(fd, F_SETFL, flags);
	if(ret == -1)
		ERR_EXIT("fcntl");
}

int read_timeout(int fd, unsigned int wait_seconds)
{
	int ret;
	if(wait_seconds > 0)
	{
		fd_set read_fdSet;	
		struct timeval timeout;
		FD_ZERO(&read_fdSet);
		FD_SET(fd, &read_fdSet);
		timeout.tv_sec = wait_seconds;
		timeout.tv_usec = 0;
		
		do
		{
			ret = select(fd+1, &read_fdSet, NULL, NULL, &timeout);
		} while(ret < 0 && errno ==EINTR);
		
		if(ret == 0)
		{
			ret = -1;	
			errno = ETIMEDOUT;
		}	
		else if(ret == 1)
			ret = 0;
	}	
	return ret;
}

int write_timeout(int fd, unsigned int wait_seconds)
{
	int ret;
	if(wait_seconds > 0)
	{
		fd_set write_fdSet;	
		struct timeval timeout;
		FD_ZERO(&write_fdSet);
		FD_SET(fd, &write_fdSet);
		timeout.tv_sec = wait_seconds;
		timeout.tv_usec = 0;
		
		do
		{
			ret = select(fd+1, NULL, &write_fdSet, NULL, &timeout);
		} while(ret < 0 && errno ==EINTR);
		
		if(ret == 0)
		{
			ret = -1;	
			errno = ETIMEDOUT;
		}	
		else if( ret == 1)
			ret = 0;
	}
	
	return ret;
}

int accept_timeout(int fd, struct sockaddr_in *addr, unsigned int wait_seconds)
{
	int ret;
	socklen_t addrlen = sizeof(struct sockaddr_in);
	
	if(wait_seconds > 0)
	{
		fd_set accept_fdSet;		
		struct timeval timeout;
		FD_ZERO(&accept_fdSet);// FD_ZERO(fd, &accept_fdSet);
		FD_SET(fd, &accept_fdSet);
		timeout.tv_sec = wait_seconds;
		timeout.tv_usec = wait_seconds;
		
		do
		{
			ret = select(fd+1, &accept_fdSet, NULL, NULL, &timeout);	
		} while(ret < 0 && errno == EINTR );
		
		if(ret == -1)
			return -1;
		else if(ret == 0)
		{
			errno = ETIMEDOUT;	
			return -1;
		}		
	}
	
	if(addr != NULL)		
		ret = accept(fd, (struct sockaddr*)addr, &addrlen);
	else
		ret = accept(fd, NULL, NULL);
		if(ret == -1)
			ERR_EXIT("accept");
			
	return ret;
}

int connect_timeout(int fd, struct sockaddr_in *addr, unsigned int wait_seconds)
{
	int ret;
	socklen_t addrlen = sizeof(struct sockaddr_in);
	
	fd_set conn_fdSet;
	FD_ZERO(&conn_fdSet);//FD_ZERO(fd, conn_fdSet);
	FD_SET(fd, &conn_fdSet);
	
	if(wait_seconds > 0)
		activate_nonblock(fd);
	
	//printf("connaaaaaaaaaaaaaaaaaaaa\n");	
	ret = connect(fd, (struct sockaddr*)addr, addrlen);
	printf("errno=%d, EINPROGRESS=%d\n", errno,EINPROGRESS);
	if(ret < 0 && errno == EINPROGRESS)
	{
		//printf("connbbbbbbbbbbbbbbbbbbb\n");
		fd_set conn_fdSet;
		FD_ZERO(&conn_fdSet);//FD_ZERO(fd, conn_fdSet);
		FD_SET(fd, &conn_fdSet);
		struct timeval timeout;
		
		timeout.tv_sec = wait_seconds;
		timeout.tv_usec = 0;
		
		do 
		{
			ret = select(fd+1, NULL, &conn_fdSet, NULL, &timeout);	
		}	while( ret < 0 && errno == EINTR);
		
		if(ret == 0)
		{
			ret = -1;	
			errno = ETIMEDOUT;
		}	
		else if(ret < 0)
			return -1;
		else if(ret == 1)
		{
			/*
			ret返回-1，可能有两种情况，一种是连接建立成功，一种是套接
			字产生的，此时错误信息不会保存至errno变量中，因此需要调用getsockopt
			来获取套接字的错误
			*/
			int err;
			socklen_t socklen = sizeof(err);
			
			int sockOptRet = getsockopt(fd, SOL_SOCKET, SO_ERROR, &err, &socklen);
			if(sockOptRet == -1)
				return -1;
				
			if(err == 0)
				ret = 0;
			else 
			{
				errno = err;	
				ret = -1;
			}	
		}	
	}
	//printf("conncccccccccccccccccc\n");	
	if(wait_seconds > 0)
		deActivate_nonblock(fd);
	//printf("ret=%d\n", ret);	
	return ret;
}

ssize_t readn(int fd, void *buf, size_t count)
{
	size_t nlength = count;
	ssize_t nRead;
	char *pBuf = (char *)buf;

	while(nlength > 0)
	{
		if((nRead = read(fd,pBuf,nlength)) < 0)
		{
			if(errno == EINTR)
				continue;
			return -1;
		}
		else if(nRead == 0)
			return count - nlength;
		pBuf += nRead;
		nlength -= nRead;
	}
	return count;
}

ssize_t writen(int fd, const void *buf, size_t count)
{
        size_t nlength = count;
        ssize_t nWritten;
        char *pBuf = (char *)buf;

        while(nlength > 0)
        {
                if((nWritten = write(fd,pBuf,nlength)) < 0)
                {
                        if(errno == EINTR)
                                continue;
                        return -1;
                }
                else if(nWritten == 0)
                       continue;
                pBuf += nWritten;
                nlength -= nWritten;
        }
        return count;
}

ssize_t recv_peek(int sockfd, void *buf, size_t len)
{
	while(1)
	{
		int ret = recv(sockfd,buf,len,MSG_PEEK);
		if(ret == -1 && errno == EINTR)
			continue;
		return ret;
	}
}

ssize_t readline(int sockfd, void *buf, size_t maxLine)
{
	ssize_t nRead;
	char *pBuf = (char*)buf;
	int ret;
	ssize_t nLen = maxLine;

	while(1)
	{
		ret = recv_peek(sockfd,pBuf,nLen);
		if(ret < 0)
			return ret;
		else if(ret == 0)
			return ret;

		nRead = ret;
		int i;
		for(i=0; i<nRead; i++)
		{
			if(pBuf[i] == '\n')
			{
				ret = readn(sockfd,pBuf,i+1);
				if(ret != i+1)
					exit(EXIT_FAILURE);
				return ret;
			}
		}
		
		if(nRead > nLen)
			exit(EXIT_FAILURE);
		nLen -= nRead; 
		ret = readn(sockfd,pBuf,nRead);
		if(ret != nRead)
			exit(EXIT_FAILURE);

		pBuf += nRead;
	}
	return -1;
}

void send_fd(int sockfd, int fd)
{
	

	return;
}

int recv_fd(const int fd)
{

	return 0;
}


