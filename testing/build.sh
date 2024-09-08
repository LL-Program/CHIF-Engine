#!/bin/bash

# Compiler and flags
CC=gcc
CFLAGS="-Wall -I./textures -I."
LDFLAGS="-lGL -lGLU -lglut -lm"

# Source and target
SRC=src/main.c
TARGET=src/engine

# Compile source files into object files
echo "Compiling $SRC..."
$CC $CFLAGS -c $SRC -o main.o

# Link object files to create the executable
echo "Linking object files to create $TARGET..."
$CC main.o -o $TARGET $LDFLAGS

# Clean up object files
echo "Cleaning up..."
rm -f main.o

echo "Build complete. Run ./$TARGET to execute."
./$TARGET
