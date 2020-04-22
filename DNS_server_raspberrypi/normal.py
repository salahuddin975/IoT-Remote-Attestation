#!/usr/bin/python

import sys, socket


shellcode = ("AAAA")


if __name__ == '__main__':
    if(len(sys.argv)) != 3:
        print "./so.py localhost 5555"
        sys.exit()

    sock=socket.socket(socket.AF_INET, socket.SOCK_DGRAM,0)
    server_addr = (sys.argv[1], int(sys.argv[2]))
    sock.sendto(shellcode, server_addr)
    sock.close()

    print "Shellcode Length: %d" %len(shellcode)



