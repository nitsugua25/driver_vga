/* boot.S - Multiboot header and kernel entry */

.set ALIGN,    1<<0
.set MEMINFO,  1<<1
.set FLAGS,    ALIGN | MEMINFO
.set MAGIC,    0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

/* Multiboot header - MUST be in first 8KB of kernel */
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

/* Reserve stack */
.section .bss
.align 16
stack_bottom:
.skip 16384  # 16 KB stack
stack_top:

/* Entry point */
.section .text
.global _start
.type _start, @function

_start:
    /* Set up stack */
    movl $stack_top, %esp
    
    /* Reset EFLAGS */
    pushl $0
    popf
    
    /* Call kernel main */
    call kernel_main
    
    /* Hang if kernel returns */
    cli
1:  hlt
    jmp 1b

.size _start, . - _start
