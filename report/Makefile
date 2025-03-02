################################################################################
# Makefile for the Spreadsheet Application Report (report directory)
# Handles compilation of LaTeX documents
################################################################################

# LaTeX compiler
LATEX = pdflatex

# Report source and output
REPORT_SRC = report.tex
REPORT_PDF = report.pdf

# Default target
all: $(REPORT_PDF)

# Generate PDF from LaTeX
$(REPORT_PDF): $(REPORT_SRC)
	@echo "Generating PDF report from LaTeX..."
	@if command -v $(LATEX) >/dev/null 2>&1; then \
		$(LATEX) $(REPORT_SRC) && $(LATEX) $(REPORT_SRC); \
		echo "Report generated: $(REPORT_PDF)"; \
	else \
		echo "Error: pdflatex command not found!"; \
		echo "Please install LaTeX (e.g., 'brew install --cask mactex' on macOS)"; \
		exit 1; \
	fi

# Clean auxiliary files but keep PDF
clean:
	@echo "Cleaning LaTeX auxiliary files..."
	@rm -f *.aux *.log *.out *.toc *.lof *.lot *.bbl *.blg *.nav *.snm *.vrb

# Clean all files including PDF
clean-all: clean
	@echo "Cleaning all LaTeX files including PDF..."
	@rm -f $(REPORT_PDF)

# Help target
help:
	@echo "Spreadsheet Application Report Makefile (report directory)"
	@echo ""
	@echo "Available targets:"
	@echo "  all (default)   - Generate the PDF report"
	@echo "  clean           - Remove LaTeX auxiliary files"
	@echo "  clean-all       - Remove all LaTeX files including PDF"
	@echo "  help            - Display this help message"

.PHONY: all clean clean-all help 