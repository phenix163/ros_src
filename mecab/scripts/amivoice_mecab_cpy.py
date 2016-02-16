#!/usr/bin/env python
# -*- coding: utf-8 -*-
# license removed for brebity
import rospy
import time
import MeCab
import re
from std_msgs.msg import String
from zmq_ros.msg import zmq_time2
from zmq_ros.msg import mecab_amivoice2
from messanger.msg import mecab_all
def talker() :
	tagger = MeCab.Tagger("-Ochasen")
	before = ''
	ss = u''
	ss2 = u''
	pub = rospy.Publisher('mecab_res2', mecab_amivoice2, queue_size = 1000)
	rospy.init_node('mecab2', anonymous=True)
	r = rospy.Rate(10)
	msg = mecab_amivoice2()
	while True:
		#f = open('/home/yhirai/mecab/test2.txt')
		x = rospy.wait_for_message("zmq_publish2",zmq_time2)
		line2 = x.speech_dic
		before = "1"
		if before != line2:
			print line2
			node = tagger.parseToNode(line2)
			msg.all = line2
			msg.tracking_ID = x.trackingID
			msg.name = x.name
			i = 0
			while node:
				msg.word[i] = node.surface
				m = re.search('(.*,)(.*,)(.*,)(.*,)(.*,)(.*,)(.*,)(.*,)',node.feature)
				if m == None:
					m = re.search('(.*,)(.*,)(.*,)(.*,)(.*,)(.*,)',node.feature)	
				msg.part[i] = m.group(1).rstrip(',')
				print "%s %s"% (node.surface,node.feature)
				if m.group(1)  == '動詞,':
					msg.part2[i] = m.group(2).rstrip(',')
					msg.part3[i] = m.group(6).rstrip(',')
					msg.part4[i] = m.group(7).rstrip(',')
				elif m.group(1) == '名詞,':
					msg.part2[i] = m.group(2).rstrip(',')
				#	if m.group(3) == '人名,':
					msg.part3[i] = m.group(3).rstrip(',')
			#		msg.part4[i] = m.group(7).rstrip(',')
				#time.sleep(0.05)
				node = node.next
				i += 1
			msg.number = i-2;
			pub.publish(msg)
			before = line2
				
if __name__ == '__main__':
	try:
		talker()
	except rospy.ROSInterruptException: pass
	