#!/usr/bin/python3

import socket
import sys
import binascii


IRRIGATION_SERVER_IP = '192.168.1.26'
IRRIGATION_SERVER_PORT = 8080

SMARTHOME_SERVER_IP = '192.168.1.27'
SMARTHOME_SERVER_PORT = 8080

DNS_SERVER_IP = '192.168.1.28'
DNS_SERVER_PORT = 8080

RCCM_SERVER_IP = '192.168.1.29'
RCCM_SERVER_PORT = 8080


seed = "10"
num_of_blocks = "2000"
block_size = "2000"

msg = seed + " " + num_of_blocks + " " + block_size + " "
msg = bytes(msg, 'ascii')


sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

while 1:
    print ("\n\nPlease select server to get checksum value: ")
    print ("Irrigation Server --> 1")
    print ("Smart Home --> 2")
    print ("DNS server --> 3")
    print ("Remote Control Car Monitor --> 4")
    print ("Exit from the program --> 5")

    val = int(input())

    if val == 1:
        server_address = (IRRIGATION_SERVER_IP, IRRIGATION_SERVER_PORT)
    elif val == 2:
        server_address = (SMARTHOME_SERVER_IP, SMARTHOME_SERVER_PORT)
    elif val == 3:
        server_address = (DNS_SERVER_IP, DNS_SERVER_PORT)
    elif val == 4:
        server_address = (RCCM_SERVER_IP, RCCM_SERVER_PORT)
    elif val == 5:
        print ("exit")
        sys.exit()
    else:
        continue
    
    sent = sock.sendto(msg, server_address)
    data, server = sock.recvfrom(4096)
    print ("Checksum: ", binascii.hexlify(data))


sock.close()


