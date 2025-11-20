#include <stdio.h>
#include "pci.h"
#include "framebuffer.h"
#include "vga.h"

int main() {
    pci_device_t vga_dev;
    framebuffer_t fb;

    // Scan PCI to find VGA device
    if (pci_scan_vga(&vga_dev) != 0) {
        printf("VGA device not found!\n");
        return 1;
    }

    printf("VGA detected: vendor=0x%x device=0x%x BAR0=0x%x\n",
           vga_dev.vendor_id, vga_dev.device_id, vga_dev.bar0);

    //  Initialize real framebuffer via BAR
    // Text mode 25x80
    framebuffer_init(&fb, vga_dev.bar0, VGA_TEXT_COLS, VGA_TEXT_ROWS);

    // ️ Display text on the real framebuffer
    vga_init_text(&fb); // Clear screen
    vga_write_text(0, 0, "HELLO TEXT MODE", 0x0F, &fb); // white on black
    vga_write_text(1, 0, "VGA VIRTUALIZATION", 0x0F, &fb);

    // Switch to graphics mode 640x480
    framebuffer_init(&fb, vga_dev.bar0, 640, 480);

    // Draw text in graphics mode using font
    vga_draw_string(&fb, 50, 50, "HELLO GRAPHICS");
    vga_draw_string(&fb, 50, 70, "A-Z FONT TEST");

    printf("Text and graphics written to VGA framebuffer.\n");

    // In a real kernel, the program would continue running here
    return 0;
}
