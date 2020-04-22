#!/bin/bash

if [ $# -eq 0 ]
  then
    echo "Enter Name of your executable"
    echo "Format: hexstring.sh <binary>"
fi

if [ $# -eq 1 ]
  then
	objcopy -O binary $1 tmp.bin
	hexdump -v -e '"\\""x" 1/1 "%02x" ""' tmp.bin
fi

rm -fr tmp.bin
echo ""
