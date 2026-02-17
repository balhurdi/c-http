# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -std=c11 -g

# Directories
SRC_DIR = src
BUILD_DIR = build

# Output binary
TARGET = $(BUILD_DIR)/server

# Find all source files
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Generate object file names in build directory
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

# Default target
all: $(BUILD_DIR) $(TARGET)

# Link objects into final executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compile each .c into .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Create build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Run the program
run: all
	./$(TARGET)

# Clean build files
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean run
