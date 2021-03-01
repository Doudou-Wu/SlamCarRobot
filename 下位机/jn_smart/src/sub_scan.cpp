#include "ros/ros.h"
#include <iostream>
#include "std_msgs/String.h"
#include "sensor_msgs/LaserScan.h"
#include "geometry_msgs/Twist.h"

#define FORWARD_RIGHT_MIN 0.0
#define FORWARD_RIGHT_MAX 60.0
#define FORWARD_LEFT_MIN 300.0
#define FORWARD_LEFT_MAX 359.0
#define FORWARD_MIN_DIS 0.20
#define FORWARD_MAX_DIS 0.30

#define BACKWARD_MIN 150
#define BACKWARD_MAX 210
#define BACKWARD_MIN_DIS 0.55
#define BACKWARD_MAX_DIS 0.65

class StopMsg
{
public:
	StopMsg()
	{
	//Topic you want to publish
		pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 1000);
	//Topic you want to subscribe
		sub = n.subscribe("/scan", 1000, &StopMsg::callback, this);
		sub_1 = n.subscribe("/cmd_vel", 1000, &StopMsg::callback_1, this);
	}

	void callback(const sensor_msgs::LaserScan data)
	{
		geometry_msgs::Twist move_cmd;
		move_cmd.linear.x=move_cmd.linear.y=move_cmd.linear.z=0;
		move_cmd.angular.x=move_cmd.angular.y=move_cmd.angular.z=0;

		for (int i = 0; i < 360; i++){
			if(FORWARD_RIGHT_MIN <= data.intensities[i] && data.intensities[i] <= FORWARD_RIGHT_MAX){
				if(FORWARD_MIN_DIS < data.ranges[i] && data.ranges[i] < FORWARD_MAX_DIS && flag == 1){
					pub.publish(move_cmd);
					ROS_INFO(" Front !!! ");
				}
			}
			if(FORWARD_LEFT_MIN <= data.intensities[i] && data.intensities[i] <= FORWARD_LEFT_MAX){
				if(FORWARD_MIN_DIS < data.ranges[i] && data.ranges[i] < FORWARD_MAX_DIS && flag == 1){
					pub.publish(move_cmd);
					ROS_INFO(" Front !!! ");
				}
			}
			if(BACKWARD_MIN <= data.intensities[i] && data.intensities[i] <= BACKWARD_MAX){
				if(BACKWARD_MIN_DIS < data.ranges[i] && data.ranges[i] <  BACKWARD_MAX_DIS && flag == 2){
					pub.publish(move_cmd);
					ROS_INFO(" Hehind !!! ");
				}
			}
		}
	}

	void callback_1(const geometry_msgs::Twist msg)
	{
		flag = 0;
		if(msg.linear.x > 0 || msg.angular.z != 0){
			flag = 1;
		}
		if(msg.linear.x < 0){
			flag = 2;
		}
		if(msg.linear.x == 0 && msg.angular.z == 0){
			flag = 0;
		}
	}
private:
	ros::NodeHandle n;
	ros::Publisher pub;
	ros::Subscriber sub;
	ros::Subscriber sub_1;
	int flag ;
};

int main(int argc, char **argv)
{
	ros::init(argc, argv, "sub_scan");
	ros::NodeHandle n;
	ros::Rate loop_rate(10);

	StopMsg SMObject;

	while(ros::ok()){
		ros::spinOnce();
		loop_rate.sleep();
	}
	
	return 0;
}

