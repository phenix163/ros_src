#!/usr/bin/env python
# -*- coding: utf-8 -*-
 
u"""Docomoの雑談対話APIを使ってチャットできるスクリプト
"""
import rospy
from std_msgs.msg import String

import sys
import urllib2
import json
import wave
import os
 
APP_URL = 'https://api.apigw.smt.docomo.ne.jp/voiceText/v1/textToSpeech'
class DocomoChat(object):
    u"""Docomoの雑談対話APIでチャット"""
 
    def __init__(self, api_key):
        super(DocomoChat, self).__init__()
        self.api_url = APP_URL + '?APIKEY=%s'%(api_key)
        self.context, self.mode = None, None
 
    def __send_message(self, input_message='', steat = '', custom_dict=None):
        
        text = "text="+input_message+steat
        print text
        request = urllib2.Request(self.api_url, text)
        request.add_header('Content-Type', 'application/x-www-form-urlencoded')
        try:
            response = urllib2.urlopen(request)
        except Exception as e:
            print e
            sys.exit()
        return response
 
    
    def send_and_get(self, input_message,steat):
        response = self.__send_message(input_message,steat)
        #received_message = self.__process_response(response)
        return response
 
 
def main():
    api_key = '49337978393057785052794f434d416a4f7442326f3473436d69774c54486470534e545958347a464c7334'
    chat = DocomoChat(api_key)
    rospy.init_node('listener', anonymous=True)
    a = 2
    message = ""
    steat = ""

    while True:
        res = rospy.wait_for_message("chatter",String)
        a = int(res.data)
        #global a 
        if a == 1:
            message = u"こんにちは"
            steat = u"&speaker=takeru&emotion=anger&emotion_level=2&pitch=70&speed=120"

        resp = chat.send_and_get(message, steat)

        test = wave.Wave_write("test.wav")
        test.setnchannels(1)
        test.setsampwidth(2)
        test.setframerate(44100)
        test.writeframes(resp.read())
        test.close()
        wavfile = "test.wav"
        os.system("aplay "+ wavfile)
        message = ""
        steat = ""


if __name__ == '__main__':
    main()
