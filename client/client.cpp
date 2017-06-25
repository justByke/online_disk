#include "head.h"

int main(int argc,char **argv)
{
	struct User user;

	if(argc != 5)
	{
		printf("usage: %s  addr  port  username passwd\n",argv[0]);
		exit(-1);
	}

	char filename[BUFFER_SIZE];
	struct Addr addr;

	strcpy(addr.host,argv[1]);
	addr.port = atoi(argv[2]);
	strcpy(user.user_name,argv[3]);
	strcpy(user.passwd,argv[4]);

	int sockfd = socket_connect(addr);

	int uid = check_login(sockfd,&user);
	if(uid <= 0)
	{
		perror("验证失败\n");
		exit(-1);
	}
	
	char s[32];
	bzero(s,32);
	recv(sockfd,s,32,0);
	s[32] = '\0';
//	printf("s: %s\n",s);
	int file_len = atoi(s);
//	printf("file_len : %d\n",file_len);
	char buf[BUFFER_SIZE];
	bzero(buf,BUFFER_SIZE);
	int data_len;
	int len = 0;
	while(data_len = recv(sockfd,buf,BUFFER_SIZE,0))
	{
		if(data_len < 0)
		{
			perror("data recv error\n");
			exit(-1);
		}
		fprintf(stdout,buf);
		bzero(buf,BUFFER_SIZE);
		len = len + data_len;
		if(len >= file_len)
			break;
	}

	char cmd[BUFFER_SIZE];
	struct Command command;
	while(fgets(cmd,BUFFER_SIZE,stdin) != NULL)
	{
		cmd[strlen(cmd)-1] = 0;
		if(strcmp(cmd,"quit") == 0)
			break;
		if(sscanf(cmd,"%s%s",&command.cmd,&command.file) != 2)
		{
			printf("input error\n");
			continue;
		}
	//	bzero(cmd,sizeof(BUFFER_SIZE));
	//	memcpy(cmd,&command,sizeof(struct Command));
		send(sockfd,cmd,strlen(cmd),0);
		printf("%s\t%s\n",command.cmd,command.file);
		if(strcmp(command.cmd,"push") == 0)
			file_push(sockfd,command.file);
		bzero(cmd,sizeof(BUFFER_SIZE));
	}	

	/*
	while(1)
	{
		printf("\n输入文件名:");
		fgets(filename,BUFFER_SIZE,stdin);
		filename[strlen(filename)-1] = 0;
		file_push(sockfd,filename);
	}
	*/
	return 0;
}

