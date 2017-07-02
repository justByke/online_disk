#include "head.h"



CDisk::CDisk()
{
    struct sockaddr_in addr_ser;
    socklen_t addrlen  = sizeof(struct sockaddr);
    int res;

    cout << "Welcome to the Disk system";
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1) {
        perror("Connect error!");
        exit(1);
    }

    bzero(&addr_ser, sizeof(addr_ser));
    addr_ser.sin_family = AF_INET;
    addr_ser.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &addr_ser.sin_addr);

    res = connect(sockfd, (struct sockaddr *)&addr_ser, addrlen);
    if(res == -1) {
        perror("Connect error!");
        exit(1);
    }
}

CDisk:: ~CDisk()
{

}

int CDisk:: user_login()          //user login online_disk
{
    char user_name[32];
    char user_pass[32];

    cout << "Please input user_name" << endl;
    scanf("%s", &user_name);
    cout << "Please input password" << endl;
    scanf("%s", &user_pass);


    cout << user_name << user_pass << endl;

    sprintf(m_cmd, "%s %s\n", user_name, user_pass);
   // send(sockfd, m_cmd, strlen(m_cmd), 0);
   // recv(sockfd, m_resp, BUFF_SIZE, 0);

}



int disk_pwd();            //显示当前目录
int disk_cd();             //更改目录
int disk_cdup();           //返回上层目录
int disk_mkdir();          //创建目录
int disk_rmdir();          //删除目录
int disk_upload();         //上传文件
int disk_download();       //下载文件
int disk_quit();           //退出登录
int disk_sendcmd();        //发送指令
//int disk_recvinfo();




