#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/param.h>
#include <sys/uio.h>
#include <unistd.h>

#define BUF_LEN 256                      /* バッファのサイズ */

int main(int argc, char *argv[]){
  int s;  /* ソケットのためのファイルディスクリプタ */
  int client_id;
  size_t goods_num,client_num;
  struct hostent *servhost;            /* ホスト名と IP アドレスを扱うための構造体 */
  struct sockaddr_in server;           /* ソケットを扱うための構造体 */
  struct servent *service;             /* サービス (http など) を扱うための構造体 */
  char buf[BUF_LEN];
  int read_size;

  char *host;
  //char host[BUF_LEN] = "127.0.0.1";    /* 接続するホスト名 */
  char path[BUF_LEN] = "/";            /* 要求するパス */
  unsigned short port = 5000;             /* 接続するポート番号 */

  /* 引数が不足している場合 */
  if (argc < 3) {
    printf("usage: client [hostname] [port]\n");
    exit(1);
  }

  /* コマンドライン引数からサーバ名を取得 */
  if (argc > 1) {
    host = (char *)argv[1];
  }

  if (argc > 2) {
    port = atoi(argv[2]);
  }

  /* ホストの情報(IPアドレスなど)を取得 */
  servhost = gethostbyname(host);
  if ( servhost == NULL ){
    fprintf(stderr, "[%s] から IP アドレスへの変換に失敗しました。\n", host);
    return 0;
  }

  bzero(&server, sizeof(server));            /* 構造体をゼロクリア */

  server.sin_family = AF_INET;

  /* IPアドレスを示す構造体をコピー */
  bcopy(servhost->h_addr, &server.sin_addr, servhost->h_length);

  server.sin_port = htons(port);

  /* ソケット生成 */
  if ( ( s = socket(AF_INET, SOCK_STREAM, 0) ) < 0 ){
    fprintf(stderr, "ソケットの生成に失敗しました。\n");
    return 1;
  }
  /* サーバに接続 */
  if ( connect(s, (struct sockaddr *)&server, sizeof(server)) == -1 ){
    fprintf(stderr, "connect に失敗しました。\n");
    return 1;
  }
  //PLEASE INPUT YOUR NAME
  /* 名前の送信 */
  read_size = read(s, buf, BUF_LEN);//sにつながっているファイルからBUF_LENバイト読み込みbuf格納
  if ( read_size > 0 ){
    write(1, buf, read_size);
  }
   fgets(buf, BUF_LEN, stdin);
    write(s, buf, strlen(buf));





  //a1:takeda a2:hajime a3:take 
 
  //自分のidとクライアント名を覚える
  read_size = read(s, buf, BUF_LEN);
  if (read_size > 0)
    write(1, buf, read_size);
  buf[read_size] = '\0';//文字列の最後という印

 

  // "Your ID: x", x はクライアントID
  if ((read_size = read(s, buf, BUF_LEN)) > 0)
    write(1, buf, read_size);
  buf[read_size] = '\0';
  sscanf(buf, "%*[^0-9]%d", &client_id);
  printf("got your client_id:%d\n",client_id);
  //sscanfは要はscanfの入力がキーボードからではなく文字列strになる
  //sscanf(const char *str入力元の文字列, const char *format書式指定文字列,格納する引数)
  //0-9までの%d(10進数int型)
  //^はYOUR ID:に相当し、捨てる

  // "n,m", n は商品数 m はクライアント合計数
  if ((read_size = read(s, buf, BUF_LEN)) > 0)
    write(1, buf, read_size);
  buf[read_size] = '\0';
  sscanf(buf, "%zu%*[^0-9]%zu", &goods_num, &client_num);
  printf("got goods_num:%zu,client_num:%zu\n",goods_num,client_num);
  



  puts("入札情報を入力");

  //それぞれの商品について入札するかどうか。各商品に対して入札する場合は1，しない場合は0
  
  //標準入力でサーバーに情報を送信
  // fgets(buf, BUF_LEN, stdin);

  //標準入力を使わずにサーバーに情報を送信
   int i=0;
  for (i=0;i<goods_num;i++){
  buf[i] = '1';
  }
   buf[i] = '\n';
   buf[i+1] = '\0';
   printf("最初の入札");

  write(s, buf, strlen(buf));//ソケットにつながるsにbufをstrlenバイト書き込む
  

  read_size = read(s, buf, BUF_LEN);
  if ( read_size > 0 ){
    write(1, buf, read_size);
  }

  /* IDとクライアント数、商品数の読み込み */
  read_size = read(s, buf, BUF_LEN);
  if ( read_size > 0 ){
    write(1, buf, read_size);
  }

  read_size = read(s, buf, BUF_LEN);
  if ( read_size > 0 ){
    write(1, buf, read_size);
  }

  /* 送受信ループ */
  /* 標準入力をそのまま送信、入札結果と価格を受信して表示 */
  while (1) {
    fgets(buf, BUF_LEN, stdin);
    write(s, buf, strlen(buf));

    read_size = read(s, buf, BUF_LEN);
    if ( read_size > 0 ){
      write(1, buf, read_size);
    }
    read_size = read(s, buf, BUF_LEN);
    if ( read_size > 0 ){
      write(1, buf, read_size);
    }
  }

  close(s);

  return 0;
}
