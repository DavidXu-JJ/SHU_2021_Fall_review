#include <string.h>
#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h> 
#include <stdlib.h> 
#include <errno.h> 
#include <sys/wait.h> 
#include <arpa/inet.h> 
 
 #define lisnum 10   //最大连接客户端
 #define myport 2333 //启用的接口
 #define maxnum 1024 //最大字节接收数
// #define myip "**.**.**.**"
 using namespace std;
 
 class keyNode{
 public:
     int clientfd;
     string name;
     keyNode(): clientfd(0), name(""){}
     void init() {
         this->clientfd=0;
         this->name="";
     }
     keyNode& operator = (const keyNode& tmp) {
         if(&tmp != this) {
             this->clientfd=tmp.clientfd;
             this->name=tmp.name;
         }
         return *this;
     }
 };
 
 inline void init(const keyNode client[], const int &sockfd) {///初始化，关闭所有客户端连接
     for(int i = 0; i < lisnum; ++ i) {
         if(client[i].clientfd != 0) {
             close(client[i].clientfd);
         }
     }
     close(sockfd);
 }
 
//广播
 inline void allSend(const keyNode client[], const char buffer[], const int &maxn, const int &now) {
 ///将信息送到除了发送者以外的所有客户端上，参数：client[]是自定义类数组，buffer[]是传输字符串，maxn是目前连接数，now是发送者于client[]里的下标
     if(buffer[0] == '\0')///长度为0不转发
         return ;
     for(int i = 0; i < maxn; ++ i) {
         /* if(i != now) */
		 send(client[i].clientfd, buffer, strlen(buffer), 0);
     }
 }
 
 int main() {
     int sockfd;
     struct sockaddr_in my_addr;
     if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
         perror("socket()");
         return 0;
     }
     cout<<"socket ok"<<endl;
     my_addr.sin_family = AF_INET;
     my_addr.sin_port = htons(myport);
     my_addr.sin_addr.s_addr = INADDR_ANY;
 
     memset(my_addr.sin_zero, 0, sizeof(my_addr.sin_zero));
     if(bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1) {
         perror("bind()");
         return 0;
     }
     puts("bind ok");
     if(listen(sockfd, lisnum) == -1) {
         perror("lisnum()");
         return 0;
     }
     cout<<"listen ok"<<endl;
    
      //以上都是正常流程
 
     fd_set clientfd;
     int maxsock;
     keyNode IDkey[lisnum];   //存储客户端信息
 
     int cntfd = 0;  //客户端个数
     maxsock = sockfd;//
     char buffer[maxnum];
     string res;
     int len = 0;
     while(true) {
         FD_ZERO(&clientfd);
 
         FD_SET(sockfd, &clientfd); //把服务器自己加入文件描述符集合（fd_set）
 
         for(int i = 0; i < lisnum; ++i) {
             if(IDkey[i].clientfd != 0) {    //客户端加入文件描述符集合（fd_set）
                 FD_SET(IDkey[i].clientfd, &clientfd);
             }
         }
         
         //监听文件描述符集合（fd_set）中的变化
         if(select(maxsock + 1, &clientfd, NULL, NULL, NULL) < 0) {
             perror("select()");
             break;
         }
         
         //循环查找是哪个套接字发送了信息
         for(int i = 0; i < cntfd; ++i) {
             if(FD_ISSET(IDkey[i].clientfd, &clientfd)) {
                 len = recv(IDkey[i].clientfd, buffer, maxnum, 0);
                 if(len <= 0) {
                     close(IDkey[i].clientfd);
                     FD_CLR(IDkey[i].clientfd, &clientfd);
 
                     IDkey[i] = IDkey[--cntfd];
                     IDkey[cntfd].init();
                 } else {
                     buffer[len] = '\0';
                     res = IDkey[i].name + ": " + buffer;
					 cout << res << endl;
                     allSend(IDkey, res.c_str(), cntfd, i);
                 }
             }
         }
 
         if(FD_ISSET(sockfd, &clientfd)) {
             struct sockaddr_in client_addr;
             socklen_t sizes=1;
             int sock_client = accept(sockfd, (struct sockaddr*)(&client_addr), &sizes);
             if(sock_client < 0) {
                 perror("accept()");
                 continue;
             }
			 cout << "连接成功\n"; 
             if(cntfd < lisnum) {
               //recv接收客户端发送的密码
               int num = recv(sock_client, buffer, maxnum, 0);
               if(num == -1) {
                  perror("recv()");
                  return 0;
               }
               buffer[num]='\0';     
               if(strcmp(buffer,"123\0")){   //123为服务器口令密码
                  strcpy(buffer,"error\0");
                  send(sock_client, buffer, strlen(buffer), 0);   //发送error表示密码出错
                  continue;
               }
			   strcpy(buffer,"ok\0");
			   send(sock_client, buffer, strlen(buffer), 0);

               IDkey[cntfd++].clientfd = sock_client;
                              
               strcpy(buffer, "连接成功!\n");
                 send(sock_client, buffer, strlen(buffer), 0);
                 ///cout<<"new connection client["<<cntfd - 1<<"] "<<inet_ntoa(client_addr.sin_addr)<<":"<<ntohs(client_addr.sin_port)<<endl;
                 memset(buffer, 0, sizeof(buffer));
                 len = recv(sock_client, buffer, maxnum, 0);   //接收客户端发送的自己的用户名
                 if(len < 0) {
                     perror("revc()");
                     init(IDkey, sockfd);
                     return 0;
                 }
                 buffer[len] = '\0';
                 IDkey[cntfd - 1].name = buffer;
                 strcat(buffer, " 加入聊天室\n");
                 allSend(IDkey, buffer, cntfd, cntfd - 1);  //广播某某加入聊天室
                 maxsock = max(maxsock, sock_client);
             } else {
                 cout<<"人数超限\n"<<endl;
             }
         }
     }
     init(IDkey, sockfd);
     return 0;
 }

