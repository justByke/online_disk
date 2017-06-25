#include "head.h"

int file_push(int sockfd,char *filename)
{
	char s[32];
	memset(s,'0',32);
	char temp[32];
	FILE *fp;

	if((fp =fopen(filename,"rb")) == NULL) {
		perror("file open failed\n");
		exit(-1);
	}

	fseek(fp,0,SEEK_END);
	int file_len = ftell(fp);
//	printf("%d\n",file_len);
	int i = 31;
	while(file_len != 0)
	{
		s[i--] = (file_len % 10)+48;
		file_len = file_len / 10;
	}	
	//printf("%s\n",s);
	send(sockfd,s,strlen(s),0);
	rewind(fp);

	char buf[BUFFER_SIZE];
	bzero(buf,BUFFER_SIZE);
	int len = 0;
	while((len = fread(buf,1,BUFFER_SIZE,fp)) > 0)
	{
		//write(1, buf, BUFFER_SIZE);

		if(send(sockfd,buf,len,0) < 0)
		{
			perror("send file failed\n");
			exit(-1);
		}

		bzero(buf,BUFFER_SIZE);
		printf(".\n");
	}

	fclose(fp);

	//close(sockfd);
	return 0;
}


