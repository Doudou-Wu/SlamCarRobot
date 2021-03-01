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
#include "server.h"
#include "parsing_data.h"

}

int main(int argc, char **argv)
{

    ros::init(argc, argv, "pub_net");
    ros::NodeHandle nh;
    std::string ui ="/cmd_vel";
	geometry_msgs::Twist msg;
	int sd;
	socket_par buf_addr;
	unsigned char rx_buf[10] = { 0 };
	unsigned char tx_buf[10] = {0x55,0x0,0x0,0x0,0x0,0x55};

	ros::Publisher pub_net = nh.advertise<geometry_msgs::Twist>("/cmd_vel",100);

	sd = socket_open();
	buf_addr = read_socket(sd);
	tx_buf[1] = OWN_ID;
	send_socket(sd,tx_buf,buf_addr.fromaddr);

    ros::Rate loop_rate(10);
    while (ros::ok())
    {
		buf_addr = read_socket(sd);
		memcpy(rx_buf,buf_addr.rx_buf,sizeof(rx_buf));

		msg = parsing_data(rx_buf);
		ROS_INFO("linear.x == %f",msg.linear.x);

        pub_net.publish(msg);
        loop_rate.sleep();

    }

    return 0;
}

