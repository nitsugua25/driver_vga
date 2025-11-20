#include "pci.h"

// Stub: simple scan, in QEMU/Bochs you would read PCI config space
int pci_scan_vga(pci_device_t *dev) {
    // Example hardcoded VGA device (QEMU standard)
    dev->bus = 0;
    dev->slot = 2;
    dev->func = 0;
    dev->vendor_id = 0x1234;  // replace by real vendor ID
    dev->device_id = 0x1111;  // replace by real device ID
    dev->bar0 = 0xA0000;      // default framebuffer BAR
    return 0; // success
}

