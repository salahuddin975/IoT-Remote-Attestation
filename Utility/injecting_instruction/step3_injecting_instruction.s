.section .text
.global _start
_start:
 .ARM
 add   r3, pc, #1       // switch to thumb mode 
 bx    r3

.THUMB                  // Provide code segment read, write, execute permission
// mprotect(base_addr(0x00010000), len (2000), 7) 
 ldr   r0, =0x01112102  //=====* set base address
 ldr   r1, =0x01100102
 sub   r0, r0, r1
 ldr   r1, =#4200
 mov   r2, #7
 mov   r7, #125
 svc   #1          

 add r3, #7
 bx r3

.ARM 
 add   r3, pc, #1       // switch to thumb mode 
 bx    r3


.THUMB                  // Change instruction in code segment
// mov r0, r5             // r5 -> store last address of a function

 ldr r0, =0x010127c4    //=====* set location to change instruction
 ldr r1, =0x01000000
 sub r0, r0, r1

 ldr r1, =0xe1a01001    // NoP instruction
 str r1, [r0]

 add r3, #7             // switch to normal mode
 bx r3

.ARM                    // restore R11, and PC value to continue program normally
 ADD R11, SP, #28       // =====* set r11 of caller function       

 LDR R1, [PC, #20]      // 16 -> 4 ldr used in this program
 SUB PC, R1, #0x01000000   // To adjust loaded return value


 
