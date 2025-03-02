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

# Generate PDF from LaTeX source
$(REPORT_PDF): $(REPORT_SRC)
	@echo "Generating PDF report..."
	@if command -v $(LATEX) >/dev/null 2>&1; then \
		$(LATEX) -interaction=nonstopmode $(REPORT_SRC); \
		$(LATEX) -interaction=nonstopmode $(REPORT_SRC); \
		echo "PDF report generated successfully."; \
	else \
		echo "Error: $(LATEX) not found. Please install LaTeX."; \
		echo "For macOS, you can install it with:"; \
		echo "  brew install --cask mactex"; \
		echo "or"; \
		echo "  brew install --cask basictex"; \
		exit 1; \
	fi

# Clean auxiliary files but keep PDF
clean:
	@echo "Cleaning auxiliary LaTeX files..."
	@rm -f *.aux *.log *.out *.toc *.lof *.lot *.bbl *.blg *.dvi *.fls *.fdb_latexmk *.synctex.gz

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
	@echo "  clean           - Remove auxiliary LaTeX files"
	@echo "  clean-all       - Remove all LaTeX files including PDF"
	@echo "  help            - Display this help message"

.PHONY: all clean clean-all help 