#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#include "open_interface.h"
#include "bt_uisensor.h"

struct Read_par get_dis_val(read_par par)
{
	int ret = 0;
	unsigned char  buf[10] = { 0 };
	char ui = 0x0 ;
	char data = 0x55 ;

	memset(buf, 0x00, sizeof(buf));
	memset(par.len, 0x00, sizeof(par.len));

	write(par.fd[0], &ui, sizeof(&data));
	write(par.fd[1], &ui, sizeof(&data));
	write(par.fd[2], &ui, sizeof(&data));
	write(par.fd[3], &data, sizeof(&data));
//	usleep(1000);
	ret = read(par.fd[0], buf, sizeof(buf));
	par.len[0] = buf[1]*256 + buf[2];

	ret = read(par.fd[1], buf, sizeof(buf));
	par.len[1] = buf[1]*256 + buf[2];
	
	ret = read(par.fd[2], buf, sizeof(buf));
	par.len[2] = buf[1]*256 + buf[2];
	
	ret = read(par.fd[3], buf, sizeof(buf));
	par.len[3] = buf[0]*256 + buf[1];

	return par;
}

