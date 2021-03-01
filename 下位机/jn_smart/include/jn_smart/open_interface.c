#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <termios.h>
#include <fcntl.h>
#include <signal.h>

#include "open_interface.h"

int open_can(const char *dev_name)
{
	int s;
	struct sockaddr_can addr;
	struct ifreq ifr;

	if ( dev_name == NULL )
	{   
		printf("please enter can dev name \n");
	}   
	
	/* 创建套接字 */
	s = socket(PF_CAN, SOCK_RAW, CAN_RAW);

	/* 设置CAN接口名 */
	strcpy(ifr.ifr_name, dev_name);
	/* 确定接口索引 */
	ioctl(s, SIOCGIFINDEX, &ifr);

	addr.can_ifindex = ifr.ifr_ifindex;
	addr.can_family = AF_CAN;
	/* 生成的套接字与网络地址进行绑定 */
	bind(s, (struct sockaddr *)&addr, sizeof(addr));

	return s;
}


int open_uart(const char *dev_name,speed_t speed)
{
	int ret = 0;
	int fd = -1;
	struct termios uart_opt = { 0 };

	if ( dev_name == NULL )
	{   
		printf("please enter uart dev name \n");
	}   

	fd = open(dev_name, O_RDWR | O_NOCTTY);
	if(fd < 0){
		perror("Open");
		return -1; 
	}   

	//enable recv 
	uart_opt.c_cflag |= CLOCAL | CREAD;

	//data 8bit
	uart_opt.c_cflag &= ~CSIZE;
	uart_opt.c_cflag |= CS8;

	/*Parity NONE*/
	uart_opt.c_cflag &=~PARENB;

	/*Stop bit 1*/
	uart_opt.c_cflag &=~CSTOPB;

	uart_opt.c_cflag &=~CRTSCTS;
	uart_opt.c_cc[VTIME] = 6;
	uart_opt.c_cc[VMIN] = 0;

	cfsetispeed(&uart_opt, speed);
	cfsetospeed(&uart_opt, speed);

	tcflush(fd, TCIOFLUSH);

	ret = tcsetattr(fd, TCSANOW, &uart_opt);
	if(ret < 0)
		if(ret < 0)
		{
			return -1;
		}
	return fd;
}

int open_key(const char *dev_name)
{
	int key_fd = -1; 
	if ( dev_name == NULL )
	{   
		printf("please enter key dev name \n");
	}   

	key_fd = open(dev_name, O_RDONLY);
	if ( key_fd < 0 ) 
	{   
		return -1; 
	}   
	else
		return key_fd;
}

