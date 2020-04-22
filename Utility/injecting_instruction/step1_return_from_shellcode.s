.section .text
.global _start
_start:
	ADD R11, SP, #12
	LDR R1, [PC, #4] @=#0x01010554
	SUB PC, R1, #0x01000000
	

# as step1.s -o step1.o && ld -N step1.o -o step1
