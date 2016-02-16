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
import requests

import cPickle

import numpy as np
import six
import cv2
import os
import six.moves.cPickle as pickle

import chainer
from chainer import computational_graph as c
from chainer import cuda
import chainer.functions as F
from chainer import optimizers
from chainer import computational_graph as c
from chainer import Variable
from chainer.functions import *


time.sleep(0.5)

url = "http://133.19.23.117:8000/asr_julius"

s = requests.Session()

def callback(data):
    
    global s
    files = {
        'myFile': open('/home/yhirai/openSMILE-2.1.0_other/input/input.wav', 'rb')
    }
    r = s.post(url, files=files)
    print r.text
          
        
def listener():

    # in ROS, nodes are unique named. If two nodes with the same
    # node are launched, the previous one is kicked off. The 
    # name for our 'listener' node so that multiple listeners can
    # run simultaenously.
    rospy.init_node('speech_recog', anonymous=True)

    rospy.Subscriber("convert_ok", AudioData, callback)

    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()
        
if __name__ == '__main__':
    listener()
