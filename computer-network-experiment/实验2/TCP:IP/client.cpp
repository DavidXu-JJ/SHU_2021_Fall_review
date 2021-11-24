#include <stdio.h>
#include <unistd.h> 
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h> 
#include <arpa/inet.h> 
#define SERVPORT 3333
#define MAXDATASIZE 100 /*每次最大数据传输量 */
int main(){
	int sockfd, recvbytes;
	char buf[MAXDATASIZE]; 
	struct sockaddr_in serv_addr; 
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){ perror("socket 创建出错!");
		exit(1);
	}
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(SERVPORT);
	serv_addr.sin_addr.s_addr = INADDR_ANY; 
	memset(serv_addr.sin_zero, 0, sizeof(serv_addr.sin_zero)); 

	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) == -1) {
		perror("connect出错!");
		exit(1);
	}

	if ((recvbytes=recv(sockfd, buf, MAXDATASIZE, 0)) ==-1) { 
		perror("recv出错!");
		exit(1); 
	}
	buf[recvbytes] = '\0'; 
	printf("%s\n",buf);

	char s[MAXDATASIZE]; 
	scanf("%s", s); 	
	if (send(sockfd, s, strlen(s), 0) == -1) perror("send出错!");

	if ((recvbytes=recv(sockfd, buf, MAXDATASIZE, 0)) ==-1) { 
		perror("recv 出错!");
		exit(1); 
	}
	buf[recvbytes] = '\0'; 
	printf("%s\n", buf); 
	if(!strcmp(buf, "口令错误\0")){
		close(sockfd);
		return 0; 
	}
	
	memset(s, 0, sizeof s);
	scanf("%s", s); 
	if (send(sockfd, s, strlen(s), 0) == -1) perror("send出错!");
	while(1){
		printf("等待服务器响应...\n"); 
		if ((recvbytes=recv(sockfd, buf, MAXDATASIZE, 0)) ==-1) { 
			perror("recv 出错!");
			exit(1); 
		}
		buf[recvbytes] = '\0'; 
		if(!strcmp(buf,"服务器主动断开连接\n\0")){
			printf("%s\n",buf);
			break;
		}
		printf("Received: %s\n",buf);
		printf("你想干什么？\n1:给服务器发消息, 2:断开连接\n");
		int op;
		scanf("%d",&op);
		if(op==2){
			strcpy(s,"disconnect\0");
			if (send(sockfd, s, strlen(s), 0) == -1) perror("send出错!");
			break;
		}
		printf("请输入你要发送的消息:\n");
		scanf("%s", s); 	
		if (send(sockfd, s, strlen(s), 0) == -1) perror("send出错!");
	}
	close(sockfd);
	printf("结束进程\n");
}
