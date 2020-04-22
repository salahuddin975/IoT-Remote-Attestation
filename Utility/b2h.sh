#!/bin/bash

echo "usage: ./dump2str.py <BINARY>"

#echo "\"$(objdump -d shellcode | grep '[0-9a-f]:' | cut -d$'\t' -f2 | grep -v 'file' | tr -d " \n" | sed 's/../\\x&/g')\"" | python shell_length.py

echo "\"$(objdump -d $1 | grep '[0-9a-f]:' | cut -d$'\t' -f2 | grep -v 'file' | tr -d " \n" | sed 's/../\\x&/g')\""

#python -c 'import sys;data=sys.stdin.read();print data'
