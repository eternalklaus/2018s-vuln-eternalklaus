#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#define  BUFF_SIZE   102
#define _CRT_SECURE_NO_WARNINGS    // strtok 보안 경고로 인한 컴파일 에러 방지



char *Userlist(){
	// !userlist <- 이걸입력하면 printUserlist호출됨
	// 현재접속한 유저리스트정보를 리턴한다. 
	// run_dbserver를통해 보낼수있음
}


//성공시 1반환, 실패시0반환
int addUser(char *IpPortGithubId) { // char userIp, int userPort, char *githubID
	// OnionUser.db 에 str 한줄 추가

	// 첫줄에 라인 추가 : sed -i '1itask goes here' lll.txt
	char command[100];
	sprintf(command, "sed -i '1i%s' %s", IpPortGithubId ,"OnionUser.db"); 
	printf("addUser command : %s\n", command);
	system(command);
}

//성공시 1반환, 실패시0반환
int deleteUser(char *githubID){
	char command[100];
	
	//sed -i '/eternalklaus/d' aaa.txt 
	sprintf(command, "sed -i '/%s/d' %s", githubID ,"OnionUser.db"); 
	printf("deleteUser command : %s\n", command);
	system(command);
	

}


// usage : run_dbserver(12345)
// dbserver는 내부적으로 OnionUser.db 라는 파일을 운용한다. 
// OnionUser.db 는 사용자들의 IP, port, GithubID엔트리들을 저장하는 데이터베이스 파일이다. 
int run_dbserver(int dbserver_port){ // [TODO] add 
   int   server_socket;
   int   client_socket;
   int   client_addr_size;

   struct sockaddr_in   server_addr;
   struct sockaddr_in   client_addr;

   char   buff_rcv[BUFF_SIZE+5];
   char   buff_snd[BUFF_SIZE+5];

   server_socket  = socket( PF_INET, SOCK_STREAM, 0);
   if( -1 == server_socket)
   {
      printf( "server socket 생성 실패\n");
      exit( 1);
   }

   memset( &server_addr, 0, sizeof( server_addr));
   server_addr.sin_family     = AF_INET;
   server_addr.sin_port       = htons(dbserver_port);           
   server_addr.sin_addr.s_addr= htonl(INADDR_ANY);

   if( -1 == bind( server_socket, (struct sockaddr*)&server_addr, sizeof( server_addr) ) )
   {
      printf( "bind() 실행 에러\n");
      exit( 1);
   }

   if( -1 == listen(server_socket, 5))
   {
      printf( "listen() 실행 실패\n");
      exit(1);
   }

   while(1)
   {
      client_addr_size  = sizeof( client_addr);
      client_socket     = accept( server_socket, (struct sockaddr*)&client_addr, &client_addr_size);

      if ( -1 == client_socket)
      {
         printf( "클라이언트 연결 수락 실패\n");
         exit(1);
      }

      read(client_socket, buff_rcv, BUFF_SIZE);
      printf("receive: %s\n", buff_rcv);
      
	  
	  if (!strncmp(buff_rcv,"@adduser",strlen("@adduser"))){ // ex) @register ip port githubID 
         addUser(buff_rcv+strlen("@adduser")+1);// 한줄을몽땅 파일에 추가...
		 printf("파일에 추가함\n");
	  }
	  
	  if (!strncmp(buff_rcv,"@deleteuser",strlen("@deleteuser"))){ // ex) @deleteuser githubID 
         if(deleteUser(buff_rcv+strlen("!deleteuser")+1))
			 printf("%s : 성공적으로 삭제했다!\n", buff_rcv+sizeof("@deleteuser")+1);
		 else
			 printf("그런 이름의 유저는 존재하지 않는다...\n");
	  }
	  
      sprintf(buff_snd, "%d : %s", strlen(buff_rcv), buff_rcv);
   
	  // 클라이언트에게 보내는 서버의 메시지...
	  write(client_socket, buff_snd, strlen(buff_snd)+1);          // +1: NULL까지 포함해서 전송
      close(client_socket);
   }
}

int main()
{
	run_dbserver(4000);
}