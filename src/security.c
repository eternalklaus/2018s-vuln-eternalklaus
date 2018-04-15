#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define  MAXSIZE 128

void escapeshell(char *str){ // [BUG] str can expend to strlen(str)*2 !
        char dangerous[] = "#&;`'\"|*?~<>^()[]{}$\\,";
        char buffer[MAXSIZE];
        int i,j;
		printf("[DBSERVER] securely escaping client's input...\n");
		printf("           %s\n",str);
        for(i=0;i<strlen(str);i++){
                for(j=0;j<strlen(dangerous);j++){
                        if(str[i] == dangerous[j]){
                                strcpy(buffer, &str[i]);  
                                strcpy(&str[i++], "\\"); 
                                strcpy(str+strlen(str), buffer); 
								// printf("[middle] str(%d) : %s\n",strlen(str),str);
                        }
                }
        }
		printf("       --> %s\n",str);
}
