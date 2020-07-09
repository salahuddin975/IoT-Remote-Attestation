#!/usr/bin/python3

import socket
import sys
import binascii
import time
import csv
import random



SERVER_IP = '192.168.1.26'
SERVER_PORT = 8080

VERIFIER_IP = '192.168.1.29'
VERIFIER_PORT = 8080


sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.settimeout(2)
server_address = (SERVER_IP, SERVER_PORT)

verifier_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
verifier_sock.settimeout(2)
verifier_address = (VERIFIER_IP, VERIFIER_PORT)
 

def is_compromised(msg):
    sent = sock.sendto(msg, server_address)
    try:
        data, server = sock.recvfrom(4096)
    except socket.timeout:
        data = "123"
        print ("exception in server")
#    print ("Checksum: ", binascii.hexlify(data))

    sent = verifier_sock.sendto(msg, verifier_address)
    try:
        verifier_data, server = verifier_sock.recvfrom(4096)
    except socket.timeout:
        verifier_data = "231"
        print ("exception in verifier")
#    print ("Checksum: ", binascii.hexlify(verifier_data))

    if data == verifier_data:
#        print ("Not detected")
        return 0
    else:
#        print ("detected") 
        return 1
        


if __name__ == "__main__":
    block_size = 500
    total_memory_size = 5913304

    num_of_blocks = 0
    num_of_seeds = 100


    fields = ['num_of_blocks', 'block_size', 'covered_memory_proportion', 'num_of_dection(out_of_100)']
    filename = "detection_rate.csv"

    with open(filename, 'w') as csvfile: 
        csvwriter = csv.writer(csvfile)
        csvwriter.writerow(fields)

        for n in range(150):
            num_of_blocks = num_of_blocks + 100
            num_of_detection = 0
            for i in range(100):
                seed = int(time.time()) + i
                msg = str(seed) + " " + str(num_of_blocks) + " " + str(block_size) + " "
                msg = bytes(msg, 'ascii')
 
                if is_compromised(msg):
                    num_of_detection = num_of_detection + 1
        
            covered_memory = round(((num_of_blocks * block_size * 100)/total_memory_size), 3)
            csvwriter.writerow([num_of_blocks, block_size, covered_memory, num_of_detection])
            print ("num_of_block: ", num_of_blocks, "; block_size: ", block_size, "; covered_memory: ", covered_memory, "%; num_of_detection: ", num_of_detection)
            time.sleep(1)        

sock.close()
verifier_sock.close()



