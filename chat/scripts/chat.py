#!/usr/bin/env python
# -*- coding: utf-8 -*-
 
u"""Docomoの雑談対話APIを使ってチャットできるスクリプト
"""
 
import sys
import urllib2
import json
import subprocess
import codecs 

APP_URL = 'https://api.apigw.smt.docomo.ne.jp/dialogue/v1/dialogue'
 
class DocomoChat(object):
    u"""Docomoの雑談対話APIでチャット"""
    def __init__(self, api_key):
        super(DocomoChat, self).__init__()
        self.api_url = APP_URL + '?APIKEY=%s'%(api_key)
        self.context, self.mode = None, None
    
    def __send_message(self, input_message='', custom_dict = None):
        req_data = {'utt': input_message}
        if self.context:
            req_data['context'] = self.context
        if self.mode:
            req_data['mode'] = self.mode
        if custom_dict:
            req_data.update(custom_dict)
        request = urllib2.Request(self.api_url, json.dumps(req_data))
        request.add_header('Content-Type', 'application/json')
        try:
            response = urllib2.urlopen(request)
        except Exception as e:
            print e
            sys.exit()
        return response
 
    def __process_response(self, response):
        resp_json = json.load(response)
        self.context = resp_json['context'].encode('utf-8')
        self.mode    = resp_json['mode'].encode('utf-8')
        return resp_json['utt'].encode('utf-8')
 
    def send_and_get(self, input_message):
        response = self.__send_message(input_message)
        received_message = self.__process_response(response)
        return received_message
 
    def set_name(self, name, yomi,sex,bloodtype,birthdateM,birthdateD,age,place):
        response = self.__send_message(custom_dict={'nickname': name, 'nickname_y': yomi,'sex': sex, 'bloodtype': bloodtype, 'birthdateM': birthdateM, 'birthdateD': birthdateD,'age': age, 'place': place})
        received_message = self.__process_response(response)
        return received_message
    
 
def main():
    api_key = '574858667152344f4e43314a52686a6848624c364935537936682f4133313758796b4e6b544d4b54784f44'
    chat = DocomoChat(api_key)
    resp = chat.set_name('ピーマン', 'ピーマン','男','A',9,21,21,'京都')
    print '相手　 : %s'%(resp)
    message = ''
    before = u''
    line2 = u''
    end = u"さようなら"
    end.encode('utf-8')
    while line2 != end:
 
  	    message = raw_input('あなた : ')
            #message = unicode(line2,'utf-8')
            #message.encode('shift-jis')
            resp = chat.send_and_get(message)
            print '相手　 : %s'%(resp)
            #sh = "sh ~/openjtalk/talk.sh \"%s\""%(resp)
            #subprocess.call(sh,shell = True)
            before = line2
if __name__ == '__main__':
    main()
