/*
2015.2.27--------------------------------
誰と誰が何を話したかを記録するモジュール

*/

#include <ros/ros.h>
#include <message_filters/subscriber.h>
#include <message_filters/synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>

#include <fstream>
#include "zmq.hpp"
#include "msgpack.hpp"

#include <humans_msgs/Humans.h>

#include <speech_msgs/Speech.h>
#include <zmq_ros/mecab_amivoice2.h>
#include <speech_msgs/SpeechSrv.h>

using namespace std;

#define MASTER 1


class Place
{
public:
  string name;
  double x;
  double y;
};

class Situation
{
public:
  int okao_id;
  Place place;
  ros::Time time;
  vector<int> people;
};

class WordProp
{
public:
  int freq;
  int num;
  vector<string> part;
  vector<Situation> situations;
};

//map< int, map< string, WordProp > > user_words;

map< string, WordProp > words_wp;
map< int, humans_msgs::Human > human_last;

class SpeechRecog
{
private:
  ros::NodeHandle nh;
  ros::ServiceServer word_srv;
  //ros::Publisher recog_pub_;
  //boost::mutex mutex_;
  ros::Subscriber speech_sub;
  ros::Subscriber okao_sub;

public:
  SpeechRecog() 
  {

    speech_sub 
      = nh.subscribe("mecab_res", 1, 
		    &SpeechRecog::speechCallback, this);

    okao_sub
      = nh.subscribe("/humans/RecogInfo", 1,
		    &SpeechRecog::okaoCallback, this);


    word_srv 
      = nh.advertiseService("word_search",
			    &SpeechRecog::wordSearchCallback, this);
    //recog_pub_ = 
    //  nh.advertise<humans_msgs::Humans>("/humans/RecogInfo", 1);

    //ファイルからメモリにこれまでの単語情報を書き込む機能

  }
  ~SpeechRecog()
  {
    //メモリからファイルにこれまでの単語情報を書き込む機能
    cout << "file write" << endl;
    stringstream ss;
    map< string, WordProp >::iterator it_wp = words_wp.begin();
    while( it_wp != words_wp.end() )
      {

	ss << "word: " << it_wp->first << ", freq: " << it_wp->second.freq 
	   << ", num: "<< it_wp->second.num; 

	
	//stringstream sit;
	for(int w_id = 0; w_id < it_wp->second.situations.size() ; ++w_id)
	  {
	    ss << ", situation[ " << w_id <<" ]: " <<  it_wp->second.situations[ w_id ].okao_id; 
	  }
	
	ss << endl;

	++it_wp; 
      }

    std::ofstream ofs("/home/yhirai/catkin_ws/src/okao_client/src/people/words.txt");//,std::ios::out | std::ios::app );
    if(ofs.fail())
      {  // if(!fout)でもよい。
        cout << "file not open" << endl;
        return;
      }
    ofs << ss.str() <<endl;
    cout << "write data:" << ss.str() <<endl;


    //
    words_wp.clear();
  }
 
  void okaoCallback(
		const humans_msgs::HumansConstPtr& okao
		)
  {
    //cout << "okao callback people num: "<< okao->human.size() << endl;
    //bool master_is_looking = false;
    if( okao->human.size() )
      {
	cout<< "add okao size: " << okao->human.size()<<endl; 
	//過去の人物情報を一旦クリア
	human_last.clear();
	for(int h_i = 0 ; h_i < okao->human.size() ; ++h_i)
	  {
	    human_last[ h_i ] = okao->human[ h_i ];
	    
	    if( okao->human[ h_i ].max_okao_id == MASTER )
	      {
		cout << "RECOG OK! Master: " << okao->human[ h_i ].max_okao_id << endl; 
	      }
	    
	  }
      }

  }

  void speechCallback(
		      const speech_msgs::SpeechConstPtr& speech
		)
  {
    cout << "speech callback" << endl;
    
    //1.開始
    //2.okao内のトラッキングIDと結びついた名前（OKAO_ID）を調べる
    //3.もし、主人の名前があったら、4へ。なければ1へ（ループ）
    //4.speech内のトラッキングIDを取得
    //5.トラッキングIDとOKAO_IDを照合する
    //6.OKAO_IDに発言を記録する
    //8.主人以外の人物の記録が終わったら、1へ。終わってなければ6へ。
  
    //人物を検出しているかどうか
    cout << "human size: "<< human_last.size() <<endl;

    if(human_last.size())
      {
	bool master_is_looking = false;
	map<long long, int> tracking_to_okao;
	//発見した人物を記録し、主人を探す
	for(int i = 0; i<human_last.size(); ++i)
	  {
	    tracking_to_okao[ human_last[i].body.tracking_id ] 
	      = human_last[i].max_okao_id;
	    cout << "now looking okao_id: " << human_last[i].max_okao_id << endl; 
	    if( human_last[i].max_okao_id == MASTER )
	      master_is_looking = true;
	  }

	//主人がいるなら記録開始
	//	if( master_is_looking )
	//{
	    speech_msgs::Speech sp_msg;


	    cout<< "now looking master!" <<endl;
	    int speech_okao_id;
	    //string to long long
	    long long speech_tracking_id = boost::lexical_cast<
	      long long>(speech->TrackingID); 
      	    cout << "now speech tracking_id: " << speech_tracking_id << endl;

	    map< long long, int >::iterator tracking_id_find
	      = tracking_to_okao.find( speech_tracking_id );
	    speech_okao_id = tracking_id_find->second;
	    sp_msg.okao_id = speech_okao_id;
	    browser(speech,speech_okao_id);
     	    cout << "now speech okao_id: " << speech_okao_id << endl;
	    cout << "all words: "<< speech->sentence <<endl;
	    for( int w_id = 0 ; w_id < speech->words.size() ; ++w_id )
	      {
		cout << "chech word: "<< speech->words[w_id].word << endl;
		if(wordcheck(speech->words[w_id].word,speech->words[w_id].part[0])){
		  WordProp wp_tmp;
		  wp_tmp.freq = 0;
		  wp_tmp.num = 0;
		  
		  map< string, WordProp >::iterator it_words_wp
		    = words_wp.find( speech->words[ w_id ].word );
		  if( it_words_wp != words_wp.end() )
		    {
		      wp_tmp = it_words_wp->second;
		    }
		  
		  ++wp_tmp.freq;
		  wp_tmp.num = speech->word_num;
		  Situation st_tmp;
		  st_tmp.okao_id = speech_okao_id;

	
		  sp_msg.words.push_back( speech->words[w_id] );

		  //すべての人物をpush_backする
		  map< long long, int >::iterator it_okao 
		    = tracking_to_okao.begin(); 
		  while(it_okao != tracking_to_okao.end() )
		    {
		      st_tmp.people.push_back( it_okao->second ); 
		      ++it_okao;
		    }
		  wp_tmp.situations.push_back( st_tmp );
		  
		  words_wp[ speech->words[ w_id ].word ] = wp_tmp; 
		}
	      }
	    //}
      }
  }

  bool wordcheck(string word,string part){
    if(part == "助詞" || part == "助動詞" || part == "記号" || word == "<->" || word == "")
      {
	cout << "part: " << part << ", word: "<< word << endl;
	return 0;
      }
    else
      return 1;
  }
  
  void browser(speech_msgs::SpeechConstPtr speech,int okao_id){

    if(speech->sentence == "<->")
      return;
    cout << "browser" <<endl;
    zmq::context_t context(1);
    zmq::socket_t sender(context,ZMQ_PUSH);
    msgpack::sbuffer buffer;
    sender.connect("tcp://133.19.23.224:8081");
    map<string, string> dict;

    dict["senderName"] = "speechRecognition";
    string name;
    for(int h_i = 0; h_i < human_last.size() ; ++h_i)
      {
	if( okao_id == human_last[ h_i ].max_okao_id 
	    && human_last[ h_i ].face.persons.size() )
	  {
	    cout<<"person size: " << human_last[ h_i ].face.persons.size() <<endl;
	    name = human_last[ h_i ].face.persons[0].name;
	    break;
	  }
	else
	  {
	    name = "Unknown";
	  }
      }
    cout<<"name:"<<name <<endl;
    dict["name"] = name;
    dict["time"] = speech->time;
    dict["speechRec"] = speech->sentence;
    dict["place"] = "pioneer";
    msgpack::pack(buffer,dict);
    zmq::message_t message(buffer.size());
    memcpy(message.data(),buffer.data(),buffer.size());
    sender.send(message);
  }

  //検索用関数
  //今はもっともその単語を発話した人物と、その回数のみを出力
  //今後は発話された場所や、時間の情報、発話したすべての人物
  bool wordSearchCallback(speech_msgs::SpeechSrv::Request &req,
			  speech_msgs::SpeechSrv::Response &res)
  {
    cout<<"now search word: "<< req.word << endl;
  
     map< string, WordProp >::iterator it_s
       = words_wp.find( req.word );

     if( it_s != words_wp.end() )
       {
	 //もし単語が見つかったら、その単語を発した人全員を返す？
	 //いまは最大値でいいんじゃない？
	 map< int,int > hist;
	 vector<Situation>::iterator it_name = it_s->second.situations.begin();
	 while(it_name != it_s->second.situations.end() )
	   {
	     ++hist[it_name->okao_id];
	     ++it_name;
	   }

	 map< int,int >::iterator it_hist = hist.begin();

	 int max_id = it_hist->first;
	 ++it_hist;
	 while( it_hist != hist.end() )
	   {
	     if( hist[ max_id ] < hist[ it_hist->first ] )
	       {
		 max_id = it_hist->first;
	       }
	     ++it_hist;
	   }
	 res.okao_id = max_id;
	 res.freq = hist[ max_id ];
	 return true;
       }

     /*
    while( it_s != user_words.end() )
      {    
	//単語があるかどうかを調べる
	map< string, WordProp >::iterator it_w 
	  = it_s->second.find( req.word );

	if( it_w != it_s->second.end() )
	  {
	    //単語があるなら、そのときの頻度などを返す

	    return true;
	  }

	++it_s;
      }
     */

    return false;
  }


};

int main(int argc, char** argv)
{
  ros::init(argc, argv, "speech_recog");
  SpeechRecog SRObject;
  ros::MultiThreadedSpinner spinner(3); // Use 3 threads
  spinner.spin(); 
  //ros::spin();
  return 0;
}
