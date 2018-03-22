#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define  BUFF_SIZE   1024


/*
int download_pubkey(){
	// wget같은걸로 ID에해당하는 키들을 모두다운로드한다
}


int register_key(){
	// 얻어온 키들을 모두 등록한다
}
*/

int main(int argc, char *argv[]){
		
	//addUser("127.0.0.1", 12345, "eternalklaus");
	//addUser("127.0.0.1", 12345, "hansh");

   int   client_socket;

   struct sockaddr_in   server_addr;

   char   buff[BUFF_SIZE+5];

   client_socket  = socket( PF_INET, SOCK_STREAM, 0);
   if( -1 == client_socket)
   {
      printf( "socket 생성 실패\n");
      exit( 1);
   }

   memset( &server_addr, 0, sizeof( server_addr));
   server_addr.sin_family     = AF_INET;
   server_addr.sin_port       = htons( 4000);
   server_addr.sin_addr.s_addr= inet_addr( "127.0.0.1");

   if( -1 == connect( client_socket, (struct sockaddr*)&server_addr, sizeof( server_addr) ) )
   {
      printf( "접속 실패\n");
      exit( 1);
   }
   
   // 처음 argv[1]에다가 메시지 전송해줘야 세그폴안뜸~!
   write( client_socket, argv[1], strlen( argv[1])+1);      // +1: NULL까지 포함해서 전송
   read ( client_socket, buff, BUFF_SIZE);
   printf( "%s\n", buff);
   close( client_socket);
   
   return 0;
}


	