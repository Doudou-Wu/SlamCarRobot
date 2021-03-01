#include "parsing_data.h"
#include "open_interface.h"

geometry_msgs::Twist parsing_data(unsigned char *buf)
{
	float speed, dis, angle;
	geometry_msgs::Twist msg;

	if(buf[0] != 0x55 || buf[5] != 0x55){
		printf("Incorrect data format\n");
		msg.linear.z = -1;
		return msg;
	}

	if(buf[1] == OWN_ID){
		switch(buf[2]){

			case F_speed:
				memset(&msg,0,sizeof(msg));
				speed = (buf[3]*256+buf[4])/100.00;
				msg.linear.x = speed;
				break;
			case B_speed:
				memset(&msg,0,sizeof(msg));
				speed = (buf[3]*256+buf[4])/100.00;
				speed = -speed;
				msg.linear.x = speed;
				break;
			case F_DIS:
				memset(&msg,0,sizeof(msg));
				dis = (buf[3]*256+buf[4])/100.00;
				speed = 0.1;
				msg.linear.x = speed;
				break;
			case B_DIS:
				memset(&msg,0,sizeof(msg));
				dis = (buf[3]*256+buf[4])/100.00;
				speed = -0.1;
				msg.linear.x = speed;
				break;
			case L_TURN:
				memset(&msg,0,sizeof(msg));
				angle = (buf[3]*256+buf[4]);
				msg.angular.z = 0.5;
				break;
			case R_TURN:
				memset(&msg,0,sizeof(msg));
				angle = (buf[3]*256+buf[4]);
				msg.angular.z = -0.5;
				break;
			case DOBOT_1:

				break;
			case DOBOT_2:

				break;
			case DOBOT_3:

				break;
			default:
				printf("no command \n");
				break;
		}
	}

	return msg;
}

