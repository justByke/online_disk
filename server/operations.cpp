#include "head.h"

int socket_connect()
{
	struct sockaddr_in addr;
	bzero(&addr,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8888);
	addr.sin_addr.s_addr = htons(INADDR_ANY);
	
	int sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd < 0)
		return -1;
	
	if((bind(sockfd,(struct sockaddr *)&addr,sizeof(addr))) == -1)
		return -1;

	if(listen(sockfd,20))
		return -1;

	return sockfd;
}


