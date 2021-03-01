#ifndef OPEN_INTERFACE_H
#define OPEN_INTERFACE_H

#include <termios.h>

#define CAN_DEV "can0"

#define BT_DEVICE "/dev/ttyS5"
#define BT_DEVICE_4 "/dev/ttyS4"

#define UART_DEV_0 "/dev/ttyS0"
#define UART_DEV_1 "/dev/ttyS1"
#define UART_DEV_3 "/dev/ttyS3"
#define UART_DEV_4 "/dev/ttyS4"
#define UART_DEV_6 "/dev/ttyS6"
#define UART_DEV_5 "/dev/ttyS5"
#define UART_DEV_7 "/dev/ttyS7"
#define UART_DEV_8 "/dev/ttyS8"
#define UART_DEV_9 "/dev/ttyS9"

#define STOP_KEY "/dev/input/event0"

#define P 0.53 //perimeter

#define M_speed(V) (int)(((60*V)/P)+0.5) //m/s--> rpm
#define M_speed_rev(V) (int)(((60*V)/P)-0.5) // -m/s--> rpm

#define L 0.482 //wheel distance

#define Forward_turn_R(V,W) (V+L*W/2)  //Turn right speed
#define Forward_turn_L(V,W) (V-L*W/2)  //Turn left speed
#define Backward_turn_R(V,W) (V-L*W/2)
#define Backward_turn_L(V,W) (V+L*W/2)

int open_can(const char *dev_name);//打开can
int open_uart(const char *dev_name,speed_t speed);//打开uart（bt和超声）
int open_key(const char *dev_name);//打开按键设备（急停）

#endif

