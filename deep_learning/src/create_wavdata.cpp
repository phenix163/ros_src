#include "ros/ros.h"
#include "std_msgs/String.h"
#include "speech_msgs/AudioData.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <unistd.h>
#include <string>
#include <assert.h>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/soundcard.h>
#include <fcntl.h>

#include <map>

using namespace std;

void audioCallback(const speech_msgs::AudioData::ConstPtr& msg)
{
  cout << msg->audio_buf[0] << endl;
  if(msg->ok == 1){
    cout << "True" << endl;
  }
  else{
    cout << "False" << endl;
  }
}

int main(int argc, char **argv){
  ros::init(argc,argv,"output_audio");
  ros::NodeHandle n;
 
  ros::Subscriber sub = n.subscribe("audiodata_info", 1000, audioCallback);

  ros::spin();

}
