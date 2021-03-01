#include "ros/ros.h"
#include <iostream>
#include "std_msgs/String.h"
#include "geometry_msgs/Twist.h"
#include "jn_smart/Ui.h"

extern "C"{
#include "open_interface.h"
#include "bt_uisensor.h"
}

#define MIN_DIS 250
#define MAX_DIS 300
//#define MAX_DIS 350

class UiMsg
{
public:
	UiMsg(ros::NodeHandle nh)
	{
	//Topic you want to publish
		vel_pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1000);
		ui_pub = nh.advertise<jn_smart::Ui>("/UItrasonic", 1000);
	//Topic you want to subscribe
		vel_sub = nh.subscribe("/cmd_vel", 1000, &UiMsg::vel_callback, this);
	}

	void ui_callback()
	{
		int dis[4] = {0};

		geometry_msgs::Twist move_cmd;
		move_cmd.linear.x=move_cmd.linear.y=move_cmd.linear.z=0;
		move_cmd.angular.x=move_cmd.angular.y=move_cmd.angular.z=0;
		dis[0] = num.num0;
		dis[1] = num.num1;
		dis[2] = num.num2;
		dis[3] = num.num3;
		
		if(flag == 1){
			if((MIN_DIS <= dis[0] && dis[0] <= MAX_DIS) || (MIN_DIS <= dis[1] && dis[1] <= MAX_DIS)){
				vel_pub.publish(move_cmd);
				ROS_INFO(" Front !!! ");
			}
		}
		if(flag == 2){
			if(MIN_DIS <= dis[2] && dis[2] <= MAX_DIS){
				vel_pub.publish(move_cmd);
				ROS_INFO(" Hehind !!! ");
			}
		}
	}

	void vel_callback(const geometry_msgs::Twist msg)
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
	
	void pub_ui_msg(read_par ui_par)
	{
		len_par = get_dis_val(ui_par);
		num.num0 = len_par.len[0];
		num.num1 = len_par.len[1];
		num.num2 = len_par.len[2];
		num.num3 = len_par.len[3];
		ui_pub.publish(num);
	}

private:
	ros::Publisher vel_pub;
	ros::Publisher ui_pub;
	ros::Subscriber vel_sub;
	int flag;
	read_par len_par;
	jn_smart::Ui num;

};

int main(int argc, char **argv)
{
	ros::init(argc, argv, "sub_scan");

	read_par ui_par;
	ui_par.fd[0] = open_uart(UART_DEV_0,B9600);
	ui_par.fd[1] = open_uart(UART_DEV_6,B9600);
	ui_par.fd[2] = open_uart(UART_DEV_5,B9600);
	ui_par.fd[3] = open_uart(UART_DEV_7,B9600);

	ros::NodeHandle n;
	ros::Rate loop_rate(10);

	UiMsg UIObject(n);

	while(ros::ok()){
		UIObject.pub_ui_msg(ui_par);
		UIObject.ui_callback();
		
		ros::spinOnce();
		loop_rate.sleep();
	}
	
	return 0;
}

