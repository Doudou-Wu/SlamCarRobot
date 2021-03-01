#include "ros/ros.h"
#include "geometry_msgs/Twist.h"

#include <sstream>
#include <iostream>

int main(int argc, char *argv[])
{
	ros::init(argc,argv,"keyboard_ctr");
	ros::NodeHandle n;
	ros::Publisher cmd_vel_pub=n.advertise<geometry_msgs::Twist>("/cmd_vel",1000);
	ros::Rate loop_rate(1);

	float linear_speed=0.1;
	float angular_speed=0.5;

	char cmd;
	
	geometry_msgs::Twist move_cmd;
	move_cmd.linear.x=move_cmd.linear.y=move_cmd.linear.z=0;
	move_cmd.angular.x=move_cmd.angular.y=move_cmd.angular.z=0;

	while(ros::ok())
	{
		cmd = getc(stdin);
		switch(cmd){
			case 'w':
				move_cmd.linear.x=linear_speed;
				move_cmd.angular.z=0;
				break;
			case 's':
				move_cmd.linear.x=(-linear_speed);
				move_cmd.angular.z=0;
				break;
			case 'a':
				move_cmd.linear.x=0;
				move_cmd.angular.z=0.5;
				break;
			case 'd':
				move_cmd.linear.x=0;
				move_cmd.angular.z=-0.5;
				break;
			case 'q':
				move_cmd.linear.x=0;
				move_cmd.angular.z=0;
				break;
			default:
				//ROS_INFO("no command");
				break;
		}
		cmd_vel_pub.publish(move_cmd);
		
		loop_rate.sleep();

	} 

	move_cmd.linear.x=0;
	move_cmd.angular.z=0;
	cmd_vel_pub.publish(move_cmd);

	return 0;
}

