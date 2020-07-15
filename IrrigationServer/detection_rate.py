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
    exception = False
    sent = sock.sendto(msg, server_address)
    try:
        data, server = sock.recvfrom(4096)
    except socket.timeout:
        data = "123"
        exception = True
        print ("exception in server")
#    print ("Checksum: ", binascii.hexlify(data))

    sent = verifier_sock.sendto(msg, verifier_address)
    try:
        verifier_data, server = verifier_sock.recvfrom(4096)
    except socket.timeout:
        verifier_data = "231"
        exception = True
        print ("exception in verifier")
#    print ("Checksum: ", binascii.hexlify(verifier_data))

    if data == verifier_data or exception:
#        print ("Not detected")
        return 0
    else:
#        print ("detected") 
        return 1
        


if __name__ == "__main__":
    block_size = 500
    total_memory_size = 5913304

    num_of_blocks = 0
    num_of_iteration = int(total_memory_size / block_size) + 5
    increment_size = 300

    num_of_seeds = 500


    fields = ['num_of_blocks', 'block_size', 'covered_memory_proportion', 'dection_rate', 'spend_time', 'avg_spend_time']
    filename = "detection_rate.csv"

    with open(filename, 'w') as csvfile: 
        csvwriter = csv.writer(csvfile)
        csvwriter.writerow(fields)

        for n in range(num_of_iteration):
            num_of_blocks = num_of_blocks + increment_size
            num_of_detection = 0
            start_time = time.time()

            for i in range(num_of_seeds):
                seed = int(time.time()) + i * i
                msg = str(seed) + " " + str(num_of_blocks) + " " + str(block_size) + " "
                msg = bytes(msg, 'ascii')
 
                if is_compromised(msg):
                    num_of_detection = num_of_detection + 1
        
            end_time = time.time()
            spend_time = round((end_time - start_time), 3)
            avg_spend_time = round((spend_time/num_of_seeds), 4)
            covered_memory = round(((num_of_blocks * block_size * 100)/total_memory_size), 3)
            detection_rate = round((num_of_detection * 100)/num_of_seeds, 3)

            csvwriter.writerow([num_of_blocks, block_size, covered_memory, detection_rate, spend_time, avg_spend_time])
            print ("num_of_block: ", num_of_blocks, "; block_size: ", block_size, "; covered_memory: ", covered_memory, "%; detection_rate: ", detection_rate, "; spend_time: ", spend_time, "; avg_spend_time: ",  avg_spend_time)
            time.sleep(1)        

sock.close()
verifier_sock.close()



