################################################################################
# Makefile for the Spreadsheet Application Report (report directory)
# Handles compilation of LaTeX documents
################################################################################

# LaTeX compiler
LATEX = pdflatex

# Report source and output
REPORT_SRC = report.tex
REPORT_PDF = report.pdf
ASSETS_DIR = assets

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

# Clean auxiliary files but keep PDF and assets
clean:
	@echo "Cleaning auxiliary LaTeX files (preserving assets folder)..."
	@rm -f *.aux *.log *.out *.toc *.lof *.lot *.bbl *.blg *.dvi *.fls *.fdb_latexmk *.synctex.gz
	@echo "Assets folder preserved."

# Clean all files including PDF but preserve assets
clean-all: clean
	@echo "Cleaning all LaTeX files including PDF (preserving assets folder)..."
	@rm -f $(REPORT_PDF)
	@echo "Assets folder preserved."

# Help target
help:
	@echo "Spreadsheet Application Report Makefile (report directory)"
	@echo ""
	@echo "Available targets:"
	@echo "  all (default)   - Generate the PDF report"
	@echo "  clean           - Remove auxiliary LaTeX files (preserves assets folder)"
	@echo "  clean-all       - Remove all LaTeX files including PDF (preserves assets folder)"
	@echo "  help            - Display this help message"

.PHONY: all clean clean-all help 