# -*- coding: utf-8 -*-

import httplib, urllib, base64
from PIL import Image
import json
import cv2
import threading
import time
import datetime
import rospy
from std_msgs.msg import String

emotion = ['sadness','neutral','contempt','disgust','anger','surprise','fear','happiness']
body = ""

emotion_value = [[0.1 for i in range(8)] for j in range(6)]

width = [0] * 6
height = [0] * 6
left = [0] * 6
top = [0] * 6
roop = 0

length = 0

class Test(threading.Thread):
    def getVarsNames( _vars, symboltable ) :
        """
        This is wrapper of getVarName() for a list references.
        """
        return [ getVarName( var, symboltable ) for var in _vars ]

    def __init__(self):
        threading.Thread.__init__(self)

    def run(self):
        pub = rospy.Publisher('chatter', String, queue_size=10)

        print "==start sub thread=="
        headers = {
            # Request headers
            'Content-Type': 'application/octet-stream',
            'Ocp-Apim-Subscription-Key' : 'ba6740d32ff14535ad744a1a1475e80f',

        }


        params = urllib.urlencode({
            # Request parameters

            # 'faceRectangles': jsons[0]["faceRectangle"],
        })
        while True:
            time.sleep(8)
            global roop
            conn = httplib.HTTPSConnection('api.projectoxford.ai')
            conn.request("POST", "/emotion/v1.0/recognize?%s" % params, body, headers)
            global emotion_value
            response = conn.getresponse()
            jsons = json.load(response)
            global length
            length = len(jsons)
            #           print jsons
            if length == 0:
                roop = 0
            else:
                for i in range(length):
                    data = jsons[i]["scores"]
                    emotion_value[i][0] = float(data["sadness"])
                    emotion_value[i][1] = float(data["neutral"])
                    emotion_value[i][2] = float(data["contempt"])
                    emotion_value[i][3] = float(data["disgust"])
                    emotion_value[i][4] = float(data["anger"])
                    emotion_value[i][5] = float(data["surprise"])
                    emotion_value[i][6] = float(data["fear"])
                    emotion_value[i][7] = float(data["happiness"])
                    data2 = jsons[i]["faceRectangle"]
                    global width
                    width[i] = int(data2["width"])
                    global height
                    height[i] = int(data2["height"])
                    global left
                    left[i] = int(data2["left"])
                    global top
                    top[i] = int(data2["top"])

                    roop = 1

                    print "number:" + str(i+1)
                    print(data)
                   
                    print(data2)

                    print
                    data_str = str(emotion_value[i].index(max(emotion_value[i])))
                    pub.publish(data_str)




if __name__ == '__main__':
    rospy.init_node('b3_kadai', anonymous=True)
    th = Test()
    th.setDaemon(True)
    th.start()

    capture = cv2.VideoCapture(0)
    if capture.isOpened() is False:
        #raise("IO Error")
	print("ba-ka")
    cv2.namedWindow("Capture",cv2.WINDOW_AUTOSIZE)
    
    while True:
        
        ret, image = capture.read()
        if ret == False:
            continue

        if roop == 1:
            for i in range(length):
                x = int(left[i]+width[i]/2)
                y = int(top[i]+height[i]/2)
                x2 = x
                y2 = top[i]
                kkk = int((width[i]+height[i])/4)
                cv2.circle(image,(x,y),kkk,(0,0,255),5)
                output = str(i+1)+ ':' + emotion[emotion_value[i].index(max(emotion_value[i]))]
                cv2.putText(image,output,(x2,y2),cv2.FONT_HERSHEY_SIMPLEX,1,(0,255,0),2)
            
        cv2.imshow("Capture",image)
        body = cv2.imencode('.jpg',image)[1].tostring()
        cv2.imwrite("image.png", image)
        if cv2.waitKey(33) >= 0:
            break

  
    cv2.destroyAllWindow()





