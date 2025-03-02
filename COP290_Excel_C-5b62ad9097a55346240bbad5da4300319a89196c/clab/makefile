# Makefile for the Spreadsheet Application
# Configures compilation settings and builds the application

# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g  # Compiler flags for debugging and warnings
LDFLAGS = -lm                        # Link with math library

# Source files and headers
SRCS = init.c display.c io.c process.c stack.c dependent.c  # Source files
OBJS = $(SRCS:.c=.o)                                        # Object files
HEADERS = init.h display.h io.h process.h stack.h dependent.h  # Header files

# Output executable name
TARGET = sheet

# Default target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile source files to object files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJS) $(TARGET)

# Run the application (convenience target)
run: $(TARGET)
	./$(TARGET)