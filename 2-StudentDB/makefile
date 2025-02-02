# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -g

# Target executable name
TARGET = db_manager

# Find all source files and header files
SRCS = $(wildcard *.c)
OBJ = $(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Compile source to executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

# Clean up build files
clean:
	rm -f $(OBJ) $(TARGET)
	rm -f student.db

test:
	./test.sh

# Phony targets
.PHONY: all clean test
