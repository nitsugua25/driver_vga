CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -O2
SRC = src/main.c src/pci.c src/framebuffer.c src/vga.c
OBJ = $(SRC:.c=.o)
TARGET = vga_virt

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
