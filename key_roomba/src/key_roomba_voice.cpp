#include<termios.h>
#include<signal.h>
#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/poll.h>
#include<boost/thread/thread.hpp>
#include<ros/ros.h>
#include<geometry_msgs/Twist.h>
#include <sstream>
#include <string>
#include <iostream>
#include <fstream>


#include<iconv.h>
#include <unistd.h>
using namespace std;

#define KEYCODE_I 0x69
#define KEYCODE_J 0x6a
#define KEYCODE_K 0x6b
#define KEYCODE_L 0x6c
#define KEYCODE_Q 0x71
#define KEYCODE_Z 0x7a
#define KEYCODE_W 0x77
#define KEYCODE_X 0x78
#define KEYCODE_E 0x65
#define KEYCODE_C 0x63
#define KEYCODE_U 0x75
#define KEYCODE_O 0x6F
#define KEYCODE_M 0x6d
#define KEYCODE_R 0x72
#define KEYCODE_V 0x76
#define KEYCODE_T 0x74
#define KEYCODE_B 0x62

#define KEYCODE_COMMA 0x2c
#define KEYCODE_PERIOD 0x2e

#define COMMAND_TIMEOUT_SEC 0.2

double max_speed = 0.100;
double max_turn = 30.0*M_PI/180.0;
bool always_command = false;

class TBK_Node
{
private:
  geometry_msgs::Twist cmdvel;
  ros::NodeHandle n_;
  ros::Publisher pub_;

 public:
  TBK_Node()
    {
      pub_ = n_.advertise<geometry_msgs::Twist>("/cmd_vel",1);
    }
  ~TBK_Node(){ }
  void keyboardLoop();
  void stopRobot()
  {
    cmdvel.linear.x = cmdvel.angular.z = 0.0;
    pub_.publish(cmdvel);
  }
};

TBK_Node* tbk;
int kfd = 0;
struct termios cooked, raw;
bool done;

int main(int argc,char** argv)
{
  ros::init(argc,argv,"tbk",ros::init_options::AnonymousName | ros::init_options::NoSigintHandler);
  TBK_Node tbk;

  boost::thread t = boost::thread(boost::bind(&TBK_Node::keyboardLoop, &tbk));

  ros::spin();

  t.interrupt();
  t.join();
  tbk.stopRobot();
  tcsetattr(kfd,TCSANOW,&cooked);

  return(0);
}

void TBK_Node::keyboardLoop()
{
  double max_tv = max_speed;
  double max_rv = max_turn;
  bool dirty = false;

  int speed = 0;
  int turn = 0;
  
  tcgetattr(kfd,&cooked);
  memcpy(&raw, &cooked, sizeof(struct termios));
  raw.c_lflag &=~ (ICANON | ECHO);
  raw.c_cc[VEOL] = 1;
  raw.c_cc[VEOF] = 2;
  tcsetattr(kfd,TCSANOW,&raw);
  
  puts("Reading from keyboard");
  puts("---------------------------------------------------------------------");
  puts("q/z : increase/decrease max angular nad linear speeds by 10%");
  puts("w/x : increase/decrease max linear speed by 10%");
  puts("e/c : increase/decrease max angular speed by 10%");
  puts("---------------------------------------------------------------------");
  puts("Moving around:");
  puts("   u=turn left and go forward    i=go forward    o=turn right and go forward");
  puts("   j=turn left                   k=stop          l=trun right");
  puts("   m=turn left and go backwards  ,=go backwards  .=turn right and go backwards");
  puts("                           anything else : stop");
  puts("---------------------------------------------------------------------");
 
  char ss[100];
  iconv_t ic;
  ic = iconv_open("UTF-8","Shift_JIS");
  int count = 0;
  char human[30][20];
  char hito[20];
  FILE *fp;
  int a = 0;
  int b = 0;
  int c = 0;
  int d = 0;
  int i = 0;
  char *ptr_in;
  char *ptr_out;
  size_t mybufsz = (size_t)19;
  struct pollfd ufd;
  ufd.fd = kfd;
  ufd.events = POLLIN;

  for(;;)
    {
      fp = fopen("/home/yhirai/jjj/rere.txt","r");
      boost::this_thread::interruption_point();
      while(fgets(ss,256,fp) != NULL){
	for(i = 0 ; ss[i] != '\0' ; i++){
	  if(ss[i] == '='&&a == 0){
	    b = 1;
	    a = 1;
	  }
	  else if(ss[i] == '='&&a == 1){
	    hito[d] = '\0';
	    b = 0;
	    a = 0;
	    c = 1;
	  }
	  else if(b == 1){
	    hito[d]=ss[i];
	    d++;
	  } 
	}
	if(c == 1){
	  strcpy(human[count],hito);
	  ptr_in = hito;
	  ptr_out = human[count];
	  //cout << hito << endl;
	  iconv(ic,&ptr_in,&mybufsz,&ptr_out,&mybufsz);
	  c = 0;
	  d = 0;
	  count++;
	}
      }
      int num;
      /*    if((num = poll(&ufd,1,250)) < 0)
	{
	  perror("poll():");
	  return;
	}
      else if(num > 0)
	{
	  if(read(kfd,&c,1) < 0)
	    {
	      perror("read():");
	      return;
	    }
	}
      else
	continue;
      */
      if(strcmp("Go",human[count-1]) == 0){//KEYCODE_I){
  	  speed = 1;
	  turn = 0;
	  dirty = true;
	  system("sh ~/openjtalk/talk.sh \"まえにいどうします\"");

      }
      else if(strcmp("Left",human[count-1]) == 0){  //KEYCODE_J:
	speed = 0;
	turn = 1;
	dirty = true;
	system("sh ~/openjtalk/talk.sh \"ひだりにいどうします\"");
      }
      else if(strcmp("Right",human[count-1]) == 0){// KEYCODE_L:
	speed = 0;
	turn = -1;
	dirty = true;
	system("sh ~/openjtalk/talk.sh \"みぎにいどうします\"");
      }    
      else if(strcmp("Back",human[count-1]) == 0){ //KEYCODE_COMMA:
	speed = -1;
	turn = 0;
	dirty = true;
	system("sh ~/openjtalk/talk.sh \"うしろにいどうします\"");
      }
      else if(strcmp("Stop",human[count-1]) == 0){
	speed = 0;
	turn = 0;
	system("sh ~/openjtalk/talk.sh \"とまります\"");
	dirty = true;
      }
      else{
	speed = 0;
	turn = 0;
	dirty = true;
      }
      if(dirty == true)
	{
	  cmdvel.linear.x = speed * max_tv;
	  cmdvel.angular.z = turn * max_rv;
	  pub_.publish(cmdvel);
	}
      fclose(fp);
      count = 0;
      usleep(200000);
    }
}