#!/bin/bash

gcc -o SmartHome main.c -l bcm2835 -fno-stack-protector -zexecstack
