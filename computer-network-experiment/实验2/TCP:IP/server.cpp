#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <errno.h> 
#include <string.h> 
#include <sys/types.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <sys/wait.h> 
#include <arpa/inet.h> 
#define SERVPORT 3333
#define BACKLOG 10
#define MAXDATASIZE 100 
int main(){
	int sockfd, client_fd; 
	struct sockaddr_in my_addr; 
	struct sockaddr_in remote_addr; 
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		perror("socket 创建出错"); exit(1); 
	}
	my_addr.sin_family = AF_INET; 
	my_addr.sin_port = htons(SERVPORT);
	my_addr.sin_addr.s_addr = INADDR_ANY;
	memset(my_addr.sin_zero, 0, sizeof(my_addr.sin_zero)); 

	remote_addr.sin_family = AF_INET; 
	remote_addr.sin_port = htons(SERVPORT);
	remote_addr.sin_addr.s_addr = INADDR_ANY;

	memset(remote_addr.sin_zero, 0, sizeof(remote_addr.sin_zero)); 
	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1) { perror("bind 出错!");
		exit(1);
	}
	if (listen(sockfd, BACKLOG) == -1) {
		perror("监听出错!");
		exit(1);
	}
	printf("监听成功\n"); 
	socklen_t sin_size = sizeof(struct sockaddr_in);
	while(1){
		while(1) {
			printf("你是否想关闭服务器？1:不关闭，2:关闭\n");
			int op;
			scanf("%d",&op);
			if(op==2){
				printf("服务器已关闭");
				close(sockfd);
				return 0;
			}
			if ((client_fd = accept(sockfd, (struct sockaddr *)&remote_addr, &sin_size)) == -1) {
				perror("accept 出错");
				continue;
			}
			printf("连接成功\n"); 
			printf("received a connection from %s\n", inet_ntoa(remote_addr.sin_addr)); 
			break; 
		}
		char password[MAXDATASIZE]; 
		strcpy(password, "123\0"); 	//密码
		char s[MAXDATASIZE]; 
		char buf[MAXDATASIZE]; 
		strcpy(s, "请输入服务器口令:\0"); 
		int len = strlen(s); 
		if (send(client_fd, s, len, 0) == -1) perror("send出错!");
		int recvbytes; 
		if ((recvbytes=recv(client_fd, buf, MAXDATASIZE, 0)) ==-1) {  
			perror("recv 出错!"); 
			exit(1); 
		} 
		buf[recvbytes] = '\0'; 
		if(strcmp(password, buf)){
			printf("对方口令错误\n已断开连接\n"); 
			strcpy(s, "口令错误\0"); 
			if (send(client_fd, s, strlen(s), 0) == -1) perror("send出错!");
			close(client_fd);
			continue; 
		}
		strcpy(s, "请输入你的用户名:\0"); 
		len = strlen(s); 
		if (send(client_fd, s, len, 0) == -1) perror("send出错!");
		
		char name[MAXDATASIZE]; 
		if ((recvbytes=recv(client_fd, name, MAXDATASIZE, 0)) ==-1) {  
			perror("recv 出错!"); 
			exit(1); 
		} 
		name[recvbytes] = '\0'; 
		int op=0;
		while(1) {
			printf("你想干什么？\n1:给客户端发消息, 2:断开连接\n"); 
			scanf("%d",&op);
			if(op == 2) break; 
			printf("请输入你要发送的消息:");
			scanf("%s",s); 
			len = strlen(s); 
			s[len] = '\n'; 
			++ len; 
			if (send(client_fd, s, len, 0) == -1) perror("send出错!");
	
			printf("等待客户端响应...\n");
			if ((recvbytes=recv(client_fd, buf, MAXDATASIZE, 0)) ==-1) {  
				perror("recv出错!"); 
				exit(1); 
			} 
			buf[recvbytes] = '\0'; 
			if(!strcmp("disconnect\0", buf))
				break; 

			printf("%s:",name);  
			printf("%s\n\n", buf); 
		}
		if(op==2){
			printf("已与客户端断开连接\n"); 
			strcpy(s, "服务器主动断开连接\n\0"); 
			len = strlen(s); 
			if(send(client_fd, s, len, 0) == -1){
				perror("send 出错!"); 
			}
		}else{
			printf("客户端断开连接\n");
		}
		close(client_fd);
	}
} 
