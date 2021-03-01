#include <ros/ros.h>
#include <std_msgs/String.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Point.h>
#include <tf/transform_listener.h>
#include <nav_msgs/Odometry.h>
#include <sstream>
#include "parsing_data.h"

extern "C"{

#include "open_interface.h"
#include "bt_uisensor.h"
#include "parsing_data.h"

}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "pub_uart");
	ros::NodeHandle nh;
	std::string ui ="/cmd_vel";
	geometry_msgs::Twist msg;
	int fd;
	unsigned char rx_buf[10] = { 0x0 };
	unsigned char tx_buf[10] = {0x55,0x0,0x0,0x0,0x0,0x55};

	ros::Publisher pub_net = nh.advertise<geometry_msgs::Twist>("/cmd_vel",100);
	
	fd = open_uart(UART_DEV_9,B115200);
	ros::Rate loop_rate(10);
	while (ros::ok())
	{
		memset(rx_buf,0x00,10);	
		read(fd,rx_buf,sizeof(rx_buf));

		msg = parsing_data(rx_buf);

        	pub_net.publish(msg);
        	loop_rate.sleep();
	}

    return 0;
}

