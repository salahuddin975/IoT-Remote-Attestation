#!/usr/bin/python3

import socket
import sys
import binascii


IRRIGATION_SERVER_IP = '192.168.1.25'
IRRIGATION_SERVER_PORT = 8080

SMARTHOME_SERVER_IP = '192.168.1.26'
SMARTHOME_SERVER_PORT = 8081

DNS_SERVER_IP = '192.168.1.27'
DNS_SERVER_PORT = 8082

RCCM_SERVER_IP = '192.168.1.28'
RCCM_SERVER_PORT = 8083


message = b"calculate checksum"
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
    else:
        print ("exit")
        sys.exit()
    
    sent = sock.sendto(message, server_address)
    data, server = sock.recvfrom(4096)
    print ("Checksum: ", binascii.hexlify(data))


sock.close()


