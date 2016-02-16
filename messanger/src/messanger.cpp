#include "ros/ros.h"
#include "std_msgs/String.h"

#include <sstream>
#include <string>
#include <iostream>
#include <fstream>

#include <boost/asio.hpp>
#include <boost/regex.hpp>
#include <unistd.h>

using namespace std;
using namespace boost::asio;
int main(int argc, char **argv)
{

  
  ros::init(argc, argv, "talker");

  ros::NodeHandle n;

  ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1000);

  int count = 0;
  
  size_t mybufsz = (size_t)19;
  const std::string ip = "133.19.23.53";
  const int port = 39390;
  
  ip::address addr = ip::address::from_string(ip);
  ip::tcp::endpoint ep(addr, port);
  ip::tcp::iostream s(ep);

  s << "STATUS\n" << std::flush;
  cout << "ok" << endl;

  boost::regex r("(=)(.*)(=)");
  boost::smatch m;
  string line,word;

  while (getline(s,line))
  {
    cout << line << endl;
  
    std_msgs::String msg;
 
    if (boost::regex_search(line, m, r)){
      word = "";
      word += m.str(2);
      msg.data = word;
      chatter_pub.publish(msg);
    }

    line = "";
 
  }


  return 0;
}