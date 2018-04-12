// echo I love + "" + very much!
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXSIZE 256

char *prolog = "echo I love ";
char *epilog = " very much!";
unsigned short int idx;

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
	   char loveletter[256];
	   unsigned int namelen = 0;
	   unsigned int epilen = strlen(epilog); //
	   unsigned int prolen = strlen(prolog); // SIZE : 0(X)1(O)ff(O). however must write [SIZE] at upper line
	   char name[MAXSIZE];

	   printf("[*] My lover's name is : ");
	   fgets(name, MAXSIZE, stdin); 
       name[strlen(name)-1] = '\x0';
       // getchar();   // eat newline

	   printf("[*] Whatever happens, I'll protect her...\n");
	   protect(name);

	   namelen = strlen(name);

	   printf("[*] Impress her name upon my memory....\n");
	   memcpy(&loveletter[idx], prolog, prolen); idx += prolen;
	   memcpy(&loveletter[idx], name, namelen); idx += namelen; // memcpy에 너무 많은양을 카피하는경우 : stack smashing으로 죽음. 
	   // 리턴어드레스까지 도달할수는 있어도, 위의 memcpy에서 prolen만큼을 덮기때문에 정확하게 리턴어드레스만 컨트롤하는건 불가능함. 
       memcpy(&loveletter[idx], epilog, epilen); idx += epilen;


	printf("[*] Her name echos in my mind...\n"); 
	system(loveletter);
}
