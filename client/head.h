#ifndef _HEAD_H
#define _HEAD_H

#define SERVER_IP "10.101.188.156"
#define SERVER_PORT 8888
#define BUFF_SIZE  256

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;


class CDisk
{
    public:

        CDisk();
        ~CDisk();

        int user_login();          //user login online_disk
        int disk_pwd();            //显示当前目录
        int disk_cd();             //更改目录
        int disk_cdup();           //返回上层目录
        int disk_mkdir();          //创建目录
        int disk_rmdir();          //删除目录
        int disk_upload();         //上传文件
        int disk_download();       //下载文件
        int disk_quit();           //退出登录
        int disk_sendcmd();        //发送指令
        int disk_recvinfo();

    private:

        int sockfd;
        string user_name;
        string user_pass;

        char m_cmd[BUFF_SIZE];
        char m_resp[BUFF_SIZE];


};


#endif
