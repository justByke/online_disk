#include "head.h"

int main()
{
	int sockfd = socket_connect();

	while(1)
	{
		pid_t pid;
		struct sockaddr_in clientaddr;
		socklen_t len = sizeof(clientaddr);
		int connfd = accept(sockfd,(struct sockaddr *)&clientaddr,&len);
		if(connfd == -1)
		{
			perror("accept failed\n");
			continue;
		}
		else
		{
			cout<<"客户端连接成功"<<endl;

			pid = fork();
			if(pid < 0)
			{
				perror("fork failed\n");
			}
			else if(pid == 0)
			{
				/*
				 *子进程负责服务器的主要功能实现
				 *父进程主要负责接受客户端的连接
				 */
				child_process(); 
			}
			else
			{
				close(connfd);
			}

		}
	}

	return 0;
}


