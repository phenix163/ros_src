#!/usr/bin/env python
import rospy
from messanger.msg import mecab

def callback(msg):
    rospy.loginfo(rospy.get_caller_id()+"%s,%s",msg.word,msg.part)
    
def listener():

    # in ROS, nodes are unique named. If two nodes with the same
    # node are launched, the previous one is kicked off. The 
    # anonymous=True flag means that rospy will choose a unique
    # name for our 'listener' node so that multiple listeners can
    # run simultaenously.
    rospy.init_node('mecab', anonymous=True)

    rospy.Subscriber("mecab_res", mecab, callback)

    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()
        
if __name__ == '__main__':
    listener()
