#!/usr/bin/env python

import rospy
from std_msgs.msg import String
import time

if __name__ == "__main__":
    rospy.init_node("rospy_udp_test")
    test_pub = rospy.Publisher("/chatter", String, queue_size=1)
    while not rospy.is_shutdown():
        test_pub.publish("arm->nvidia")
        rospy.loginfo("published: arm->nvidia")
        time.sleep(1)
