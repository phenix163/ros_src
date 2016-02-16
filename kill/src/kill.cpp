#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>
#include <string>
#include <iostream>
#include <fstream>

#include<iconv.h>
#include <unistd.h>
using namespace std;

  
int main(int argc, char **argv)
{
   char kk[20];
   ros::init(argc, argv, "listener");
    ros::NodeHandle n;
   while(1){
     std_msgs::StringConstPtr testdata = ros::topic::waitForMessage<std_msgs::String>("chatter");

     strcpy(kk,testdata->data.c_str());
     if(strcmp(kk,"END") == 0){
       cout << "aaa" << endl;
       // system("kill `ps -ef | grep julius| grep -v grep | awk '{print $2}'`");
       break;
     }

     usleep(200000);
   }
   return 0;
}