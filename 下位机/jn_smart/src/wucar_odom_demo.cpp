#include<ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include "jn_smart/Ui.h"

extern "C"{
#include "open_interface.h"
#include "motor_contorl.h"
}

double dt = 0.05;
double get_encode_count = 0;
double wheel_r =0;//上次的脉冲数据
double wheel_l =0;
double x = 0;
double y = 0;
double th = 0;

void chatterCallback(const jn_smart::Ui& odom_msg)//是一个回调函数，当接收到 chatter 话题的时候就会被调用。
{
	ros::NodeHandle n;
	tf::TransformBroadcaster odom_broadcaster;
	ros::Publisher odom_pub = n.advertise<nav_msgs::Odometry>("odom", 100);
	ros::Time current_time;
   		//ROS_INFO("wheel_r == %d, wheel_l == %d",odom_msg.wheel_r,odom_msg.wheel_l);
		if(get_encode_count == 0){
    		wheel_r = odom_msg.wheel_r;
    		wheel_l = odom_msg.wheel_l;
    		get_encode_count++;
    	}
    	double dwheel_r = odom_msg.wheel_r - wheel_r; //增量=这次收到的脉冲数据-上次的脉冲数据
    	double dwheel_l = odom_msg.wheel_l - wheel_l;
    	wheel_r = odom_msg.wheel_r; //这次收到的脉冲数据 变成 上次的脉冲数据
    	wheel_l = odom_msg.wheel_l;
    	current_time = ros::Time::now();
    	// last_time = current_time;
    	double dright = dwheel_r * 0.53 / 5606;
    	double dleft = dwheel_l * 0.53 / 5606;
    	double dxy_ave = (dright + dleft) / 2.0;
    	double dth = (dright - dleft) / 0.45;
    	double vxy = dxy_ave / dt;
    	double vth = dth / dt;
    	double dx = cos(th) * dxy_ave;
    	double dy = sin(th) * dxy_ave;
    	if (dth != 0){
    		th += dth;
    	}
    	x += dx;
    	y += dy;

		geometry_msgs::TransformStamped odom_trans;
	    odom_trans.header.stamp = current_time;
	    odom_trans.header.frame_id = "odom";
	    odom_trans.child_frame_id = "base_link";

    	geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromYaw(th);
	    odom_trans.transform.translation.x = x; //
	    odom_trans.transform.translation.y = y; //
	    odom_trans.transform.translation.z = 0.0;
	    odom_trans.transform.rotation = odom_quat;
	    odom_broadcaster.sendTransform(odom_trans);

	    nav_msgs::Odometry odom;
	    odom.header.stamp = current_time;
	    odom.header.frame_id = "odom";

	    odom.pose.pose.position.x = x;
	    odom.pose.pose.position.y = y;
	    odom.pose.pose.position.z = 0.0;
	    odom.pose.pose.orientation = odom_quat;

	    odom.child_frame_id = "base_link";
	    odom.twist.twist.linear.x = vxy; 
	    odom.twist.twist.linear.y = 0;
	    odom.twist.twist.angular.z = vth; 

    	odom_pub.publish(odom);
    	//ROS_INFO("x == %d, y == %d \n",x, y);
}



int main(int argc, char** argv){
    jn_smart::Ui num;

    ros::init(argc, argv, "odometry_publisher");
    ros::NodeHandle n;
    ros::Time current_time, last_time;
  	current_time = ros::Time::now();
  	// last_time = ros::Time::now();

    ros::Publisher odom_pub = n.advertise<nav_msgs::Odometry>("odom", 100);
	tf::TransformBroadcaster odom_broadcaster;
	ros::Subscriber odom = n.subscribe("/Odom_jn", 1000, chatterCallback);

	ros::spin();
    return 0;
}
