#include<iostream>
#include<stdio.h>
#include<mysql/mysql.h>
#include<string>

using namespace std;

//#include"mysql/mysql.h"
//#pragma comment(lib,"lib/mysqlclient.lib")
//#pragma comment(lib,"lib/mysqlservices.lib")

#define MYSQL_SERVER "localhost" 
#define MYSQL_USERNAME "root"
#define MYSQL_PASSWORD "yuma0921"
#define MYSQL_DB NULL

int main(){

  MYSQL *mysql=NULL;
  MYSQL_RES *result;
  MYSQL_ROW row;
  
  mysql = mysql_init(NULL);
  mysql_real_connect( mysql, 
            MYSQL_SERVER, MYSQL_USERNAME,
            MYSQL_PASSWORD, MYSQL_DB, 0, NULL, 0 );
  
  if( mysql ){
    while(1){
      string command;
      string word;
      cout << "単語を入力してください" << endl;
      cin >> word;
      command = "select rd_title from test.redirect, (select page_id from test2.page where page_title=\"";
      command += word;
      command += "\") t_page where rd_from=page_id;";
      mysql_query(mysql,command.c_str());
      result = mysql_store_result( mysql );                       // SQL結果
      int count = (int)mysql_num_rows( result );             // レコード数
      if(count == 0)
	cout << "出ませんでした" << endl;
      for(int u = 0; u < count ; u++){
	row = mysql_fetch_row( result );            // 結果セットの次のレコードを取得する。
	printf( "この単語ではないですか: %s\n", row[0] );
      }
      bool restart;
      cout << "続けますか？ ok:1 no:0" << endl;
      cin >> restart;
      if(restart == 0)
	break;
    }
  }
  mysql_close( mysql );                           // MySQLからログオフ

  mysql_free_result( result );                    // 領域の解放
}