#include "head.h"

int main(int argc,char **argv)
{
	struct User user;
	struct Control control;
	struct sockaddr_in servaddr,client_addr;
	char ch[64];
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERVER_PORT);
	servaddr.sin_addr.s_addr = htons(INADDR_ANY);

	int sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd < 0)
	{
		perror("socket falied!\n");
		exit(-1);
	}

	if(bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) == -1) {
		perror("bind failed!\n");
		exit(-1);
	}

	if(listen(sockfd,20)) {
		perror("listen failed!\n");
		exit(-1);
	}

	while(1) {
		pid_t pid;
		struct sockaddr_in clientaddr;
		socklen_t len = sizeof(clientaddr);
		int connfd = accept(sockfd,(struct sockaddr *)&clientaddr,&len);
		if(connfd == -1) {
			perror("accept failed!\n");
			continue;
		}

		else {
			printf("client %s : %d connect successful\n",inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port));
			print_time(ch);
			printf("加入的时间是:%s\n",ch);


			pid = fork();
			if(pid < 0) {
				perror("fork failed\n");
			}
			else if(pid == 0) {
				if(recv(connfd,(char *)&user,sizeof(struct User),0) < 0)
				{
					perror("shuju jie shou cuo wu\n");
					exit(-1);
				}
				printf("客户端发送过来的用户名是:%s,密码是:%s\n",user.user_name,user.passwd);
				if((user.uid = mysql_check_login(user)) <= 0)
				{
					printf("身份验证失败\n");
					exit(-1);
				}
				printf("身份验证成功\n");
				send(connfd,(char *)&user,sizeof(struct User),0);
			//在验证成功之后将这个客户的目录发送过去显示.	
				

				list_file(connfd,clientaddr,user);
				char cmd[20];
				char file[64];
				char line[BUFFER_SIZE];

				while((recv(connfd,line,BUFFER_SIZE,0)) > 0)
				{
				//	printf("%s\n",line);
					if(sscanf(line,"%s%s",cmd,file) == 2)
					{
				//		printf("%s%s\n",cmd,file);
						if(strcmp(cmd,"push") == 0)
							push_file(connfd,clientaddr,user,file);
					}
				}
				/*
				while(1)	
				{
					push_file(connfd,clientaddr);
				}
				exit(0);
				*/
			}
			else {
				close(connfd);
			}

		}
	}

	return 0;
}


void print_time(char *ch)
{
	time_t now;
	struct tm * stm;
	time(&now);
	stm = localtime(&now);
	sprintf(ch,"%02d:%02d:%02d\n",stm->tm_hour,stm->tm_min,stm->tm_sec);
	return ;
}












