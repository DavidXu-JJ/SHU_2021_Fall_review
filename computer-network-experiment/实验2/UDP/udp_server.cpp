#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#define SERV_PORT   2333
#define MAXDATASIZE 100 

int main()
{
	int sock_fd;

	if((sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
		perror("socket 创建出错"); exit(1); 
	}


	struct sockaddr_in addr_serv;
	int len;
	memset(&addr_serv, 0, sizeof(struct sockaddr_in)); 
	addr_serv.sin_family = AF_INET; 
		addr_serv.sin_port = htons(SERV_PORT); 
		addr_serv.sin_addr.s_addr = htonl(INADDR_ANY); 
	len = sizeof(addr_serv);

	if(bind(sock_fd, (struct sockaddr *)&addr_serv, sizeof(addr_serv)) < 0)
	{
		perror("bind error:");
		exit(1);
	}


	int recv_num;
	int send_num;
	char send_buf[MAXDATASIZE];
	char recv_buf[MAXDATASIZE];
	char password[MAXDATASIZE] = "123\0"; 
	char name[MAXDATASIZE]; 
	struct sockaddr_in addr_client;
	
	while(1)
	{
		printf("等待连接...\n");
		
		recv_num = recvfrom(sock_fd, recv_buf, sizeof(recv_buf), 0, (struct sockaddr *)&addr_client, (socklen_t *)&len);
	
		
		if(recv_num < 0){
				perror("recvfrom error:");
				exit(1);
			}
		recv_buf[recv_num] = '\0';
		
		printf("%s\n",recv_buf);

		strcpy(send_buf, "请输入口令:\n\0"); 
		send_num = sendto(sock_fd, send_buf, strlen(send_buf), 0, (struct sockaddr *)&addr_client, len);
		if(send_num < 0)
		{
			perror("sendto error:");
			exit(1);
		}


		recv_num = recvfrom(sock_fd, recv_buf, sizeof(recv_buf), 0, (struct sockaddr *)&addr_client, (socklen_t *)&len);

		if(recv_num < 0)
		{
			perror("recvfrom error:");
			exit(1);
		}
		recv_buf[recv_num] = '\0';

		if(strcmp(recv_buf, password)){
			strcpy(send_buf, "口令错误\n\0"); 
			/* send_num = sendto(sock_fd, send_buf, recv_num, 0, (struct sockaddr *)&addr_client, len); */
			send_num = sendto(sock_fd, send_buf, strlen(send_buf), 0, (struct sockaddr *)&addr_client, sizeof(struct sockaddr));
			if(send_num < 0)
				{
					perror("sendto error:");
					exit(1);
				}
			continue; 
		}

		printf("客户端口令输入正确，正在请求用户名...\n");
		strcpy(send_buf, "口令正确, 请输入用户名:\n\0"); 
		/* send_num = sendto(sock_fd, send_buf, recv_num, 0, (struct sockaddr *)&addr_client, len); */
		send_num = sendto(sock_fd, send_buf, strlen(send_buf), 0, (struct sockaddr *)&addr_client, sizeof(struct sockaddr));
		if(send_num < 0)
		{
			perror("sendto error:");
			exit(1);
		}


		recv_num = recvfrom(sock_fd, name, sizeof(name), 0, (struct sockaddr *)&addr_client, (socklen_t *)&len);

		if(recv_num < 0)
		{
			perror("recvfrom error:");
			exit(1);
		}
		name[recv_num] = '\0';
		
		printf("用户名为:%s\n",recv_buf);

		while(1){
			printf("你想干什么？\n1:给客户端发消息, 2:结束进程\n");
			int op; 
			scanf("%d", &op); 
			if(op == 2) {
				strcpy(send_buf, "服务器断开通信\0"); 	
				send_num = sendto(sock_fd, send_buf, strlen(send_buf), 0, (struct sockaddr *)&addr_client, sizeof(struct sockaddr));
				if(send_num < 0)
					{
						perror("sendto error:");
						exit(1);
					}
				break; 
			}
			printf("请输入你要发送的消息:\n");
			scanf("%s", send_buf); 	

			/* send_num = sendto(sock_fd, send_buf, recv_num, 0, (struct sockaddr *)&addr_client, len); */
			send_num = sendto(sock_fd, send_buf, strlen(send_buf), 0, (struct sockaddr *)&addr_client, sizeof(struct sockaddr));

			if(send_num < 0)
			{
				perror("sendto error:");
				exit(1);
			}

			printf("请等待客户端响应...\n"); 

			recv_num = recvfrom(sock_fd, recv_buf, sizeof(recv_buf), 0, (struct sockaddr *)&addr_client, (socklen_t *)&len);

			if(recv_num < 0)
			{
				perror("recvfrom error:");
				exit(1);
			}
			recv_buf[recv_num] = '\0';

			printf("%s:%s\n", name, recv_buf); 
			if(!strcmp(recv_buf, "客户端放弃发送\0")) {
				printf("客户端放弃发送\n");
				break;
			}	
		}
		
	}

	close(sock_fd);

	return 0;
}
