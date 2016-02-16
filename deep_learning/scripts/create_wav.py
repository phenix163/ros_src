#!/usr/bin/env python
# -*- coding: utf-8 -*-
 
import sys

import rospy
from speech_msgs.msg import AudioData
import subprocess
import codecs
import wave
import pyaudio
from numpy import *
import os, glob, random
import struct
import time

first = 0

write_wave = ""
wav_file = ""
audio = []
pub = rospy.Publisher('wav_ok',AudioData,queue_size=100)
start = time.time()

def callback(data):
    
    global first
    global write_wave
    global wav_file
    global audio
    global pub
    
    if data.ok == True:
        audio.extend(data.audio_buf)
    else:
	start = time.time()
        byte_count = (len(audio)) * 4  # 32-bit floats
        wav_file = ""
        # write the header
        wav_file += struct.pack('<ccccIccccccccIHHIIHH',
                                'R', 'I', 'F', 'F',
                                byte_count + 0x2c - 8,  # header size
                                'W', 'A', 'V', 'E', 'f', 'm', 't', ' ',
                                0x10,  # size of 'fmt ' header
                                3,  # format 3 = floating-point PCM
                                1,  # channels
                                16000,  # samples / second
                                16000 * 4,  # bytes / second
                                4,  # block alignment
                                32)  # bits / sample
        wav_file += struct.pack('<ccccI',
                                'd', 'a', 't', 'a', byte_count)
        for sample in audio:
            wav_file += struct.pack("<f", sample)
        
        fi = open("/home/yhirai/openSMILE-2.1.0_other/input/bb.wav",'wb')
        for value in wav_file:
            fi.write(value)
        first = 0
        wav_file = ""
        audio = []

        msg = AudioData()
        msg.ok = True
        msg.tracking_ID = data.tracking_ID
        pub.publish(msg)
	elapsed_time = time.time() - start
	print elapsed_time        

def listener():

    # in ROS, nodes are unique named. If two nodes with the same
    # node are launched, the previous one is kicked off. The 
    # anonymous=True flag means that rospy will choose a unique
    # name for our 'listener' node so that multiple listeners can
    # run simultaenously.
    rospy.init_node('listener', anonymous=True)

    rospy.Subscriber("audiodata_info", AudioData, callback)

    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()
        
if __name__ == '__main__':
    listener()
