#!/bin/sh
arm-linux-gnueabihf-gcc bt_uisensor.c motor_contorl.c open_interface.c server.c parsing_data.cpp -fPIC -shared -o libjnsmart.so -I/opt/ros/kinetic/include
