#include "include/speech.h"
 
extern void command(speech_msgs::Extract ext_msg,int okao_id);

void extract(speech_msgs::SpeechConstPtr speech,int okao_id){
    int i = 0;
    int d = 0;
    int no = 0;
    int b = 0;
    //    cout << "extract_start" << endl;
    speech_msgs::Extract ext_msg;
    ext_msg.com_jud = 0;
    for(int i = 0 ; i < speech->word_num ; ){
	if(speech->words[i].part[0] == "名詞" || speech->words[i].part[0] == "感動詞"){
	  cout << "extract now" << endl;
	  if(speech->words[i].part[0] == "名詞"){
	    struct stat st;
	    string textfile;
	    int ret;
	    const char* text;
	    if(speech->words[i].word == "バイバイ" || speech->words[i].word == "ばいばい" || speech->words[i].word == "さようなら"){
	      ext_msg.com = "bye";
	      ext_msg.com_jud = 1;
	    }
	    ext_msg.noun.push_back(speech->words[i].word);
	    d++;
	  }
	}
	else if(speech->words[i].part[0] == "動詞"){
	  cout << "verb now" << endl;
	  if(speech->words[i].word == "な"){
	    no = 1;
	  }
	  else{
	    if(speech->words[i].part[2] == "未然形"){
	      no = 1;
	    }
	    else{
	      if(speech->words[i].part[3] == "する"){
		for(b = 1; i-b != 0 ;b++){
		  if(speech->words[i-b].part[0] == "名詞"){
		    ext_msg.com = speech->words[i-b].word;
		    ext_msg.com_jud = 1;
		    //verb_pub.publish(verb_msg);
		    break;
		  }
		}
	      }
	      else{
		ext_msg.verb.push_back(speech->words[i].part[3]);
		//verb_pub.publish(verb_msg);
	      }
	    }
	  }
	}
	if(no != 1){
	  ext_msg.noun_num = d;
	}
	i++;
      }
	  cout << ext_msg.noun_num << endl;
	  if(ext_msg.verb.size() != 0 || ext_msg.com.size() != 0)
	    command(ext_msg,okao_id);
	
  }
  
