#!/usr/bin/env python
# -*- coding: utf-8 -*-
# license removed for brebity
import rospy
import time
import MeCab
import re
import socket
from std_msgs.msg import String

host = '133.19.23.117'
port = 10500
from messanger.msg import mecab_all
def talker() :
	clientsock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	clientsock.connect((host,port))
	tagger = MeCab.Tagger("-Ochasen")
	before = ''
	ss = u''
	ss2 = u''
	ok = 0
	line2 = ''
	pub = rospy.Publisher('mecab_res', mecab_all, queue_size = 1000)
	rospy.init_node('mecab', anonymous=True)
	r = rospy.Rate(10)
	msg = mecab_all()
	while True:
		rcvmsg = clientsock.recv(1024)
		sf = clientsock.makefile()
	#	for rcvmsg in sf.readlines():
		rcvmsg = sf.readline()
		while rcvmsg != None :
		#	print rcvmsg
			line = re.search('WORD=\"([^\"]+)\"',rcvmsg)
			ok1 = re.search('(<RECOGOUT>)',rcvmsg)
			ok2 = re.search('(</RECOGOUT>)',rcvmsg)
			if line != None:
				line2 += line.group(1)
				print line.group(0)
			elif ok1 != None:
				print 'ok'
			elif ok2 != None:
				ok = 1
			if ok == 1:
				print line2
				node = tagger.parseToNode(line2)
				msg.all = line2
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
						msg.part3[i] = m.group(3).rstrip(',')
					node = node.next
					i += 1
				msg.number = i-2;
				pub.publish(msg)
				ok = 0
				line2 = ""
			rcvmsg = sf.readline()
if __name__ == '__main__':
	try:
		talker()
	except rospy.ROSInterruptException: pass
	