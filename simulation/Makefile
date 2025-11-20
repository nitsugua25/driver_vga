# Compiler
CC = gcc
CFLAGS = -Wall -Wextra -O2 -Iinclude

# Source files
SRC = src/main.c src/vga.c src/font.c src/bmp.c

# Object files
OBJ = $(SRC:.c=.o)

# Executable
TARGET = vga_sim

# Default target
all: $(TARGET)

# Link object files
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

# Compile C files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean
clean:
	rm -f $(OBJ) $(TARGET) output.bmp

# Phony targets
.PHONY: all clean
