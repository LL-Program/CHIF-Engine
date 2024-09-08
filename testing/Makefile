# Compiler and flags
CC = gcc
CFLAGS = -Wall -I./textures -I. -I/usr/include/GL
LDFLAGS = -lGL -lGLU -lglut

# Target executable
TARGET = engine

# Source files
SRCS = src/main.c

# Object files
OBJS = $(SRCS:.c=.o)

# Rule to build the target
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

# Rule to compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule to remove generated files
clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: all clean
