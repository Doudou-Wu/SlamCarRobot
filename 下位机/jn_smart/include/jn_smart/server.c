#include "server.h"

int socket_open(void)
{
	/*1 创建socket*/ 
	int sd = socket(PF_INET, SOCK_DGRAM, 0);
	if(sd == -1)
	{
		perror("socket failed");
		return -1;
	}
	/*2 准备地址*/
	struct sockaddr_in addr;
	addr.sin_family = PF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);// INADDR_ANY use htonl

	/*3 socket addr 绑定*/
	int res = bind(sd, (struct sockaddr *)&addr,sizeof(addr));
	if(res == -1)
	{
		perror("bind failed");
		return -1;
	}
	return sd;
}

socket_par read_socket(int sd)
{
	/* 进行通信*/
	socket_par buf_addr;
	int len = sizeof(buf_addr.fromaddr);

	memset(&buf_addr, 0, sizeof(buf_addr));
	recvfrom(sd, buf_addr.rx_buf, sizeof(buf_addr.rx_buf),0,(struct sockaddr *) &buf_addr.fromaddr,&len);
	return buf_addr;
}

int send_socket(int sd,unsigned char * tx_buf,struct sockaddr_in fromaddr)
{
	sendto(sd, tx_buf, sizeof(tx_buf), 0,(struct sockaddr *)&fromaddr,sizeof(fromaddr));

	return 0;
}

