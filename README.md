# Security impact
- Grading Criteria에 포함된 3가지 항목을 포함하도록 설계했습니다.

| 취약점 | 설명 |
|----------|------------------|
| 1. Control-flow hijack | DBSERVER에서의 임의 커멘드 실행이 가능합니다. |
| 2. Privacy breach | OnionMessenger에 접속한 모든 사용자의 IP, ID, public key 정보 유출이 가능합니다. |
| 3. DoS | DBSERVER 에서의 임의커멘드 실행이 가능하므로 Onion network로의 접속을 disable시킬 수 있습니다. |


# Vulnerability difficulty
- Grading Criteria에 포함된 3가지 항목을 포함하도록 설계했습니다.

| 취약점 | 설명 |
|----------|------------------|
|  1. Logic error | escapingshell()함수에서 버퍼 관리를 잘못하여, 특정 조건 아래에서 buffer overflow가 유발됩니다 |
|  2. Memory error | 1번의 로직에러를 이용하면 일부 데이터들을 조작할 수 있습니다. 예를들어 프로그램 내부 시스템 커멘드의 길이정보를 조작할 수 있습니다. |
|  3. Injection and others | 2번을 이용하여 시스템 커멘드의 길이정보를 조작한 후에는 command injection이 가능합니다. |

# Assumption
|RELRO | STACK CANARY | NX | PIE | RPATH | RUNPATH | FILE
|Partial RELRO  | Canary found | NX enabled | No PIE | No RPATH | No RUNPATH | dbserver

# etc
| Security features | Command injection filter가 적용된 프로그램으로써, default로는 command injection 이 불가능하므로 공격을 위해서는 이를 우회하여야 합니다. |


개발자는 command injection공격을 방어하기 위해서 아래와 같은 escapeshell를 추가했습니다. 
이 함수는 사용자 입력값에 포함된 위험한 문자들을 escaping 합니다. 


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
[security.c]

escapeshell이 실행되면 위험한 캐릭터 앞에 escaping 문자인 "\"가 추가되어 str의 전체 길이가 늘어납니다. 
하지만 개발자는 이 사실을 간과하였고, 결과적으로 str이 최대 길이(MAXSIZE 127)를 넘어 오버플로우될 수 있습니다. 


        char input[MAXSIZE];
        int8_t prolen; //SIZE : 00(X) 01(O)...ff(O).
        int8_t epilen;
        int8_t inputlen; 