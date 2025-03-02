################################################################################
# Master Makefile for the Spreadsheet Application Project
# This Makefile coordinates building the executable, running tests, and generating reports
################################################################################

# Define shell to use
SHELL := /bin/bash

# Define directories
CLAB_DIR := clab
TESTS_DIR := tests
REPORT_DIR := report
TARGET_DIR := target/release

# Define commands
MAKE := make
RM := rm -f
MKDIR := mkdir -p
CP := cp

# Default target - build the spreadsheet application
all: build

# Build the spreadsheet application and copy to target/release
build:
	@echo "Building spreadsheet application..."
	@$(MAKE) -C $(CLAB_DIR)
	@$(MKDIR) $(TARGET_DIR)
	@$(CP) $(CLAB_DIR)/sheet $(TARGET_DIR)/spreadsheet
	@echo "Build complete. Executable is at: $(TARGET_DIR)/spreadsheet"

# Run the application
run: build
	@echo "Running spreadsheet application..."
	@cd $(TARGET_DIR) && ./spreadsheet 10 10

# Run all tests
test:
	@echo "Running test suite..."
	@if [ -d "$(TESTS_DIR)" ]; then \
		$(MAKE) -C $(TESTS_DIR) test_all; \
	else \
		echo "Error: Tests directory not found!"; \
		exit 1; \
	fi
	@echo "Tests completed."

# Generate the PDF report from LaTeX and clean up auxiliary files
report:
	@echo "Generating PDF report from LaTeX..."
	@if [ -d "$(REPORT_DIR)" ]; then \
		$(MAKE) -C $(REPORT_DIR); \
		echo "Cleaning up auxiliary LaTeX files (preserving assets folder)..."; \
		find $(REPORT_DIR) -type f -not -name "*.pdf" -not -name "*.tex" -not -name "Makefile" -not -name "*.bib" -not -name "*.cls" -not -name "*.sty" -not -path "$(REPORT_DIR)/assets/*" -delete; \
	else \
		echo "Error: Report directory not found!"; \
		exit 1; \
	fi
	@echo "Report generation complete. PDF is at: $(REPORT_DIR)/report.pdf"

# Check memory usage
memory-check:
	@echo "Checking memory usage..."
	@if [ -f "check_memory.sh" ]; then \
		chmod +x check_memory.sh && ./check_memory.sh; \
	else \
		echo "Error: Memory check script not found!"; \
		exit 1; \
	fi

# Run memory stress test
memory-stress:
	@echo "Running memory stress test..."
	@if [ -f "memory_stress_test.sh" ]; then \
		chmod +x memory_stress_test.sh && ./memory_stress_test.sh; \
	else \
		echo "Error: Memory stress test script not found!"; \
		exit 1; \
	fi

# Check for memory leaks
memory-leaks:
	@echo "Checking for memory leaks..."
	@if [ -f "check_memory_leaks.sh" ]; then \
		chmod +x check_memory_leaks.sh && ./check_memory_leaks.sh; \
	else \
		echo "Error: Memory leak check script not found!"; \
		exit 1; \
	fi

# Run comprehensive memory test suite
memory-suite:
	@echo "Running comprehensive memory test suite..."
	@if [ -f "memory_test_suite.sh" ]; then \
		chmod +x memory_test_suite.sh && ./memory_test_suite.sh; \
	else \
		echo "Error: Memory test suite script not found!"; \
		exit 1; \
	fi

# Clean build artifacts in clab directory
clean-clab:
	@echo "Cleaning clab build artifacts..."
	@$(MAKE) -C $(CLAB_DIR) clean

# Clean test artifacts
clean-tests:
	@echo "Cleaning test artifacts..."
	@if [ -d "$(TESTS_DIR)" ]; then \
		$(MAKE) -C $(TESTS_DIR) clean; \
	fi

# Clean report artifacts but keep the PDF
clean-report:
	@echo "Cleaning report artifacts (keeping PDF and assets folder)..."
	@if [ -d "$(REPORT_DIR)" ]; then \
		find $(REPORT_DIR) -type f -not -name "*.pdf" -not -name "*.tex" -not -name "Makefile" -not -name "*.bib" -not -name "*.cls" -not -name "*.sty" -not -path "$(REPORT_DIR)/assets/*" -delete; \
	fi

# Clean all report artifacts including the PDF
clean-report-all:
	@echo "Cleaning all report artifacts (including PDF, preserving assets folder)..."
	@if [ -d "$(REPORT_DIR)" ]; then \
		find $(REPORT_DIR) -type f -not -name "*.tex" -not -name "Makefile" -not -name "*.bib" -not -name "*.cls" -not -name "*.sty" -not -path "$(REPORT_DIR)/assets/*" -delete; \
	fi

# Clean target directory
clean-target:
	@echo "Cleaning target directory..."
	@rm -rf $(TARGET_DIR)

# Clean all build artifacts
clean: clean-clab clean-tests clean-report clean-target
	@echo "All build artifacts cleaned."

# Clean everything including PDFs
clean-all: clean-clab clean-tests clean-report-all clean-target
	@echo "All artifacts cleaned."

# Build everything (application, run tests, generate report)
complete: build test report
	@echo "Complete build finished successfully!"

# Display help for all Makefiles
help-all:
	@echo "===== MASTER MAKEFILE HELP ====="
	@$(MAKE) help
	@echo ""
	@echo "===== CLAB MAKEFILE HELP ====="
	@$(MAKE) -C $(CLAB_DIR) help
	@echo ""
	@echo "===== TESTS MAKEFILE HELP ====="
	@$(MAKE) -C $(TESTS_DIR) help
	@echo ""
	@echo "===== REPORT MAKEFILE HELP ====="
	@$(MAKE) -C $(REPORT_DIR) help

# Help target
help:
	@echo "Spreadsheet Application Project Master Makefile"
	@echo ""
	@echo "Available targets:"
	@echo "  all (default)   - Build the spreadsheet application in target/release"
	@echo "  build           - Same as 'all'"
	@echo "  run             - Build and run the spreadsheet application"
	@echo "  test            - Run the test suite"
	@echo "  report          - Generate the PDF report from LaTeX and clean auxiliary files (preserves assets folder)"
	@echo "  memory-check    - Check memory usage"
	@echo "  memory-stress   - Run memory stress test"
	@echo "  memory-leaks    - Check for memory leaks"
	@echo "  memory-suite    - Run comprehensive memory test suite"
	@echo "  clean-clab      - Clean clab build artifacts"
	@echo "  clean-tests     - Clean test artifacts"
	@echo "  clean-report    - Clean report artifacts (keeping PDF and assets folder)"
	@echo "  clean-report-all - Clean all report artifacts (including PDF, preserving assets folder)"
	@echo "  clean-target    - Clean target directory"
	@echo "  clean           - Clean all build artifacts"
	@echo "  clean-all       - Clean everything including PDFs"
	@echo "  complete        - Build application, run tests, generate report"
	@echo "  help            - Display this help message"
	@echo "  help-all        - Display help for all Makefiles"

# Declare phony targets
.PHONY: all build run test report memory-check memory-stress memory-leaks memory-suite \
        clean-clab clean-tests clean-report clean-report-all clean-target clean clean-all complete help help-all 