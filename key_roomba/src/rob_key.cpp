#include <ros/ros.h>
#include "geometry_msgs/Twist.h"//---++Step5:(Always) For Keyboard
/* This is real step to make Robot moveable, make sure you have connected Robot and PC through USB port.
Open new terminal and execute following command to run <b>core</b> of the ros :

=$ roscore=

Open another terminal and execute following command run node RosAria:

=$ rosrun ROSARIA RosAria=

The above command will make Robot ready to accept velocity command. NOTE :: RosAria by default  uses port ID /dev/ttyUSB0 and if this port ID is not available then you can configure it on another available port ID like /dev/ttyUSB2 for example the command will be:</p>

=$ rosrun ROSARIA RosAria =

Open one more terminal and execute following command to run node rob_key:

=$ rosrun ROSARIA rob_key=

 Now if every thing done well then you can move Poineer P3-DX using keys ( Space key for STOP, Up for forward, Down for backward, LEFT for left turn and RIGHT key for right turn). 
//Thanks a lot for using this tutorial.

*/
#include <signal.h>
#include <termios.h>
#define KEYCODE_R 0x43
#define KEYCODE_L 0x44
#define KEYCODE_U 0x41
#define KEYCODE_D 0x42
#define KEYCODE_Q 0x71
#define KEY_SPACE 0x20

//=Path_from_Home/ROSARIA$ rosrun ROSARIA <b>RosAria</b>=

using geometry_msgs::Twist;
using namespace std;

ros::Publisher chatter_pub;
ros::Time t1;//Point to be NOTED
Twist vel;
int kfd = 0;
struct termios cooked, raw;
void quit(int sig)
   {
     tcsetattr(kfd, TCSANOW, &cooked);//Point to be NOTED
     ros::shutdown();
     exit(0);
   }
//=Path_from_Home/ROSARIA$ rosrun ROSARIA <b>RosAria</b>=

int main(int argc, char** argv)
{
  ros::init(argc, argv, "Rob_key");
  ros::NodeHandle n;
   chatter_pub = n.advertise<Twist>("/cmd_vel", 1);  
   signal(SIGINT,quit);
   char c;
   bool dirty=false;//Point to be NOTED
                t1=ros::Time::now();//Point to be NOTED

   tcgetattr(kfd, &cooked);//Point to be NOTED
   memcpy(&raw, &cooked, sizeof(struct termios));
   raw.c_lflag &=~ (ICANON | ECHO);
   raw.c_cc[VEOL] = 1;
   raw.c_cc[VEOF] = 2;
   tcsetattr(kfd, TCSANOW, &raw);

 while (ros::ok())
  {
//=Path_from_Home/ROSARIA$ rosrun ROSARIA <b>RosAria</b>=

    if(read(kfd, &c, 1) < 0)
       {
              perror("read():");
         exit(-1);
       }
         switch(c)
       {
         case KEYCODE_L:
           ROS_DEBUG("LEFT");//Point to be NOTED
      puts("TURN LEFT");
           vel.angular.z  = 0.5;//Point to be NOTED
           dirty = true;
           break;

         case KEYCODE_R:
            ROS_DEBUG("RIGHT");
      puts("TURN RIGHT");
          vel.angular.z  = -0.5;
           dirty = true;
           break;

         case KEYCODE_U:
            ROS_DEBUG("UP");
      puts("FORWARD");
           vel.linear.x = 0.1;
                vel.angular.z  = 0;
            dirty = true;
           break;//Point to be NOTED
//Point to be NOTED
         case KEYCODE_D:
            ROS_DEBUG("DOWN");
                puts("BACKWARD");
           vel.linear.x = -0.1;
                vel.angular.z  = 0;
                ROS_DEBUG("LEFT");
           dirty = true;
           break;

         case KEY_SPACE:
           ROS_DEBUG("STOP");
                puts("SPACE");
           vel.linear.x = 0.0;
      vel.angular.z = 0.0;
           dirty = true;
       }

          vel.linear.y=0;
      vel.linear.z=0;
               vel.angular.x = 0;
      vel.angular.y = 0;
           if(dirty==true)
           {

         chatter_pub.publish(vel);
              dirty=false;

       }
   
    ros::spinOnce();
   
  }//while
return(0);
}
