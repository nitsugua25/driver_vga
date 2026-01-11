#ifndef PCI_H
#define PCI_H

#include <stdint.h>

typedef struct {
    uint8_t bus;
    uint8_t slot;
    uint8_t func;
    uint16_t vendor_id;
    uint16_t device_id;
    uint32_t bar0;
} pci_device_t;

/* Scan PCI bus for VGA device */
int pci_scan_vga(pci_device_t *dev);

#endif
