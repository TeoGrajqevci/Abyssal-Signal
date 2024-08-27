# Compiler and linker settings
CC = gcc
CFLAGS = -Wall -Ilibs
LDFLAGS = -Llibs -lraylib -lm

# Source files and object files
SRCS = main.c camera.c fog.c game.c map.c menu.c
OBJS_DIR = obj
OBJS = $(SRCS:%.c=$(OBJS_DIR)/%.o)

# Executable name
TARGET = Abyssal-Signal

# Default target
all: build

# Create object directory
$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

# Build the executable
build: $(OBJS_DIR) $(TARGET)

# Link the executable
$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

# Compile source files into object files
$(OBJS_DIR)/%.o: %.c | $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Run the executable
run: build
	./$(TARGET)

# Create a release version
release: build
	@if [ -f $(TARGET) ]; then \
		echo "Creating release archive..."; \
		tar -czf $(TARGET).tar.gz $(TARGET) libs/ resources/ shaders/ *.c *.h Makefile; \
	else \
		echo "Error: $(TARGET) not found. Please build the project first."; \
		exit 1; \
	fi

# Clean up build artifacts
clean:
	rm -f $(OBJS) $(TARGET)
	rm -rf $(OBJS_DIR)

# Phony targets
.PHONY: all build run release clean
