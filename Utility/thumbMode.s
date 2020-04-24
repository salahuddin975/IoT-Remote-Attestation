.section .text
.global _start
_start:
    .ARM
    add r3, pc, #1         // switch to thumb mode 
    bx r3
    
    // necessary instructions to follow
    
    .THUMB
    add r3, #7         // switch to normal mode
    bx r3
