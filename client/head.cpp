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
    char res[32];
    char again[32];

LOGIN:
    cout << "Please input user_name" << endl;
    scanf("%s", &user_name);
    cout << "Please input password" << endl;
    scanf("%s", &user_pass);

    memset(m_cmd, 0, BUFF_SIZE);
    sprintf(m_cmd, "%s %s\n", user_name, user_pass);
    disk_sendcmd();        //发送指令
    disk_recvinfo();
    sscanf(m_resp, "%s", &res);
    if(strcmp(res,"-1") == 0) {
        cout<<"Login error!"<<endl;
        cout << "Login again? YES:again, else quit!" << endl;
        cin >> again;
        if (again == "YES") {
            goto LOGIN;
        } else {
            cout << "Welcome login again!" << endl;
            exit(1);
        }
    }
     cout << "Login success!" << endl;
}



int disk_pwd();            //显示当前目录
int disk_cd();             //更改目录
int disk_cdup();           //返回上层目录
int disk_mkdir();          //创建目录
int disk_rmdir();          //删除目录


int disk_upload();         //上传文件
int disk_download();       //下载文件
int disk_quit();           //退出登录


int CDisk:: disk_sendcmd()        //发送指令
{
    try {
        send(sockfd, m_cmd, strlen(m_cmd), 0);
    }catch(exception e) {
        cout << e.what() << endl;
    }
}
int CDisk:: disk_recvinfo()
{
    memset(m_resp, 0, BUFF_SIZE);
    try {
        recv(sockfd, m_resp, BUFF_SIZE, 0);
    } catch(exception e) {
         cout << e.what() << endl;
    }

}




