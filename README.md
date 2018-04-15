# Summery
Command injection filter를 우회하여 임의 커멘드 실행을 가능케 하는 익스플로잇입니다.  
모순적이게도 이는 Command injection filter 내부의 버퍼 관리 취약점 때문에 발생합니다.  

# Assumption
기본적으로는 아래와 같은 Security feature 이 적용되어 있는 weak assumption model을 따릅니다.  
추가적으로, 프로그램 내부에 commend injection filter이 적용되었으므로 단순 커멘드인젝션 공격은 불가능합니다.  
따라서 임의커멘드 실행을 위해서 이를 우회하여야 합니다.  
  
|RELRO | STACK CANARY | NX | PIE | RPATH | RUNPATH | FILE
|------|--------------|----|-----|-------|---------|-----
|Partial RELRO  | Canary found | NX enabled | No PIE | No RPATH | No RUNPATH | dbserver

# Security impact
| 취약점 | 설명 |
|----------|------------------|
| Control-flow hijack | DBSERVER에서의 임의 커멘드 실행이 가능합니다. |
| Privacy breach | OnionMessenger에 접속한 모든 사용자의 IP, ID, public key 정보 유출이 가능합니다. |
| DoS | 공격자는 Whole Onion network 를 disable할 수 있습니다. |

# Vulnerability difficulty
| 취약점 | 설명 |
|----------|------------------|
| Logic error | escapingshell()함수에서의 버그로 인해 특정 조건 아래에서 buffer overflow가 유발됩니다 |
| Memory error | Logic error 트리거 후에는 일부 데이터들을 조작할 수 있습니다. 공격자는 프로그램 내부 시스템 커멘드의 길이정보를 조작합니다. |
| Injection and others | 시스템 커멘드 길이정보를 조작한 후에는 command injection이 가능합니다. |


# Exploit code
- 익스플로잇은 3-stage로 실행됩니다.  
- https://github.com/KAIST-IS521/2018s-vuln-eternalklaus/blob/master/exploit/gogo.py  

# Exploit 
[VICTIM]
1. Directory `src`
2. Compile dbserver by `gcc -o dbserver dbserver.c security.c`
3. Run dbserver `./dbserver`
  
[ATTACKER]
1. Directory `exploit`
2. Run docker image by `sudo ./run.sh` command.
3. Inside docker image, run exploit code `./gogo.py`.
4. Then get a remote shell.
  

# Exploit result
![onion](exploit.png)
