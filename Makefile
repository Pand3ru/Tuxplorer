# Makefile

# Compiler and Compiler Flags
CC=gcc
CFLAGS=-g -Wall
LIBS=-lncurses

# The build target executable
TARGET=debug_TuxPlorer

# Object files
OBJS=main.o draw.o dirFunctions.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LIBS)

main.o : main.c draw.h
	$(CC) $(CFLAGS) -c main.c

draw.o : draw.c draw.h
	$(CC) $(CFLAGS) -c draw.c
dirFunctions.o : dirFunctions.c dirFunctions.h
	$(CC) $(CFLAGS) -c dirFunctions.c

.PHONY: clean

clean:
	$(RM) $(TARGET) $(OBJS)
