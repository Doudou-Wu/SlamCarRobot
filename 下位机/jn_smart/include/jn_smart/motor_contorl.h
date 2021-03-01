#ifndef MOTOR_CONTORL_H
#define MOTOR_CONTORL_H

#include "open_interface.h"

#define MOTO_R 0x0101   // The right MOTO ID
#define MOTO_L 0x0102   // The left MOTO ID

#define OFFSET_6 16777216
#define OFFSET_4 65536
#define OFFSET_2 256

typedef struct Odom_par{
	int motor_l; //left speed
	int motor_r; //rigth speed
	int pos_l; //left position
	int pos_r; //rigth position
	unsigned char *wh_l_buf;
	unsigned char *wh_r_buf;
}odom_par;

int servo_on_off(int socket_fd,int key);//使能非使能
void pwm_control(int socket_fd, int motor1, int motor2);//两个轮胎速度
void max_speed(int socket_fd,int speed);//设置最大速度，不要太大
void set_par(int socket_fd,int addr,int val);
int set_speed_l(int socket_fd,int speed);
int set_speed_r(int socket_fd,int speed);
odom_par rcv_pos(int socket_fd);

#endif
