# \# VGA Driver \& PCI - OS Project

# 

# \## 📚 Project Overview

# 

# This project implements a \*\*VGA text mode driver\*\* in two parts:

# 1\. \*\*Simulation\*\*: Software simulation that generates a BMP image

# 2\. \*\*Virtualization\*\*: Real kernel driver that runs in QEMU/Bochs

# 

# ---

# 

# \## 🎯 What This Project Does

# 

# \### Simple Explanation:

# The computer screen in text mode is a grid of \*\*25 rows × 80 columns\*\*. Each cell can show one letter (A-Z) or space, with colors. This project:

# \- \*\*Simulates\*\* this grid in software and saves it as an image (Part 1)

# \- \*\*Actually controls\*\* the real screen in a mini operating system kernel (Part 2)

# 

# ---

# 

# \## 📂 Project Structure

# ```

# driver\_vga/

# ├── simulation/              # Part 1: Software simulation

# │   ├── include/

# │   │   ├── vga.h           # VGA functions declarations

# │   │   ├── font.h          # Font data for rendering letters

# │   │   └── bmp.h           # BMP image generation

# │   ├── src/

# │   │   ├── main.c          # Test program

# │   │   ├── vga.c           # VGA simulation logic

# │   │   ├── font.c          # 8×8 pixel fonts for A-Z

# │   │   └── bmp.c           # Creates .bmp files

# │   └── Makefile

# │

# └── virtualisation/          # Part 2: Real kernel driver

# &nbsp;   ├── include/

# &nbsp;   │   ├── vga.h           # VGA hardware control

# &nbsp;   │   └── pci.h           # PCI bus scanning

# &nbsp;   ├── src/

# &nbsp;   │   ├── boot.S          # Kernel entry point (assembly)

# &nbsp;   │   ├── main.c          # Kernel main function

# &nbsp;   │   ├── vga.c           # Direct hardware VGA control

# &nbsp;   │   └── pci.c           # Scan PCI bus for VGA card

# &nbsp;   ├── linker.ld           # Memory layout for kernel

# &nbsp;   └── Makefile

# ```

# 

# ---

# 

# \## 🔧 Part 1: SIMULATION

# 

# \### What It Does:

# Creates a \*\*fake VGA screen\*\* in memory (like a virtual screen), draws text on it, and converts it to a BMP image file.

# 

# \### Key Concepts You Need to Know:

# 

# \#### 1. \*\*VGA Text Buffer (25×80)\*\*

# ```c

# static uint16\_t vga\_buffer\[25]\[80];

# ```

# \- \*\*25 rows\*\* (vertical) by \*\*80 columns\*\* (horizontal)

# \- Each position stores \*\*one character\*\*

# \- Each character = \*\*2 bytes\*\* (16 bits):

# &nbsp; - \*\*Lower 8 bits\*\*: ASCII character code (like 'A' = 65)

# &nbsp; - \*\*Upper 8 bits\*\*: Color information

# 

# \*\*Example\*\*: To store white 'A' on blue background at position \[0]\[0]:

# ```c

# vga\_buffer\[0]\[0] = 'A' | (color << 8);

# ```

# 

# \#### 2. \*\*Color Encoding (2 bytes per character)\*\*

# ```

# Bit layout:

# \[15-12]: Background color (4 bits) = 16 colors

# \[11-8]:  Foreground color (4 bits) = 16 colors  

# \[7-0]:   ASCII character (8 bits) = 256 possible chars

# 

# Example: 0x1F41

# &nbsp; - 0x1 (bits 12-15) = Blue background

# &nbsp; - 0xF (bits 8-11)  = White text

# &nbsp; - 0x41 (bits 0-7)  = Character 'A'

# ```

# 

# \*\*Why 4 bits for color?\*\* 4 bits = 16 possible values = 16 colors:

# \- 0=Black, 1=Blue, 2=Green, 3=Cyan, 4=Red, 5=Magenta, 6=Brown, 7=Light Grey

# \- 8=Dark Grey, 9=Light Blue, 10=Light Green, 11=Light Cyan, 12=Light Red, 13=Light Magenta, 14=Yellow, 15=White

# 

# \#### 3. \*\*Font Rendering (8×8 pixels)\*\*

# Each letter is stored as \*\*8 bytes\*\*, where each byte represents one row of 8 pixels.

# ```c

# // Letter 'A' example (8×8 pixels)

# static const uint8\_t font\_A\[8] = {

# &nbsp;   0b00111100,  // Row 1:    \*\*\*\*

# &nbsp;   0b01100110,  // Row 2:   \*\*  \*\*

# &nbsp;   0b11000011,  // Row 3:  \*\*    \*\*

# &nbsp;   0b11111111,  // Row 4:  \*\*\*\*\*\*\*\*

# &nbsp;   0b11000011,  // Row 5:  \*\*    \*\*

# &nbsp;   0b11000011,  // Row 6:  \*\*    \*\*

# &nbsp;   0b11000011,  // Row 7:  \*\*    \*\*

# &nbsp;   0b00000000   // Row 8:  (empty)

# };

# ```

# 

# \*\*How it works\*\*: 

# \- 1 bit = white pixel

# \- 0 bit = black pixel

# \- When rendering, we check each bit and draw the corresponding pixel

# 

# \#### 4. \*\*Framebuffer (640×480 pixels)\*\*

# ```c

# static uint8\_t framebuffer\[480]\[640]\[3];

# ```

# \- \*\*480 rows\*\* × \*\*640 columns\*\* of pixels

# \- Each pixel = \*\*3 bytes\*\* (RGB: Red, Green, Blue)

# \- Why 640×480? Because 80 chars × 8 pixels = 640, and 25 rows × 8 pixels = 480

# 

# \*\*Conversion\*\*:

# \- VGA text buffer (25×80 chars) → Framebuffer (480×640 pixels)

# \- Each character occupies 8×8 pixels

# 

# \#### 5. \*\*BMP File Format\*\*

# ```

# BMP File structure:

# ├── Header (54 bytes)

# │   ├── "BM" signature

# │   ├── File size

# │   ├── Pixel data offset (54)

# │   └── Image dimensions

# └── Pixel Data

# &nbsp;   └── RGB values (bottom row first!)

# ```

# 

# \*\*Important\*\*: BMP stores pixels \*\*bottom-to-top\*\*, so we write row 479 first, then 478, etc.

# 

# \### How to Run Simulation:

# ```bash

# cd simulation

# make clean

# make

# ./vga\_sim

# \# Output: output.bmp (640×480 image)

# ```

# 

# \### Main Function: `vga\_write(int row, int col, const char \*txt)`

# ```c

# vga\_write(5, 10, "HELLO");  // Write "HELLO" at row 5, column 10

# ```

# 

# \*\*What it does\*\*:

# 1\. Takes each character in the text

# 2\. Checks if it's A-Z or space (reject others)

# 3\. Stores it in vga\_buffer with color

# 4\. Renders the character to framebuffer using the font

# 

# ---

# 

# \## 🖥️ Part 2: VIRTUALIZATION (Real Kernel)

# 

# \### What It Does:

# Creates a \*\*bootable operating system kernel\*\* that writes directly to the VGA hardware at address \*\*0xB8000\*\*.

# 

# \### Key Concepts You Need to Know:

# 

# \#### 1. \*\*What is a Kernel?\*\*

# A kernel is the \*\*core of an operating system\*\*. It runs with full hardware access and controls:

# \- Memory

# \- Devices (keyboard, screen, disk)

# \- Programs

# 

# Our kernel is \*\*minimal\*\*: it just displays text on screen.

# 

# \#### 2. \*\*Booting Process\*\*

# ```

# Computer Power On

# &nbsp;   ↓

# BIOS (firmware in the computer)

# &nbsp;   ↓

# GRUB Bootloader (loads our kernel)

# &nbsp;   ↓

# Our Kernel (boot.S → main.c)

# ```

# 

# \#### 3. \*\*Multiboot Header\*\* (in boot.S)

# ```asm

# .section .multiboot

# .align 4

# .long 0x1BADB002    # Magic number

# .long 0x00000003    # Flags

# .long -0x1BADB005   # Checksum

# ```

# 

# \*\*Why is this needed?\*\*

# \- GRUB bootloader looks for this "magic number" in the first 8KB of the kernel

# \- If found, GRUB knows: "This is a bootable kernel, I can load it"

# \- Without it: "error: no multiboot header found"

# 

# \*\*The numbers explained\*\*:

# \- `0x1BADB002`: Magic constant defined by Multiboot specification

# \- `0x00000003`: Flags (bit 0 = align modules, bit 1 = provide memory info)

# \- Checksum: `-(magic + flags)` must equal zero when added together

# 

# \#### 4. \*\*VGA Hardware Address: 0xB8000\*\*

# ```c

# \#define VGA\_MEMORY 0xB8000

# uint16\_t \*vga = (uint16\_t \*)VGA\_MEMORY;

# vga\[0] = 'A' | (0x0F << 8);  // White 'A' appears on screen!

# ```

# 

# \*\*Why 0xB8000?\*\*

# \- This is a \*\*physical memory address\*\* in the computer

# \- The VGA card maps its text buffer to this address

# \- When you write to 0xB8000, it \*\*instantly appears on the monitor\*\*

# \- This is called \*\*memory-mapped I/O\*\*

# 

# \*\*How it works\*\*:

# ```

# CPU writes to 0xB8000

# &nbsp;   ↓

# Memory controller routes to VGA card (not RAM!)

# &nbsp;   ↓

# VGA card reads the data

# &nbsp;   ↓

# Displays character on monitor

# ```

# 

# \#### 5. \*\*Memory Layout\*\* (linker.ld)

# ```

# Physical Memory Map:

# 0x00000000 - 0x000FFFFF  (0-1MB)    : BIOS, bootloader, reserved

# 0x00100000 - ...         (1MB+)     : Our kernel loads here

# 

# Our Kernel Layout:

# 0x00100000: .multiboot section  ← GRUB finds this first

# 0x00100010: .text section       ← Our code (functions)

# 0x00102000: .rodata section     ← Strings like "HELLO"

# 0x00103000: .data section       ← Initialized variables

# 0x00104000: .bss section        ← Stack and uninitialized data

# ```

# 

# \*\*Why start at 1MB (0x00100000)?\*\*

# \- First 1MB is reserved for:

# &nbsp; - BIOS code (0x00000-0x003FF)

# &nbsp; - Video memory (0xA0000-0xBFFFF includes 0xB8000!)

# &nbsp; - BIOS data

# \- Kernels traditionally load at 1MB to avoid conflicts

# 

# \#### 6. \*\*Boot.S Explained\*\* (Assembly Entry Point)

# ```asm

# .section .bss

# stack\_bottom:

# .skip 16384        # Reserve 16KB for stack

# stack\_top:

# 

# .section .text

# .global \_start

# \_start:

# &nbsp;   movl $stack\_top, %esp    # Set up stack pointer

# &nbsp;   call kernel\_main         # Jump to C code

# &nbsp;   cli                      # Disable interrupts

# &nbsp;   hlt                      # Halt CPU

# ```

# 

# \*\*What's happening\*\*:

# 1\. \*\*Stack\*\*: Programs need a stack for function calls and local variables. We reserve 16KB.

# 2\. \*\*%esp register\*\*: Stack pointer. Points to current top of stack.

# 3\. \*\*call kernel\_main\*\*: Jumps to our C function `kernel\_main()`

# 4\. \*\*cli/hlt\*\*: If kernel\_main returns (it shouldn't), disable interrupts and halt forever

# 

# \*\*Why assembly?\*\* 

# \- The CPU starts in a special state

# \- We need to set up the stack before C code can run

# \- C assumes a stack exists; assembly creates it

# 

# \#### 7. \*\*PCI Bus Scanning\*\* (pci.c)

# ```c

# \#define PCI\_CONFIG\_ADDRESS 0xCF8  // PCI address port

# \#define PCI\_CONFIG\_DATA    0xCFC  // PCI data port

# ```

# 

# \*\*What is PCI?\*\*

# \- \*\*Peripheral Component Interconnect\*\*: A bus that connects devices to CPU

# \- Devices: graphics card, network card, sound card, etc.

# \- Each device has an address (bus, slot, function)

# 

# \*\*How PCI scanning works\*\*:

# 1\. Write device address to port 0xCF8

# 2\. Read device info from port 0xCFC

# 3\. Check class code: 0x03 00 = VGA graphics controller

# 4\. Read BAR0 (Base Address Register 0) = memory address of device

# 

# \*\*I/O Ports explained\*\*:

# ```c

# outl(0xCF8, address);  // Send address to PCI controller

# data = inl(0xCFC);     // Read data from PCI controller

# ```

# 

# \*\*Why?\*\* Hardware uses I/O ports (like 0xCF8) for communication. The `outl` and `inl` instructions talk directly to hardware.

# 

# \*\*Our PCI scan function\*\*:

# ```c

# for (bus = 0; bus < 256; bus++) {           // Try all buses

# &nbsp;   for (slot = 0; slot < 32; slot++) {     // Try all slots

# &nbsp;       vendor\_id = pci\_read\_config(...);   // Read device ID

# &nbsp;       if (vendor\_id == 0xFFFF) continue;  // No device here

# &nbsp;       

# &nbsp;       class\_code = pci\_read\_config(...);  // Read device type

# &nbsp;       if (class\_code == 0x0300) {         // VGA controller?

# &nbsp;           bar0 = pci\_read\_config(...);    // Get framebuffer address

# &nbsp;           return SUCCESS;

# &nbsp;       }

# &nbsp;   }

# }

# ```

# 

# \#### 8. \*\*How Our Kernel Works\*\*:

# ```

# GRUB loads kernel at 0x00100000

# &nbsp;   ↓

# Finds multiboot header

# &nbsp;   ↓

# Jumps to \_start (boot.S)

# &nbsp;   ↓

# Sets up stack

# &nbsp;   ↓

# Calls kernel\_main() (main.c)

# &nbsp;   ↓

# vga\_init\_text() - prepares VGA driver

# &nbsp;   ↓

# vga\_clear() - fills screen with spaces

# &nbsp;   ↓

# vga\_write\_text() - writes colored text to 0xB8000

# &nbsp;   ↓

# pci\_scan\_vga() - scans PCI bus for VGA card

# &nbsp;   ↓

# Infinite loop (hlt) - kernel stays alive

# ```

# 

# \### How to Build and Run:

# ```bash

# cd virtualisation

# make clean

# make

# qemu-system-i386 -cdrom vga\_kernel.iso -display curses

# 

# \# To exit QEMU: Press Esc then 2

# \# Or from another terminal: pkill qemu

# ```

# 

# ---

# 

# \## 🎓 Exam Questions \& Answers

# 

# \### Q1: "Explain how VGA text mode works"

# \*\*Answer\*\*: 

# VGA text mode is a 25×80 grid stored at memory address 0xB8000. Each cell is 2 bytes: 1 byte for the ASCII character (A-Z or space in our case), and 1 byte for colors (4 bits foreground, 4 bits background). When we write to 0xB8000, the VGA hardware reads it and displays characters on screen instantly.

# 

# \### Q2: "Why do we need a multiboot header?"

# \*\*Answer\*\*: 

# The multiboot header (magic number 0x1BADB002) tells the GRUB bootloader "this is a bootable kernel." Without it, GRUB doesn't know how to load our code. The header must be in the first 8KB of the kernel file so GRUB can find it.

# 

# \### Q3: "What is the difference between simulation and virtualization?"

# \*\*Answer\*\*: 

# \- \*\*Simulation\*\*: Software that mimics VGA behavior. We create a fake screen in memory and render it to a BMP file. No real hardware involved.

# \- \*\*Virtualization\*\*: Real kernel running in QEMU. We write directly to VGA hardware (0xB8000) and the text appears on a real (virtual) screen.

# 

# \### Q4: "Explain the color encoding (2 bytes per character)"

# \*\*Answer\*\*: 

# Each character takes 16 bits (2 bytes):

# \- Bits 0-7: ASCII code (e.g., 'A' = 0x41)

# \- Bits 8-11: Foreground color (4 bits = 16 colors)

# \- Bits 12-15: Background color (4 bits = 16 colors)

# 

# Example: 0x1F41 = Blue background (1), White foreground (F), letter 'A' (41).

# 

# \### Q5: "Why does the kernel load at address 0x00100000 (1MB)?"

# \*\*Answer\*\*: 

# The first 1MB of memory is reserved:

# \- 0x00000-0x9FFFF: Usable RAM but needed by BIOS

# \- 0xA0000-0xBFFFF: Video memory (includes VGA at 0xB8000)

# \- 0xC0000-0xFFFFF: BIOS ROM

# 

# Loading at 1MB avoids conflicts with these reserved areas. This is a standard convention.

# 

# \### Q6: "How does PCI scanning work?"

# \*\*Answer\*\*: 

# PCI devices are organized by bus (0-255) and slot (0-31). We use I/O ports 0xCF8 (address) and 0xCFC (data):

# 1\. Write device address to port 0xCF8

# 2\. Read configuration data from port 0xCFC

# 3\. Check class code: 0x030000 means VGA graphics

# 4\. Read BAR0 to get the device's memory address

# 

# We scan all possible bus/slot combinations to find the VGA card.

# 

# \### Q7: "What is a framebuffer?"

# \*\*Answer\*\*: 

# A framebuffer is a region of memory that holds pixel data for the screen. In text mode, it's at 0xB8000 (25×80 characters). In graphics mode, it's a larger area where each byte/word represents a pixel's color. Writing to the framebuffer changes what appears on screen.

# 

# \### Q8: "Why do we only support A-Z and space?"

# \*\*Answer\*\*: 

# Project requirement! The specification says: "Ne gérer que les caractères A-Z et l'espace." This simplifies font rendering (we only need 27 font glyphs: A-Z + space). In the code, we check:

# ```c

# if (c != ' ' \&\& !(c >= 'A' \&\& c <= 'Z'))

# &nbsp;   return;  // Ignore other characters

# ```

# 

# \### Q9: "What is the stack and why do we need it?"

# \*\*Answer\*\*: 

# The stack is memory for:

# \- Function call return addresses

# \- Local variables

# \- Function parameters

# 

# CPUs expect a valid stack pointer (%esp register) before running C code. In boot.S, we reserve 16KB and set %esp to point to it. Without a stack, function calls would crash.

# 

# \### Q10: "How does BMP generation work?"

# \*\*Answer\*\*: 

# BMP files have a 54-byte header (file info, dimensions) followed by pixel data. Each pixel is 3 bytes (RGB). Important: BMP stores pixels bottom-to-top, so we write row 479 first, then 478, down to row 0. Our framebuffer is 640×480 (80 chars × 8 pixels wide, 25 chars × 8 pixels tall).

# 

# \### Q11: "What is memory-mapped I/O?"

# \*\*Answer\*\*: 

# Memory-mapped I/O means hardware devices appear as memory addresses. Writing to 0xB8000 doesn't store data in RAM—it sends data to the VGA card. The memory controller routes accesses to 0xB8000-0xB8FFF to the VGA hardware instead of RAM. This allows simple programming: `\*(uint16\_t\*)0xB8000 = 'A'` displays 'A' on screen.

# 

# \### Q12: "Why use assembly (boot.S) instead of just C?"

# \*\*Answer\*\*: 

# When the CPU starts, it's in a raw state:

# \- No stack set up

# \- Registers undefined

# \- C code assumes a working environment

# 

# Assembly lets us:

# 1\. Create the multiboot header (specific binary format)

# 2\. Set up the stack pointer

# 3\. Initialize the environment before C code runs

# 

# Once the stack is ready, we can safely jump to C (`call kernel\_main`).

# 

# \### Q13: "What happens if we remove the multiboot header?"

# \*\*Answer\*\*: 

# GRUB won't recognize the kernel and shows: "error: no multiboot header found". The bootloader needs this header to know:

# \- This is a kernel (not random data)

# \- Where to load it in memory

# \- What information to provide (memory map, etc.)

# 

# \### Q14: "Why 8×8 pixel fonts?"

# \*\*Answer\*\*: 

# 8×8 is a standard character size:

# \- VGA text mode traditionally uses 8×16 or 9×16, but 8×8 is simpler

# \- Each character fits in 8 bytes (1 byte per row)

# \- With 80 columns: 80 × 8 = 640 pixels wide (standard VGA resolution)

# \- With 25 rows: 25 × 8 = 200 pixels (we use 480 for framebuffer, leaving space)

# 

# \### Q15: "What is the purpose of linker.ld?"

# \*\*Answer\*\*: 

# The linker script tells the linker how to arrange sections in memory:

# \- Put `.multiboot` first (GRUB needs it at the start)

# \- Put `.text` (code) at 0x00100000

# \- Arrange `.data`, `.bss`, etc. in order

# 

# Without it, the linker might place sections randomly, and the multiboot header might not be in the first 8KB.

# 

# ---

# 

# \## 🐛 Common Issues \& Solutions

# 

# \### Issue 1: "error: no multiboot header found"

# \*\*Cause\*\*: Multiboot header not in first 8KB of kernel file.

# \*\*Solution\*\*: 

# \- Check linker.ld: `.multiboot` section must be first

# \- Verify with: `readelf -l vga\_kernel.bin` (multiboot should be in first LOAD segment)

# 

# \### Issue 2: Blank screen in QEMU

# \*\*Cause\*\*: VGA memory not being written correctly.

# \*\*Solution\*\*: 

# \- Verify address: `#define VGA\_MEMORY 0xB8000`

# \- Test with simple code: `\*(uint16\_t\*)0xB8000 = 0x0F41;` (white 'A')

# 

# \### Issue 3: Can't exit QEMU curses mode

# \*\*Solution\*\*: 

# \- Press `Esc` then `2`

# \- Or `Ctrl+A` then `X`

# \- Or `pkill qemu` from another terminal

# 

# \### Issue 4: "gtk initialization failed"

# \*\*Cause\*\*: No graphical display on server.

# \*\*Solution\*\*: Use `-display curses` for text-based display

# 

# ---

# 

# \## 📝 Quick Reference

# 

# \### VGA Text Mode Specifications:

# \- \*\*Resolution\*\*: 25 rows × 80 columns

# \- \*\*Memory Address\*\*: 0xB8000

# \- \*\*Character Size\*\*: 2 bytes (ASCII + color)

# \- \*\*Color Format\*\*: 4 bits background + 4 bits foreground

# \- \*\*Supported Characters\*\*: A-Z and space

# 

# \### Build Commands:

# ```bash

# \# Simulation

# cd simulation \&\& make \&\& ./vga\_sim

# 

# \# Virtualization

# cd virtualisation \&\& make \&\& qemu-system-i386 -cdrom vga\_kernel.iso -display curses

# ```

# 

# \### Memory Map:

# ```

# 0x00000000: BIOS/Reserved

# 0x000B8000: VGA Text Buffer (25×80×2 = 4000 bytes)

# 0x00100000: Kernel Start (our code loads here)

# ```

# 

# ---

# 

# \## ✅ Project Checklist

# 

# \### Simulation Part:

# \- \[x] VGA buffer simulation (25×80)

# \- \[x] Character encoding (2 bytes: ASCII + color)

# \- \[x] Font rendering (8×8 pixels, A-Z + space)

# \- \[x] Framebuffer (640×480 RGB)

# \- \[x] BMP file generation

# \- \[x] Function `vga\_write(row, col, txt)`

# 

# \### Virtualization Part:

# \- \[x] Multiboot header (boot.S)

# \- \[x] Kernel entry point and stack setup

# \- \[x] VGA hardware driver (0xB8000)

# \- \[x] PCI bus scanning (I/O ports 0xCF8/0xCFC)

# \- \[x] Text display with colors

# \- \[x] Bootable ISO creation

# \- \[x] QEMU/Bochs compatible

# 

# ---

# 

# \## 📖 Additional Resources

# 

# \### Understanding OS Concepts:

# \- \*\*Multiboot Specification\*\*: https://www.gnu.org/software/grub/manual/multiboot/multiboot.html

# \- \*\*OSDev Wiki\*\*: https://wiki.osdev.org/ (great resource for kernel development)

# \- \*\*VGA Text Mode\*\*: https://wiki.osdev.org/VGA\_Hardware#Text\_mode

# 

# \### Tools Documentation:

# \- \*\*QEMU\*\*: https://www.qemu.org/docs/master/

# \- \*\*GCC\*\*: https://gcc.gnu.org/onlinedocs/

# \- \*\*LD (Linker)\*\*: https://ftp.gnu.org/old-gnu/Manuals/ld-2.9.1/html\_mono/ld.html

# 

# ---

# 

# \## 🎯 Summary

# 

# This project demonstrates:

# 1\. \*\*How computers display text\*\* (VGA text mode at 0xB8000)

# 2\. \*\*How operating systems boot\*\* (multiboot, GRUB, kernel)

# 3\. \*\*How hardware communication works\*\* (memory-mapped I/O, PCI)

# 4\. \*\*Low-level programming\*\* (assembly, direct memory access)

# 

# \*\*Both parts work\*\*: 

# \- Simulation creates BMP images

# \- Virtualization runs a real kernel in QEMU

# 

# \*\*Key achievement\*\*: We built an OS kernel from scratch that controls hardware!

# 

# ---

# 

# \*Good luck with your exam! 🚀\*

