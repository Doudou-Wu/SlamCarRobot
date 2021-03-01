#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 12345
#define SERVER_IP "192.168.0.192"

typedef struct Socket_par{
	unsigned char rx_buf[10];
	struct sockaddr_in fromaddr;
}socket_par;

int socket_open(void);
socket_par read_socket(int sd);
int send_socket(int sd,unsigned char * tx_buf,struct sockaddr_in fromaddr);

#endif
