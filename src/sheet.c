#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "cell/cell.h"
#include "parser/parser.h"
#include "graph/graph.h"
#include "viewport/viewport.h"
#include "error/error.h"
#include "util/util.h"

#define MAX_ROWS 999
#define MAX_COLS 18278
#define VIEWPORT_SIZE 10

typedef struct {
    Cell ***grid;
    DependencyGraph *graph;
    Viewport viewport;
    int rows;
    int cols;
    ErrorCode last_error;
    double last_time;
} Spreadsheet;

// Initialize spreadsheet grid
Cell ***create_grid(int rows, int cols) {
    Cell ***grid = malloc(rows * sizeof(Cell **));
    for (int i = 0; i < rows; i++) {
        grid[i] = malloc(cols * sizeof(Cell *));
        for (int j = 0; j < cols; j++) {
            grid[i][j] = cell_create();
        }
    }
    return grid;
}

// Convert column index to letters (0 -> A, 26 -> AA, etc.)
void col_to_str(int col, char *buffer) {
    int len = 0;
    do {
        buffer[len++] = 'A' + (col % 26);
        col = col / 26 - 1;
    } while (col >= 0);
    buffer[len] = '\0';

    // Reverse the string
    for (int i = 0; i < len/2; i++) {
        char tmp = buffer[i];
        buffer[i] = buffer[len-1-i];
        buffer[len-1-i] = tmp;
    }
}

// Display current viewport
void display_spreadsheet(const Spreadsheet *sheet) {
    int start_row, end_row, start_col, end_col;
    viewport_get_visible_range(&sheet->viewport, &start_row, &end_row, &start_col, &end_col);
    
    // Print column headers
    printf("     ");
    for (int col = start_col; col <= end_col; col++) {
        char col_str[4];
        col_to_str(col, col_str);
        printf("%-6s", col_str);
    }
    printf("\n");

    // Print rows
    for (int row = start_row; row <= end_row; row++) {
        printf("%-4d", row + 1);
        for (int col = start_col; col <= end_col; col++) {
            printf("%-6d", sheet->grid[row][col]->value);
        }
        printf("\n");
    }
}

// Evaluate formula and update dependencies
ErrorCode evaluate_formula(Spreadsheet *sheet, int row, int col, const char *formula) {
    // Parse formula and get dependencies
    ASTNode *ast = parse_formula(formula);
    if (!ast) return ERR_SYNTAX;

    int dep_count = 0;
    int **deps = NULL;
    // get_dependencies(ast, &deps, &dep_count);

    // Add dependencies to graph
    for (int i = 0; i < dep_count; i++) {
        if (!util_is_valid_cell(deps[i][0], deps[i][1], sheet->rows, sheet->cols)) {
            free_ast(ast);
            free(deps);
            return ERR_INVALID_CELL;
        }
        graph_add_dependency(sheet->graph, deps[i][0], deps[i][1], row, col);
    }

    // TODO: Actual evaluation logic
    cell_set_formula(sheet->grid[row][col], formula);
    cell_set_value(sheet->grid[row][col], 0); // Temporary placeholder
    
    free_ast(ast);
    free(deps);
    return ERR_OK;
}

// Handle cell assignment command
ErrorCode handle_assignment(Spreadsheet *sheet, const char *cmd) {
    char *eq = strchr(cmd, '=');
    if (!eq) return ERR_SYNTAX;

    // Parse cell reference
    int target_row, target_col;
    if (util_parse_cellref(strndup(cmd, eq - cmd), &target_row, &target_col) != 0) {
        return ERR_INVALID_CELL;
    }

    // Validate cell coordinates
    if (!util_is_valid_cell(target_row, target_col, sheet->rows, sheet->cols)) {
        return ERR_INVALID_CELL;
    }

    // Process formula
    return evaluate_formula(sheet, target_row, target_col, eq + 1);
}

// Main command loop
void run_spreadsheet(Spreadsheet *sheet) {
    char input[256];
    clock_t start, end;
    
    while (1) {
        // Display
        display_spreadsheet(sheet);
        
        // Show prompt
        printf("[%.1f] (%s) > ", sheet->last_time, error_get_message(sheet->last_error));
        fflush(stdout);

        // Get input
        if (!fgets(input, sizeof(input), stdin)) break;
        input[strcspn(input, "\n")] = '\0';

        start = clock();
        sheet->last_error = ERR_OK;

        // Process command
        if (strcmp(input, "q") == 0) break;
        else if (strchr(input, '=')) sheet->last_error = handle_assignment(sheet, input);
        else if (strlen(input) == 1) {
            switch (input[0]) {
                case 'w': viewport_scroll_up(&sheet->viewport); break;
                case 's': viewport_scroll_down(&sheet->viewport); break;
                case 'a': viewport_scroll_left(&sheet->viewport); break;
                case 'd': viewport_scroll_right(&sheet->viewport); break;
                default: sheet->last_error = ERR_INVALID_COMMAND;
            }
        } else {
            sheet->last_error = ERR_INVALID_COMMAND;
        }

        // Calculate execution time
        end = clock();
        sheet->last_time = (double)(end - start) / CLOCKS_PER_SEC;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s ROWS COLS\n", argv[0]);
        return 1;
    }

    int rows = atoi(argv[1]);
    int cols = atoi(argv[2]);

    if (rows < 1 || rows > MAX_ROWS || cols < 1 || cols > MAX_COLS) {
        fprintf(stderr, "Invalid grid size\n");
        return 1;
    }

    Spreadsheet sheet = {
        .grid = create_grid(rows, cols),
        .graph = graph_create(rows, cols),
        .viewport = viewport_create(rows, cols),
        .rows = rows,
        .cols = cols,
        .last_error = ERR_OK,
        .last_time = 0.0
    };

    run_spreadsheet(&sheet);

    // Cleanup
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cell_destroy(sheet.grid[i][j]);
        }
        free(sheet.grid[i]);
    }
    free(sheet.grid);
    graph_destroy(sheet.graph);

    return 0;
}