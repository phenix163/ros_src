#include "picojson-master/picojson.h"
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "zmq_ros/amivoice_noun_verb2.h"
#include "rviz_plugin_tutorials/OverlayText.h"

#include <fstream>
#include <iostream>
#include <cassert>
#include <memory>
#include <string>
#include <algorithm>
using namespace std;
using namespace picojson;

int robot_number(string noun);

int main(int argc, char **argv) {
  int c;
  map<std::string,std::string> json_map,json_map2;
  value root,root2,root3;
  std::ifstream ifs("/home/yhirai/catkin_ws/src/noun/src/rank.txt");
  string rank[5];
  int i = 0;
  std_msgs::String command_msg;
  std_msgs::String chat_msg;
  ros::init(argc, argv, "noun");
  ros::NodeHandle n;
  ros::Publisher command_pub = n.advertise<std_msgs::String>("chatter", 1000);
  ros::Publisher overlay_text 
    = n.advertise<rviz_plugin_tutorials::OverlayText>("command_text", 10);
  ros::Publisher baxter_pub = n.advertise<std_msgs::String>("baxter_chatter", 1000);
  ros::Publisher chat_pub = n.advertise<std_msgs::String>("julius", 100);
  string aa;
  string bb = "なし";
  int robot = 0;
  int sub_robot = 0;
  int ok = 0;
  int chat_ok = 0;
  string com;
  string admin = "admin";
  string aa2;
  while(getline(ifs,rank[i]))
    i++;
  while(1){
    zmq_ros::amivoice_noun_verb2ConstPtr noun_msg = ros::topic::waitForMessage<zmq_ros::amivoice_noun_verb2>("noun");
    com = "";
    aa2 = "";
    aa2 = noun_msg->verb;
    root2 = value();
    c = 0;
    ifstream stream2("/home/yhirai/catkin_ws/src/verb/src/verb_admin.json");
    stream2 >> root2;
    //   cout << com << endl;
    const picojson::value::object& obj2 = root2.get<picojson::object>();
    for (picojson::value::object::const_iterator i2 = obj2.begin(); i2 != obj2.end(); ++i2) {
      json_map2.insert( make_pair(i2->first,i2->second.to_str() ) );
    } 
    c = json_map2.count(aa2);
    cout << aa2 << endl;
    if(c > 0){
      object image2 = root2.get<object>()[aa2].get<object>();
      // cout << "command=" << image2["command"].get<string>() << endl;
      com = image2["command"].get<string>();
      admin = image2["admin"].get<string>();
    }
    //cout << com << endl;
    if(com == "move"){
      chat_ok = 1;
      /*if(admin != noun_msg->name){
	cout << "あなたには実行権限がありません" << endl;
	continue;
	}*/
      for(i = 1; i < noun_msg->value ; i++){
	aa = "";
	aa = noun_msg->noun[i];
	
	root = value();
	c = 0;
 
	ifstream stream("/home/yhirai/catkin_ws/src/noun/src/noun.json");
	stream >> root;
	
	const picojson::value::object& obj = root.get<picojson::object>();
	for (picojson::value::object::const_iterator i = obj.begin(); i != obj.end(); ++i) {
	  json_map.insert( make_pair(i->first,i->second.to_str() ) );
	}
	c = json_map.count(aa);
       // cout << c << endl;
	if(c > 0){
	  object image = root.get<object>()[aa].get<object>();
	  //	cout << "command=" << image["command"].get<string>() << endl;
	  command_msg.data = image["command"].get<string>();
	}
	robot = 1;
	sub_robot = robot_number(aa);
	if(sub_robot > 1){
	  cout << "命令を送るロボットが間違っています" << endl;
	  continue;
	}
      }
      //  cout << com << endl;
      if(com == "move"){
	//	robot = 1;
	while(1){
	  if(robot == 0){
	    cout << "Which robot?" << endl;
	    //    system("aplay /home/yhirai/dictation-kit-v4.3.1-linux/know_robot2.wav");
	    zmq_ros::amivoice_noun_verb2ConstPtr noun_msg = ros::topic::waitForMessage<zmq_ros::amivoice_noun_verb2>("noun");
	    for(i = 1; i < noun_msg->value ; i++ ){
	      sub_robot = robot_number(noun_msg->noun[i]);
	      if(sub_robot == 1)
		robot = sub_robot;
	    }
	    if(robot == 0){
	      cout << "Sorry... I don't know this robot..." << endl;
	      //system("aplay /home/yhirai/dictation-kit-v4.3.1-linux/know_robot.wav");
	      continue;  //登録されていないロボットの場合ループ
	      }
	    else if(robot == 3){
	      break;
	    }
	  }  //ここまで場所が入ってる場合
	  if(command_msg.data == ""){
	    cout << "Where?" << endl;
	    //system("aplay /home/yhirai/dictation-kit-v4.3.1-linux/know_place2.wav");
	    zmq_ros::amivoice_noun_verb2ConstPtr noun_msg = ros::topic::waitForMessage<zmq_ros::amivoice_noun_verb2>("noun");
	    for(i = 0;i < noun_msg->value ; i++){
	      c = json_map.count(noun_msg->noun[i]);
	      if(c > 0){
		object image3 = root.get<object>()[noun_msg->noun[i]].get<object>();
		//	cout << "command=" << image["command"].get<string>() << endl;
		command_msg.data = image3["command"].get<string>();
	      }
	    }
	    if(command_msg.data == ""){
	      cout << "Sorry... Not register..." << endl;
	      //system("aplay /home/yhirai/dictation-kit-v4.3.1-linux/know_place.wav");
	      continue; //登録されていない場所の場合ループ
	    }	  
	  } //場所が入っていない場合
	  if(command_msg.data != "" && robot == 1){
	    //system("aplay /home/yhirai/dictation-kit-v4.3.1-linux/go.wav");
	    cout << command_msg.data << endl;

	    //rviz上に表示
	    rviz_plugin_tutorials::OverlayText comtext;
	    std::string src;
	    src = "Go to " + command_msg.data + "'s position.";
	    comtext.text = src; 
	    overlay_text.publish( comtext );

	    command_pub.publish(command_msg);
	    robot = 0;
	    com = "";
	    break;
	  }//全部入ったとき
	}//全部はいるまでループ
      }  
      else if(com == "visual" && robot == 1 && command_msg.data != ""){
	cout << command_msg.data << endl;
      }
    }
    else if(com == "return"){
      chat_ok = 1;
      i = 0;
      ok = 0;
      while(i != 3){
	cout << rank[i] << endl;
	command_msg.data = rank[i];

	//rviz上に表示
	rviz_plugin_tutorials::OverlayText comtext;
	std::string src;
	src = "Go to " + command_msg.data + "'s position";
	comtext.text = src; 
	overlay_text.publish( comtext );

	command_pub.publish(command_msg);
	while(1){
	  zmq_ros::amivoice_noun_verb2ConstPtr noun_msg = ros::topic::waitForMessage<zmq_ros::amivoice_noun_verb2>("noun");
	  if(noun_msg->noun[1] == "はい"){
	    ok = 1;
	    break;
	  }
	  else if(noun_msg->noun[1] == "いいえ"){
	    break;
	  }
	}
	if(ok == 1){
	  cout << "Nice support!!" << endl;
	  break;
	}
	i++;
      }
    }
    else if(com == "handshake"){
      if(admin != noun_msg->name){
        cout << "あなたには実行権限がありません" << endl;
        continue;
      }
      chat_ok = 1;
      command_msg.data = com;
      baxter_pub.publish(command_msg);
      cout << "handshake" << endl;
      //system("aplay /home/yhirai/catkin_ws/baxter_hand.wav");
    }
    else if(com == "up"){
      if(admin != noun_msg->name){
        cout << "あなたには実行権限がありません" << endl;
        continue;
      }
      chat_ok = 1;
      string hh;
      for(i = 1; i < noun_msg->value ; i++){
        aa = "";
	hh = "";
        aa = noun_msg->noun[i];
        root = value();
	robot = robot_number(aa);
        c = 0;
        ifstream stream("/home/yhirai/catkin_ws/src/noun/src/noun.json");
        stream >> root;

        const picojson::value::object& obj = root.get<picojson::object>();
        for (picojson::value::object::const_iterator i = obj.begin(); i != obj.end(); ++i) {
          json_map.insert( make_pair(i->first,i->second.to_str() ) );
	}
        c = json_map.count(aa);

        if(c > 0){
          object image = root.get<object>()[aa].get<object>();
          //    cout << "command=" << image["command"].get<string>() << endl;                                                                                 
          hh = image["command"].get<string>();
        }
	if(robot == 3 && hh == "righthand"){
	  command_msg.data = "armup_r";
	  cout << command_msg.data << endl;
	  baxter_pub.publish(command_msg);
	}
	else if(robot == 3 && hh == "lefthand"){
	  command_msg.data = "armup_l";
	  cout << command_msg.data << endl;
	  baxter_pub.publish(command_msg);
	}
	else if(robot == 3 && hh == "doublehand"){
	  command_msg.data = "armup_d";
	  cout << command_msg.data << endl;
	  baxter_pub.publish(command_msg);
	}
      }
    }
    else if(com == "down"){
      if(admin != noun_msg->name){
        cout << "あなたには実行権限がありません" << endl;
        continue;
      }
      chat_ok = 1;
      for(i = 1; i < noun_msg->value ; i++){
        aa = "";
        aa = noun_msg->noun[i];
	robot = robot_number(aa);
	if(robot == 3){
	  cout << "down" << endl;
	  command_msg.data = "armdown";
	  baxter_pub.publish(command_msg);
	}
      }
    }
    else if(com == "bye"){
      /*  if(admin != noun_msg->name){
        cout << "あなたには実行権限がありません" << endl;
        continue;
	}*/
      chat_ok = 1;
      cout << "okdesu" << endl;
      for(i = 1; i < noun_msg->value ; i++){
        aa = "";
        aa = noun_msg->noun[i];

        robot = robot_number(aa);
	if(robot == 3){
	  cout << "bye" << endl;
	  command_msg.data = "bye";
	  baxter_pub.publish(command_msg);
	}
      }
    }
    if(chat_ok == 0){
      chat_msg.data = noun_msg->all;
      chat_pub.publish(chat_msg);
    }
    robot = 0;
    chat_ok = 0;
    command_msg.data = "";
    com = "";
    aa2 = "";
    c = 0;
  }
}

int robot_number(string noun){
  ifstream stream("/home/yhirai/catkin_ws/src/noun/src/robot.json");
  value root;
  int c;
  map<std::string,std::string> json_map;
  stream >> root;
  const picojson::value::object& obj = root.get<picojson::object>();
  for (picojson::value::object::const_iterator i = obj.begin(); i != obj.\
	 end(); ++i) {
    json_map.insert( make_pair(i->first,i->second.to_str() ) );
  }
  c = json_map.count(noun);

  if(c > 0){
    object image = root.get<object>()[noun].get<object>();
    double hh = image["robot_num"].get<double>();
    return hh;
  }
  else{
    return 0;
  }
}
