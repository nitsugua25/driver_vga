ENTRY(_start)

SECTIONS
{
    . = 0x00100000;
    
    _kernel_start = .;
    
    /* Multiboot header must be in first segment */
    .multiboot : {
        KEEP(*(.multiboot))
    }
    
    .text : {
        *(.text)
    }
    
    .rodata : {
        *(.rodata*)
    }
    
    .eh_frame : {
        *(.eh_frame)
    }
    
    .data : {
        *(.data)
    }
    
    .bss : {
        *(.bss)
        *(COMMON)
    }
    
    _kernel_end = .;
}
