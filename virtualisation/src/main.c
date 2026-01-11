#include "vga.h"
#include "pci.h"

void kernel_main(void) {
    // Initialize VGA text mode
    vga_init_text();
    
    // Clear screen
    vga_clear();
    
    // Display header
    vga_write_text(0, 0, "VGA DRIVER   OS PROJECT", VGA_COLOR(VGA_WHITE, VGA_BLUE));
    
    // Display messages
    vga_write_text(2, 5, "HELLO FROM KERNEL", VGA_COLOR(VGA_LIGHT_GREEN, VGA_BLACK));
    vga_write_text(4, 5, "TEXT MODE WORKING", VGA_COLOR(VGA_YELLOW, VGA_BLACK));
    
    // Scan PCI for VGA device
    pci_device_t vga_dev;
    if (pci_scan_vga(&vga_dev) == 0) {
        vga_write_text(6, 5, "PCI VGA FOUND", VGA_COLOR(VGA_LIGHT_CYAN, VGA_BLACK));
        vga_write_text(7, 7, "BUS SCAN COMPLETE", VGA_COLOR(VGA_WHITE, VGA_BLACK));
    } else {
        vga_write_text(6, 5, "PCI SCAN COMPLETE", VGA_COLOR(VGA_LIGHT_CYAN, VGA_BLACK));
    }
    
    vga_write_text(10, 5, "KERNEL LOADED SUCCESSFULLY", VGA_COLOR(VGA_LIGHT_MAGENTA, VGA_BLACK));
    
    // Kernel loop
    while(1) {
        __asm__ volatile ("hlt");
    }
}
