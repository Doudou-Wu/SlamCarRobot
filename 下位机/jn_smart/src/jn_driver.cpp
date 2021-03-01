#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "jn_smart/Ui.h"

extern "C"{
#include "open_interface.h"
#include "motor_contorl.h"
#include <pthread.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <sys/socket.h>
}

class JNdriver
{
public:
	JNdriver(ros::NodeHandle nh,int can_fd)
	{
		this->can_fd = can_fd;
		pthread_mutex_init(&mutex, NULL);
		sub = nh.subscribe("/cmd_vel", 1000, &JNdriver::chatterCallback,this);
		odom_pub = nh.advertise<jn_smart::Ui>("/Odom_jn", 1000);
		mvr = mvl = 0;
		flag = 0;

	}

        void pub_odom_msg()
        {
		pthread_mutex_lock(&mutex);
		flag = 0;
		odom_msg = rcv_pos(can_fd);
		num.wheel_r = (((unsigned int)odom_msg.wh_r_buf[5]) | ((unsigned int)odom_msg.wh_r_buf[4]<<8) | \
				((unsigned int)odom_msg.wh_r_buf[7]<<16)) | ((int)odom_msg.wh_r_buf[6]<<24);

		num.wheel_l = (((unsigned int)odom_msg.wh_l_buf[5]) | ((unsigned int)odom_msg.wh_l_buf[4]<<8) | \
				((unsigned int)odom_msg.wh_l_buf[7]<<16)) | ((int)odom_msg.wh_l_buf[6]<<24);

		odom_pub.publish(num);
		free(odom_msg.wh_l_buf);
		free(odom_msg.wh_r_buf);
		odom_msg.wh_l_buf = NULL;
		odom_msg.wh_r_buf = NULL;
		flag = 1;
		pthread_mutex_unlock(&mutex);
        }

	void chatterCallback(const geometry_msgs::Twist& msg)
	{
		float vr,vl;

		if(msg.linear.x > 0 && msg.angular.z == 0){
			mvr=mvl=M_speed(msg.linear.x);
		}

		if(msg.linear.x < 0 && msg.angular.z == 0){
			mvr=mvl=M_speed_rev(msg.linear.x);
		}

		if(msg.linear.x > 0 && msg.angular.z != 0){
			vr = Forward_turn_R(msg.linear.x,msg.angular.z);
			vl = Forward_turn_L(msg.linear.x,msg.angular.z);

			mvr = M_speed(vr);
			mvl = M_speed(vl);
		}

		if(msg.linear.x < 0 && msg.angular.z != 0){
			vr = Backward_turn_R(msg.linear.x,msg.angular.z);
			vl = Backward_turn_L(msg.linear.x,msg.angular.z);

			mvr = M_speed_rev(vr);
			mvl = M_speed_rev(vl);
		}

		if(msg.angular.z > 0 && msg.linear.x == 0){
			vr = msg.angular.z*L/2; 
			mvr = M_speed(vr);
			mvl = -mvr;
		}

		if(msg.angular.z < 0 && msg.linear.x == 0){
			vr = msg.angular.z*L/2;
			mvr = M_speed(vr);
			mvl = -mvr;
		}

		if(msg.angular.z == 0 && msg.linear.x == 0){
			mvl = 0;
			mvr = 0;
		}

		pthread_mutex_lock(&mutex);
		memset(&nop_buf,0,sizeof(struct can_frame));	
		if(flag == 1){
			set_speed_l(can_fd,mvl);
			read(can_fd, &nop_buf, sizeof(nop_buf));
			set_speed_r(can_fd,mvr);
			read(can_fd, &nop_buf, sizeof(nop_buf));
		}
		pthread_mutex_unlock(&mutex);

	}
private:
	ros::Publisher odom_pub;
	ros::Subscriber sub;
	odom_par odom_msg;
	jn_smart::Ui num;
	int can_fd;
	pthread_mutex_t mutex;

	int flag;
	int mvr,mvl;
	struct can_frame nop_buf;
};

int main(int argc, char **argv)
{
	int can_fd;
	can_fd = open_can(CAN_DEV);
	servo_on_off(can_fd,1);

	ros::init(argc, argv, "jn_driver");
	ros::NodeHandle n;
	ros::Rate loop_rate(20);
//	ros::Rate loop_rate(10);

	JNdriver JNdriverObject(n,can_fd);

	ros::AsyncSpinner spinner(3);
	spinner.start();

	while(ros::ok()){
		JNdriverObject.pub_odom_msg();		

		ros::spinOnce();
		loop_rate.sleep();
	}

	ros::waitForShutdown();
	servo_on_off(can_fd,0);
	close(can_fd);

	return 0;
}
