#ifndef PARSING_DATA_H
#define PARSING_DATA_H

#include <ros/ros.h>
#include <std_msgs/String.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Point.h>

#include <stdio.h>

#define OWN_ID 0x1

#define F_speed 0x1
#define B_speed 0x2
#define F_DIS	0x3
#define B_DIS	0x4
#define L_TURN	0x5
#define R_TURN	0x6
#define DOBOT_1	0x7
#define DOBOT_2	0x8
#define DOBOT_3	0x9

geometry_msgs::Twist parsing_data(unsigned char *buf);

#endif
