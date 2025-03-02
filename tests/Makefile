CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lm

# Source files from the original project
SRC_DIR = ../clab
SRC_FILES = $(SRC_DIR)/io.c $(SRC_DIR)/process.c $(SRC_DIR)/dependent.c $(SRC_DIR)/display.c $(SRC_DIR)/stack.c

# Test files
TEST_FILES = test_runner.c test_io.c test_process.c test_dependent.c test_display.c test_integration.c

# Object files
OBJ_FILES = $(SRC_FILES:.c=.o) $(TEST_FILES:.c=.o)

# Executable name
EXECUTABLE = test_runner

# Input files
INPUT_FILES = test_input.txt test_arithmetic.txt test_functions.txt test_dependencies.txt test_navigation.txt

# Default target
all: $(EXECUTABLE)

# Link the executable
$(EXECUTABLE): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile source files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(EXECUTABLE) $(TEST_FILES:.c=.o) *_output.txt

# Run tests with the default input file
run: $(EXECUTABLE)
	./$(EXECUTABLE) test_input.txt test_output.txt

# Run specific test files
test_arithmetic: $(EXECUTABLE)
	./$(EXECUTABLE) test_arithmetic.txt test_arithmetic_output.txt

test_functions: $(EXECUTABLE)
	./$(EXECUTABLE) test_functions.txt test_functions_output.txt

test_dependencies: $(EXECUTABLE)
	./$(EXECUTABLE) test_dependencies.txt test_dependencies_output.txt

test_navigation: $(EXECUTABLE)
	./$(EXECUTABLE) test_navigation.txt test_navigation_output.txt

# Run all tests
test_all: $(EXECUTABLE)
	@echo "===== Running all tests with all input files ====="
	@echo "===== START OF ALL TESTS =====" > test_output.txt
	@echo "\n===== Running tests with test_input.txt =====" >> test_output.txt
	./$(EXECUTABLE) test_input.txt temp_output.txt
	@cat temp_output.txt >> test_output.txt
	
	@echo "\n\n===== Running tests with test_arithmetic.txt =====" >> test_output.txt
	./$(EXECUTABLE) test_arithmetic.txt temp_output.txt
	@cat temp_output.txt >> test_output.txt
	
	@echo "\n\n===== Running tests with test_functions.txt =====" >> test_output.txt
	./$(EXECUTABLE) test_functions.txt temp_output.txt
	@cat temp_output.txt >> test_output.txt
	
	@echo "\n\n===== Running tests with test_dependencies.txt =====" >> test_output.txt
	./$(EXECUTABLE) test_dependencies.txt temp_output.txt
	@cat temp_output.txt >> test_output.txt
	
	@echo "\n\n===== Running tests with test_navigation.txt =====" >> test_output.txt
	./$(EXECUTABLE) test_navigation.txt temp_output.txt
	@cat temp_output.txt >> test_output.txt
	
	@echo "\n===== END OF ALL TESTS =====" >> test_output.txt
	@echo "All tests completed. Results saved to test_output.txt"
	@rm -f temp_output.txt
	
# Create a sample test input file
test_input.txt:
	@echo "Creating sample test input file..."
	@echo "A1=10" > test_input.txt
	@echo "B1=20" >> test_input.txt
	@echo "C1=A1+B1" >> test_input.txt
	@echo "D1=SUM(A1:C1)" >> test_input.txt
	@echo "q" >> test_input.txt

.PHONY: all clean run test_arithmetic test_functions test_dependencies test_navigation test_all 