#include "head.h"

void child_process(int connfd)
{

	chdir(path);
	SDisk disk_server;
	disk_server.set_socket(connfd);
	disk_server.user_login();

	disk_server.disk_recvcmd();
	while(strcmp(disk_server.get_resp(),"quit") != 0)
	{
		if(strcmp(disk_server.get_recmd(),"push") == 0)
		{
		//	disk_server.disk_upload();
		}
		else if(strcmp(disk_server.get_recmd(),"download") == 0)
		{
		//	disk_server.disk_download();
		}
		else if(strcmp(disk_server.get_recmd(),"ls") == 0)
		{
			disk_server.disk_ls();
		}
		else if(strcmp(disk_server.get_recmd(),"cd") == 0)
		{
			/*
			 *if(cd dir)
			 *	disk_server.disk_cd(dir);
			 *else if(cd ..)
			 *	disk_server.disk_cdup();
			 *
			 */
		}
		else if(strcmp(disk_server.get_recmd(),"rm") == 0)
		{
			/*
			 *if(rm -r dir)
			 *	disk_server.disk_rmdir(dir);
			 *if(rm file)
			 *	disk_server.disk_rmfile(file);
			 *
			 */
		}	
	}
}














