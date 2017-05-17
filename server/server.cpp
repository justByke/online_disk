#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_PORT 8888
#define BUFFER_SIZE 1024

void print_time(char *ch);

int main(int argc,char **argv)
{
	struct sockaddr_in servaddr;
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
			pid = fork();
			if(pid < 0) {
				perror("fork failed\n");
			}
			else if(pid == 0) {
				char buf[BUFFER_SIZE];
				int data_len;
				FILE *fp = NULL;
				bzero(buf,BUFFER_SIZE);
				if((fp = fopen("data","wb"))==NULL)
				{
					perror("FILE open failed\n");
					exit(-1);
				}
				int size = 0;// block

				while(data_len = recv(connfd,buf,BUFFER_SIZE,0)) {
					if(data_len < 0) {
						perror("data recv mistake\n");
						exit(-1);
					}
					size++;
					if(size == 1)
					{
						printf("正在接收来自%s : %d 的文件\n",inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port));
					}					
					else
						printf(".");
					//向文件中写入
					int write_len = fwrite(buf,sizeof(char),data_len,fp);
					if(write_len > data_len) {
						printf("写入错误\n");
						exit(-1);
					}
					bzero(buf,BUFFER_SIZE);
				}
				
				if(size > 0)
					printf("%s : %d 的文件传送成功\n",inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port));
				else
					printf("%s : %d 的文件传送失败\n",inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port));
				fclose(fp);
				exit(0);
			}
			else {
				;
			}

		}
		close(connfd);
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
















