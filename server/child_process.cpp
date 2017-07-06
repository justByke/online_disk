#include "head.h"

void child_process(int sockfd)
{
	chdir(path);
	pid_t pid;
	struct sockaddr_in clientaddr;
	socklen_t  len = sizeof(clientaddr);
	int connfd = accept(sockfd,(struct sockaddr *)&clientaddr,&len);
	if(connfd == -1)
	{
		perror("accept failed\n");
		continue;
	}
	else 
	{
		cout<<"客户端链接成功"<<endl;

		pid = fork();
		if(pid < 0)
		{
			perror("fork failed\n");
		}
		else if(pid == 0)
		{
			SDisk disk_server;
			disk_server.set_socket(connfd);
			disk_server.user_login();

			disk_server.disk_recvcmd();
			while(strcmp(disk_server.get_resp(),"quit") != 0)
			{

			}
		}
		else
		{
			close(connfd);
		}
	}
}
