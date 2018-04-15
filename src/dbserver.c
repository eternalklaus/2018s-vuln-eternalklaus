// @userlist
// @adduser "192.168.0.1 2222 eternalklaus"
// @deleteuser eternlaklaus
// 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#define  BUFF_SIZE   1024
#define  PARAM_SIZE  512
#define  MAXSIZE 127

#define _CRT_SECURE_NO_WARNINGS 


char *escapeshell(char* str); 

typedef struct _CMDBLOCK{
        char input[MAXSIZE];
        int8_t prolen; //SIZE : 00(X) 01(O)...ff(O).
        int8_t epilen;
        int8_t inputlen; 
}CMDBLOCK;


// @userlist
char *Userlist(){ // User should download result as OnionUser.db.tmp
	   int fd;
	   char *buff = (char*)malloc(BUFF_SIZE); 
	   fd=open("OnionUser.db",O_RDONLY);
	   read(fd,buff,BUFF_SIZE-1);
	   close(fd);
	   return buff;
}

// @adduser
int addUser(char *IpPortGithubId) { 
	CMDBLOCK c; 
	char *prolog = "sed -i '1i"; 
	char *epilog = " ' OnionUser.db";
	unsigned int idx = 0;
	char cmd[1024] = {0,};
	
	memset(c.input, 0, MAXSIZE);
	strncpy(c.input, IpPortGithubId, MAXSIZE); // safely copied to c.input
	
	c.prolen = strlen(prolog);      // 2. modified to len(sh -c blabla..)!
	c.epilen = strlen(epilog);      // 2. modified to 0!
	c.inputlen = strlen(c.input);   //    safe... 
	                                //    참고) 0xff 인 경우 strncpy의 파라미터패싱과정에서 ffffffffffffffff이 됨. 따라서 MAXSIZE < 128이 되야함. 
	escapeshell(c.input);           // 1. bof!
	
	strncpy(&cmd[idx], prolog, c.prolen);    idx += c.prolen; 
	strncpy(&cmd[idx], c.input, c.inputlen); idx += c.inputlen;
	strncpy(&cmd[idx], epilog, c.epilen);    idx += c.epilen;
	
	printf("[DBSERVER] executing command...\n");
	printf("           \"%s\"\n", cmd);
	system(cmd);
	
	return 1; 
}

// @deleteuser
int deleteUser(char *githubID){
	CMDBLOCK c; 
	char *prolog = "sed -i '/ ";
	char *epilog = "/d' OnionUser.db";
	unsigned int idx = 0;
	char cmd[1024] = {0,};
	
	memset(c.input, 0, MAXSIZE);
	strncpy(c.input, githubID, MAXSIZE); // safely copied to c.input
	
	c.prolen = strlen(prolog);      // 2. modified to len(sh -c blabla..)!
	c.epilen = strlen(epilog);      // 2. modified to 0!
	c.inputlen = strlen(c.input);   
	
	escapeshell(c.input);           // 1. bof!
	
	strncpy(&cmd[idx], prolog, c.prolen);    idx += c.prolen; 
	strncpy(&cmd[idx], c.input, c.inputlen); idx += c.inputlen;
	strncpy(&cmd[idx], epilog, c.epilen);    idx += c.epilen;
	
	printf("[DBSERVER] executing command...\n");
	printf("           \"%s\"\n", cmd);
	system(cmd);
	
	return 1; 

}

int run_dbserver(int dbserver_port){
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
      printf( "[DBSERVER] server socket error\n");
      exit( 1);
   }

   memset( &server_addr, 0, sizeof( server_addr));
   server_addr.sin_family     = AF_INET;
   server_addr.sin_port       = htons(dbserver_port);           
   server_addr.sin_addr.s_addr= htonl(INADDR_ANY);

   if( -1 == bind( server_socket, (struct sockaddr*)&server_addr, sizeof( server_addr) ) )
   {
      printf( "[DBSERVER] bind() error\n");
      exit( 1);
   }

   if( -1 == listen(server_socket, 5))
   {
      printf( "[DBSERVER] listen() error\n");
      exit(1);
   }

   char* ipstr = malloc(100);
   char* param = malloc(PARAM_SIZE);
   
   
   
   while(1)
   {
	  memset(ipstr, 0, 100);
	  memset(param, 0, PARAM_SIZE);
	  
	  memset(buff_rcv, 0, BUFF_SIZE+5);
	  memset(buff_snd, 0, BUFF_SIZE+5);
   
      client_addr_size  = sizeof( client_addr);
      client_socket     = accept( server_socket, (struct sockaddr*)&client_addr, &client_addr_size);

	  struct sockaddr_in *s = (struct sockaddr_in *)&client_addr;
      memset(ipstr, 0, 100);
      inet_ntop(AF_INET, &s->sin_addr, ipstr, 100);
      printf("[DBSERVER] Peer IP address: %s\n", ipstr);

	  
	  
      if ( -1 == client_socket)
      {
         printf( "[DBSERVER] Client connection failed\n");
         exit(1);
      }

      read(client_socket, buff_rcv, BUFF_SIZE);
      
	  if (!strncmp(buff_rcv,"@adduser",strlen("@adduser"))){ 
		  strcpy(param, ipstr);
		  strncat(param, buff_rcv+strlen("@adduser"), PARAM_SIZE-strlen(ipstr)-1); 
		  param[strlen(param)-1] = 0; // eat newline
		  addUser(param);                 
		  printf("[DBSERVER] User login : %s\n\n",buff_rcv+strlen("@adduser")+1); 
	  }
	  
	  if (!strncmp(buff_rcv,"@deleteuser",strlen("@deleteuser"))){ 
		  strncpy(param, buff_rcv+strlen("@deleteuser")+1, PARAM_SIZE-1);
		  param[strlen(param)-1] = 0; // eat new line
          deleteUser(param);
		  printf("[DBSERVER] User logout : %s\n",buff_rcv+sizeof("@deleteuser"));  
	  }
	  
	  if (!strncmp(buff_rcv,"@userlist",strlen("@userlist"))){
          snprintf(buff_snd, BUFF_SIZE, "%s", Userlist());  
	  }
	  write(client_socket, buff_snd, strlen(buff_snd)+1);  
      close(client_socket);
   }
}

int main(int argc, char *argv[])
{
	//deleteUser(argv[1]);
	run_dbserver(4000); // 4000번포트사용
}

