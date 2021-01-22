#!/bin/bash

g++ main.c ./../common/checksum.cpp -l bcm2835 -fno-stack-protector -zexecstack -lpthread -fpermissive -lssl -lcrypto   -o SmartHome
