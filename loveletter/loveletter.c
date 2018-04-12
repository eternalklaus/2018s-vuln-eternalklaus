// echo I love + "" + very much!
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXSIZE 256

char *prolog = "echo I love ";
char *epilog = " very much!";
unsigned short int idx;

typedef struct _COMMAND{
        char name[MAXSIZE];
        unsigned int prolen; //SIZE : 0(X)1(O)ff(O). however must write [SIZE] at upper lin
        unsigned int epilen;
        unsigned int namelen;
}COMMAND;

// ♡ ♥ --> 3byte
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
                                strcat(name, buffer);
                        }
                }
        }
        printf("[DBG] name is %s(%d)\n",name, strlen(name));
}


int main(int argc, char *argv[]){
        char loveletter[1024];
        memset(loveletter,0,1024);
        COMMAND command;
        command.epilen = strlen(epilog);
        command.prolen = strlen(prolog);

        printf("[*] My lover's name is : ");
        fgets(command.name, MAXSIZE, stdin); 
        command.name[strlen(command.name)-1] = '\x0';

        printf("[*] Whatever happens, I'll protect her...\n");
        protect(command.name);
        command.namelen = strlen(command.name);

        printf("[*] Impress her name upon my memory...\n");
        memcpy(&loveletter[idx], prolog, command.prolen); idx += command.prolen;
        memcpy(&loveletter[idx], command.name, command.namelen); idx += command.namelen; // memcpy too much byte : stack smashing
        // To get reach to RET, attacker should overwrite [prolen] length at upper line... (tradeoff)
        memcpy(&loveletter[idx], epilog, command.epilen); idx += command.epilen;
       
        printf("[*] Her name echos in my mind...\n"); 
        system(loveletter);
}
