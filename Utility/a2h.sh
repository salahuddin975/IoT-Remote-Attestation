as $1.s -o $1.o && ld -N $1.o -o $1

objcopy -O binary $1 $1.bin

hexdump -v -e '"\\""x" 1/1 "%02x" ""' $1.bin


echo ""

