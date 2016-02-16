#include "picojson-master/picojson.h"
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <fstream>
#include <iostream>
#include <cassert>
#include <memory>
#include <string>
#include <algorithm>
using namespace std;
using namespace picojson;

int main(int argc, char **argv) {
  int c;
  map<std::string,std::string> json_map;
  value root;
  ros::init(argc, argv, "verb");
  ros::NodeHandle n;
  ros::Publisher command_pub = n.advertise<std_msgs::String>("command_verb", 1000);
  string aa;
  string bb = "なし";
  std_msgs::String command_msg;
  while(1){
      std_msgs::StringConstPtr msg = ros::topic::waitForMessage<std_msgs::String>("verb");
      aa = msg->data;
      //aa.replace(aa.begin(),aa.end(),'\n',' ');
      //  aa.ignore();
      cout << aa << endl;
      root = value();
      c = 0;
  
      ifstream stream("/home/yhirai/catkin_ws/src/verb/src/verb.json");
      //if ( !stream.is_open() ) return 1;
      stream >> root;
      //      assert( get_last_error().empty() );
    
      const picojson::value::object& obj = root.get<picojson::object>();
      for (picojson::value::object::const_iterator i = obj.begin(); i != obj.end(); ++i) {
	json_map.insert( make_pair(i->first,i->second.to_str() ) );
      }
      c = json_map.count(aa);
      cout << c << endl;
      if(c > 0){
	object image = root.get<object>()[aa].get<object>();
	cout << "command=" << image["command"].get<string>() << endl;
	command_msg.data = image["command"].get<string>();
	command_pub.publish(command_msg);
      }
  }
}
