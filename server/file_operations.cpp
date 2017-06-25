#include "head.h"

void push_file(int connfd,struct sockaddr_in clientaddr,struct User user,char file[64])
{ 
	char s[32];
	bzero(s,32);
	int file_len = 0;
	int fd = recv(connfd,s,32,0);
	if(fd == 0)
	{
		printf("客户端断开连接\n");
		exit(0);
	}
	file_len = atoi(s);
	char buf[BUFFER_SIZE];
	int data_len;
	FILE *fp = NULL;

	
	if((fp = fopen(file,"wb"))==NULL)
	{
		perror("FILE open failed\n");
		exit(-1);
	}

	int size = 0;// block
	int len = 0;
	bzero(buf,BUFFER_SIZE);
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
			len = len + data_len;
//			printf("%d\n",len);
			if(len >= file_len)
				break;
	}

	
	if(size > 0)
		printf("%s : %d 的文件传送成功\n",inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port));
	else
		printf("%s : %d 的文件传送失败\n",inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port));
	fclose(fp);
//	exit(0);

}

void list_file(int connfd,struct sockaddr_in clientaddr,struct User user)
{
	FILE *fp;
	chdir("./user_file");
	if((access(user.user_name,F_OK)) < 0)
	{
		if((mkdir(user.user_name,S_IRWXU)) < 0)
		{
			perror("mkdir error\n");
			exit(-1);
		}
	}

	char cmd[64] = "./";
	strcat(cmd,user.user_name);
	
	if(chdir(cmd) < 0)
		perror("chdir error\n");

	char commd[64] = "dir>";
	strcat(commd,user.user_name);
	strcat(commd,".txt");
	system(commd);	

	char file_name[64];
	strcpy(file_name,user.user_name);
	strcat(file_name,".txt");

	if((fp = fopen(file_name,"rb")) == NULL)
	{
		perror("file open error\n");
		exit(-1);
	}

	char s[32];
	memset(s,'0',32);
	s[32] = '\0';
//	printf("s : %s\n",s);
	fseek(fp,0,SEEK_END);
	int file_len = ftell(fp);
	printf("file_len : %d\n",file_len);
	int i = 31;
	while(file_len != 0)
	{
		s[i--] = (file_len % 10) + 48;
		file_len = file_len / 10;
	}

//	printf("s : %s\n",s);
	send(connfd,s,strlen(s),0);
	rewind(fp);

	char buf[BUFFER_SIZE];
	bzero(buf,BUFFER_SIZE);
	int len = 0;
	while((len = fread(buf,1,BUFFER_SIZE,fp)) > 0)
	{
		if(send(connfd,buf,len,0) < 0)
		{
			perror("send file failed\n");
			exit(-1);
		}

		bzero(buf,BUFFER_SIZE);
	}

}











