# 요약
Command injection filter를 우회하여 임의 커멘드 실행을 가능케 하는 익스플로잇입니다. 
모순적이게도 이는 Command injection filter 에서의 버퍼 관리 취약점 때문에 발생합니다.

# Security impact
| 취약점 | 설명 |
|----------|------------------|
| 1. Control-flow hijack | DBSERVER에서의 임의 커멘드 실행이 가능합니다. |
| 2. Privacy breach | OnionMessenger에 접속한 모든 사용자의 IP, ID, public key 정보 유출이 가능합니다. |
| 3. DoS | 공격자는 Whole Onion network 를 disable할 수 있습니다. |

# Vulnerability difficulty
| 취약점 | 설명 |
|----------|------------------|
| Logic error | escapingshell()함수에서의 버그로 인해 특정 조건 아래에서 buffer overflow가 유발됩니다 |
| Memory error | Logic error 트리거 후에는 일부 데이터들을 조작할 수 있습니다. 공격자는 프로그램 내부 시스템 커멘드의 길이정보를 조작합니다. |
| Injection and others | 시스템 커멘드 길이정보를 조작한 후에는 command injection이 가능합니다. |

# Assumption
|RELRO | STACK CANARY | NX | PIE | RPATH | RUNPATH | FILE
|------|--------------|----|-----|-------|---------|-----
|Partial RELRO  | Canary found | NX enabled | No PIE | No RPATH | No RUNPATH | dbserver

# Exploit code
- 익스플로잇은 3-stage로 실행됩니다.  
- https://github.com/KAIST-IS521/2018s-vuln-eternalklaus/blob/master/src/gogo.py

# Exploit screen
![onion](exploit.png)
