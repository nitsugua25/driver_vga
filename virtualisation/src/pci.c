#include "pci.h"

#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA    0xCFC

/* I/O port functions */
static inline void outl(uint16_t port, uint32_t val) {
    __asm__ volatile ("outl %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint32_t inl(uint16_t port) {
    uint32_t ret;
    __asm__ volatile ("inl %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

/* Read PCI configuration register */
static uint32_t pci_read_config(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
    uint32_t address = (uint32_t)(
        (1 << 31) |                    // Enable bit
        ((uint32_t)bus << 16) |        // Bus number
        ((uint32_t)slot << 11) |       // Device/slot number
        ((uint32_t)func << 8) |        // Function number
        (offset & 0xFC)                // Register offset (aligned)
    );
    
    outl(PCI_CONFIG_ADDRESS, address);
    return inl(PCI_CONFIG_DATA);
}

/* Scan for VGA device (Class 0x03, Subclass 0x00) */
int pci_scan_vga(pci_device_t *dev) {
    for (uint16_t bus = 0; bus < 256; bus++) {
        for (uint8_t slot = 0; slot < 32; slot++) {
            uint32_t vendor = pci_read_config(bus, slot, 0, 0x00);
            
            if (vendor == 0xFFFFFFFF)
                continue; // No device
            
            uint16_t vendor_id = vendor & 0xFFFF;
            uint16_t device_id = (vendor >> 16) & 0xFFFF;
            
            // Read class code (offset 0x08)
            uint32_t class_code = pci_read_config(bus, slot, 0, 0x08);
            uint8_t base_class = (class_code >> 24) & 0xFF;
            uint8_t sub_class = (class_code >> 16) & 0xFF;
            
            // Check if VGA controller (Class 0x03, Subclass 0x00)
            if (base_class == 0x03 && sub_class == 0x00) {
                dev->bus = bus;
                dev->slot = slot;
                dev->func = 0;
                dev->vendor_id = vendor_id;
                dev->device_id = device_id;
                
                // Read BAR0 (offset 0x10)
                dev->bar0 = pci_read_config(bus, slot, 0, 0x10);
                
                return 0; // Success
            }
        }
    }
    
    return -1; // VGA device not found
}
