#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define  MAXSIZE 256

void escapeshell(char *str){ // str can expend to strlen(str)*2 !
        char dangerous[] = "#&;`'\"|*?~<>^()[]{}$\\,";
        char buffer[MAXSIZE];
        int i,j;
		printf("[escapeshell] str : %s\n",str);
        for(i=0;i<strlen(str);i++){
                for(j=0;j<strlen(dangerous);j++){
                        if(str[i] == dangerous[j]){
                                strcpy(buffer, &str[i]);  
                                strcpy(&str[i++], "\\"); 
                                strcpy(str+strlen(str), buffer); 
								printf("[middle] str(%d) : %s\n",strlen(str),str);
                        }
                }
        }
		printf("[escapeshell] str : %s\n",str);
}