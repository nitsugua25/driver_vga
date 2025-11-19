CC=gcc
CFLAGS=-Wall -O2 -Iinclude

OBJS=src/vga.o src/bmp.o src/font.o src/main.o

all: sim

sim: $(OBJS)
	$(CC) $(CFLAGS) -o sim $(OBJS)

clean:
	rm -f sim $(OBJS)
