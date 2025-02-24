# Compiler
CC = zig cc

# Target Architecture
TARGET_ARCH = x86_64-linux-musl 

# Compiler flags
CFLAGS = -Wall -Wextra -target $(TARGET_ARCH) 

# Linker flags
LDFLAGS = -target $(TARGET_ARCH)

# Directories
SRC_DIR = ./src
BUILD_DIR = ./build
BIN_DIR = ./bin

# Source file
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# Executable
TARGET = $(BIN_DIR)/test

# Build target
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Build object file
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create build directory
$(BUILD_DIR):
	mkdir -p $@

# Create bin directory
$(BIN_DIR):
	mkdir -p $@

# Main target
all: $(BIN_DIR) $(TARGET)

# Clean up
clean:
	rm -r $(BIN_DIR) $(BUILD_DIR)

run:
	qemu-x86_64 ./bin/test

.PHONY: all clean run
