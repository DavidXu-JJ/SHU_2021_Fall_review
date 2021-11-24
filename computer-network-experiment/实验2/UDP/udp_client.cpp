#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define DEST_PORT 2333
#define DSET_IP_ADDRESS  "127.0.0.1"
#define MAXDATASIZE 100 

int main()
{
	int sock_fd;

	if((sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
		perror("socket 创建出错"); exit(1); 
	}

	struct sockaddr_in addr_serv;
	int len;
	memset(&addr_serv, 0, sizeof(addr_serv));
	addr_serv.sin_family = AF_INET;
	/* addr_serv.sin_addr.s_addr = inet_addr(DSET_IP_ADDRESS); */
	addr_serv.sin_addr.s_addr = htonl(INADDR_ANY); 
	addr_serv.sin_port = htons(DEST_PORT);
	len = sizeof(addr_serv);


	int send_num;
	int recv_num;
	char send_buf[MAXDATASIZE];
	char recv_buf[MAXDATASIZE];

	printf("发送请求...\n");
	strcpy(send_buf,"收到对话请求，客户端正在输入口令...\n\0");
	send_num = sendto(sock_fd, send_buf, strlen(send_buf), 0, (struct sockaddr *)&addr_serv, len);
	if(send_num < 0)
		{
			perror("sendto error:");
			exit(1);
		}
	recv_num = recvfrom(sock_fd, recv_buf, sizeof(recv_buf), 0, (struct sockaddr *)&addr_serv, (socklen_t *)&len);
	if(recv_num < 0)
		{
			perror("recvfrom error:");
			exit(1);
		}
	recv_buf[recv_num] = '\0';
	
	printf("%s\n", recv_buf); 
	
	scanf("%s", send_buf);
	
	send_num = sendto(sock_fd, send_buf, strlen(send_buf), 0, (struct sockaddr *)&addr_serv, len);
	if(send_num < 0)
		{
			perror("sendto error:");
			exit(1);
		}
	
	recv_num = recvfrom(sock_fd, recv_buf, sizeof(recv_buf), 0, (struct sockaddr *)&addr_serv, (socklen_t *)&len);
	if(recv_num < 0)
		{
			perror("recvfrom error:");
			exit(1);
		}
	recv_buf[recv_num] = '\0';
	
	printf("%s", recv_buf); 
	if(!strcmp(recv_buf, "口令错误\n\0")){
		close(sock_fd);
		return 0; 
	}
	
	scanf("%s", send_buf); 
	send_num = sendto(sock_fd, send_buf, strlen(send_buf), 0, (struct sockaddr *)&addr_serv, len);
	if(send_num < 0)
		{
			perror("sendto error:");
			exit(1);
		}
	while(1){
		printf("请等待服务器响应...\n"); 
		recv_num = recvfrom(sock_fd, recv_buf, sizeof(recv_buf), 0, (struct sockaddr *)&addr_serv, (socklen_t *)&len);
		
		if(recv_num < 0)
			{
				perror("recvfrom error:");
				exit(1);
			}
		recv_buf[recv_num] = '\0';
		if(!strcmp(recv_buf,"服务器断开通信\0")){
			close(sock_fd);
			return 0;
		}
			
		printf("received:%s\n", recv_buf); 
		
		printf("你想干什么？\n1:给服务器发消息, 2:结束进程\n");
		int op; 
		scanf("%d", &op); 
		if(op == 2) break; 
		
		printf("请输入你要发送的消息:\n");
		scanf("%s", send_buf); 	
		
		send_num = sendto(sock_fd, send_buf, strlen(send_buf), 0, (struct sockaddr *)&addr_serv, len);
		
		if(send_num < 0)
			{
				perror("sendto error:");
				exit(1);
			}
	}
	printf("结束进程\n");
	strcpy(send_buf, "客户端放弃发送\0"); 	
	send_num = sendto(sock_fd, send_buf, strlen(send_buf), 0, (struct sockaddr *)&addr_serv, len);
	if(send_num < 0)
		{
			perror("sendto error:");
			exit(1);
		}
	
	close(sock_fd);
	return 0;
}
