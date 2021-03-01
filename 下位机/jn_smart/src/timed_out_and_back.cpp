#include "ros/ros.h"
//#include "std_msgs/String.h"//geometry_msgs
#include "geometry_msgs/Twist.h"//包含elocity space消息
#include "math.h"

#include <sstream>
#include <iostream>

int main(int argc, char **argv)
{
	ros::init(argc,argv,"out_and_back");//指定节点“out_and_back”
	ros::NodeHandle n;//创造一个节点句柄
	ros::Publisher cmd_vel_pub=n.advertise<geometry_msgs::Twist>("/cmd_vel",1000);//将在/cmd_vel话题上发布一个geometry_msgs::Twist消息
	//ros::Publisher cmd_vel_pub=n.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel",1000);//将在/cmd_vel话题上发布一个geometry_msgs::Twist消息
	int rate=50;//定义更新频率
	ros::Rate loop_rate(rate);//更新频率50Hz，它会追踪记录自上一次调用Rate::sleep()后时间的流逝，并休眠直到一个频率周期的时间

	//初始化操作
	float linear_speed=0.2;//向前的线速度0.2m/s
	float goal_distance=1.0;//行进记录1.0m
	float linear_duration=goal_distance/linear_speed;//行进时间
	float angular_speed=1.0;//角度素1.0rad/s
	float goal_angle=M_PIl;//more /usr/include/math.h | grep pi可以查看pi的定义
	float angular_duration=goal_angle/angular_speed;//旋转时间

	geometry_msgs::Twist move_cmd;//定义消息对象
	move_cmd.linear.x=move_cmd.linear.y=move_cmd.linear.z=0;
	move_cmd.angular.x=move_cmd.angular.y=move_cmd.angular.z=0;


	int count=0;//记录循环次数
	while(ros::ok())//等待键盘ctrl+C操作则停止
	{
		std::cout<<"Hello world:"<<"The "<<count<<"th circle!"<<"\n";

		//向前运动
		move_cmd.linear.x=linear_speed;
		int ticks=int(linear_duration*rate);
		for(int i=0;i<ticks;i++)
		{
			cmd_vel_pub.publish(move_cmd);
			loop_rate.sleep();
		}

		//旋转前停止
		move_cmd.linear.x=0;
		cmd_vel_pub.publish(move_cmd);
		ros::Duration(1).sleep(); //休眠1s

		//旋转180度
		move_cmd.angular.z=angular_speed;//设置角速度
		ticks=int(goal_angle*rate);
		for(int i=0;i<ticks;i++)
		{
			cmd_vel_pub.publish(move_cmd);
			loop_rate.sleep();
		}

		//停止
		move_cmd.angular.z=0;
		cmd_vel_pub.publish(move_cmd);
		ros::Duration(1).sleep(); 

		count++;
	} 
	//使停止
	move_cmd.linear.x=0;
	move_cmd.angular.z=0;
	cmd_vel_pub.publish(move_cmd);
	ros::Duration(1).sleep(); 

	return 0;
}

