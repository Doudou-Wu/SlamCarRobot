#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>

#include "motor_contorl.h"
#include "open_interface.h"

char send_data[8];

int set_speed(int pwm_speed)
{
	send_data[0]=0x00;    
	send_data[1]=0x28;    
	send_data[2]=0x00;    
	send_data[3]=0x00;    

	send_data[5]=pwm_speed&0x00FF;
	send_data[4]=pwm_speed>>8;    

	return 6;
}

int can_send(int socket_fd, char * can_command, int n, int motor_id)
{
	int nbytes;
	struct can_frame frame;

	frame.can_id = motor_id;
	frame.can_dlc = n;

	memcpy(frame.data, can_command, frame.can_dlc);

	nbytes = write(socket_fd, &frame, sizeof(frame));
	if (nbytes < 0) {
		perror("can raw soket write");
		return 1;
	}

	return 0;
}

int servo_on_off(int socket_fd,int key)
{
	send_data[0]=0x00;    
	send_data[1]=0x24;    
	send_data[2]=0x00;    
	send_data[3]=0x10;    
	send_data[4]=0x00;    
	if(key==1)
		send_data[5]=0x01;   

	if(key==0)
		send_data[5]=0x00; 

	can_send(socket_fd,send_data,6,MOTO_R);
	can_send(socket_fd,send_data,6,MOTO_L);

	usleep(250000);

	return 6;
}

void pwm_control(int socket_fd, int motor_l, int motor_r)
{
	int i;

	i=set_speed(motor_r);
	can_send(socket_fd,send_data,i,MOTO_R);

	i=set_speed(motor_l);
	can_send(socket_fd,send_data,i,MOTO_L);

	usleep(250000);

}

void max_speed(int socket_fd,int speed)
{
	send_data[0]=0x00;    
	send_data[1]=0x1e;    
	send_data[2]=0x00;    
	send_data[3]=0xac;    
	send_data[5]=speed&0x00FF;
	send_data[4]=speed>>8;    

	can_send(socket_fd,send_data,6,MOTO_R);
	can_send(socket_fd,send_data,6,MOTO_L);

	usleep(250000);

}

void set_par(int socket_fd,int addr,int val)
{
	send_data[0]=0x00;    
	send_data[1]=0x1e;    
	send_data[2]=0x00;    
	send_data[3]=addr&0x00FF;
	send_data[5]=val&0x00FF; 
	send_data[4]=val>>8;    

	can_send(socket_fd,send_data,6,MOTO_R);
	can_send(socket_fd,send_data,6,MOTO_L);

	usleep(250000);
}

int set_speed_l(int socket_fd,int speed)
{
    int i = 6;

    send_data[0]=0x00;
    send_data[1]=0x28;
    send_data[2]=0x00;
    send_data[3]=0x00;

    send_data[5]=speed&0x00FF;
    send_data[4]=speed>>8;

    can_send(socket_fd,send_data,i,MOTO_L);

    return 0;
}

int set_speed_r(int socket_fd,int speed)
{
    int i = 6;

    send_data[0]=0x00;
    send_data[1]=0x28;
    send_data[2]=0x00;
    send_data[3]=0x00;

    send_data[5]=speed&0x00FF;
    send_data[4]=speed>>8;

    can_send(socket_fd,send_data,i,MOTO_R);

    return 0;
}

#if 1
odom_par rcv_pos(int socket_fd)
{
	struct can_frame frame_l,frame_r,temp_frame;
	odom_par m_speed;
	int ret;

	memset(&m_speed,0,sizeof(&m_speed));
	memset(&frame_l,0,sizeof(struct can_frame));
	memset(&frame_r,0,sizeof(struct can_frame));
	memset(&temp_frame,0,sizeof(struct can_frame));

	m_speed.wh_l_buf = malloc(8);
	if(m_speed.wh_l_buf == NULL)
		printf("m_speed.wh_l_buf malloc error !!!\n");

	m_speed.wh_r_buf = malloc(8);
	if(m_speed.wh_r_buf == NULL)
		printf("m_speed.wh_r_buf malloc error !!!\n");

	ret = read(socket_fd, &temp_frame, sizeof(temp_frame));
	if(temp_frame.can_id == 0x601 && temp_frame.data[1] == 0xca){
//		memcpy(frame_r.data,temp_frame.data,8);
//		memcpy(m_speed.wh_r_buf,frame_r.data,8);
		memcpy(m_speed.wh_r_buf,temp_frame.data,8);
	}
	if(temp_frame.can_id == 0x602 && temp_frame.data[1] == 0xca){
//		memcpy(frame_l.data,temp_frame.data,8);
//		memcpy(m_speed.wh_l_buf,frame_l.data,8);
		memcpy(m_speed.wh_l_buf,temp_frame.data,8);
	}

	ret = read(socket_fd, &temp_frame, sizeof(temp_frame));
	if(temp_frame.can_id == 0x602 && temp_frame.data[1] == 0xca){
//		memcpy(frame_l.data,temp_frame.data,8);
//		memcpy(m_speed.wh_l_buf,frame_l.data,8);
		memcpy(m_speed.wh_l_buf,temp_frame.data,8);
	}
	if(temp_frame.can_id == 0x601 && temp_frame.data[1] == 0xca){
//		memcpy(frame_r.data,temp_frame.data,8);
//		memcpy(m_speed.wh_r_buf,frame_r.data,8);
		memcpy(m_speed.wh_r_buf,temp_frame.data,8);
	}

	return m_speed;
}

#endif
