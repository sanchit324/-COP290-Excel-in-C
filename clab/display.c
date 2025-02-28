#include <stdio.h>
#include <limits.h>  // Add this for INT_MIN
#include "init.h"
#include "display.h"
#include "io.h"  // Add this to get access to output_enabled

int curr_org_r = 1;
int curr_org_c = 1;
int cellwidth = 8;  // Default cell width
int displayr = 10;  // Default rows in display
int displayc = 10;  // Default columns in display

#define DEFAULT_SCROLL 10  // Default scroll amount in any direction

void int_to_alpha(int x, char* alpha) {
    if (x > MAXCOL) return;
    int i = MAXCOLWIDTH - 1;
    alpha[i + 1] = '\0';

    while (x != 0) {
        alpha[i] = (char)('A' - 1 + ((x % 26) == 0 ? 26 : (x % 26)));
        x -= ((x % 26) == 0 ? 26 : (x % 26));
        x /= 26;
        i--;
    }
    
    while (i >= 0) {
        alpha[i] = ' ';
        i--;
    }
}

void display_sheet() {
    // If output is disabled, return immediately
    if (!output_enabled) {
        return;
    }

    int row_start = curr_org_r - 1;
    int col_start = curr_org_c - 1;

    if (row_start >= MAXROW || col_start >= MAXCOL || row_start < 0 || col_start < 0) {
        printf("Error in Index\n");
        return;
    }

    printf("%*s", cellwidth, "");
    for (int i = 0; col_start + i < MAXCOL && i < displayc; i++) {
        char temp[MAXCOLWIDTH + 1] = "   ";
        int_to_alpha(col_start + i + 1, temp);
        printf("%*s", cellwidth, temp);
    }
    printf("\n");

    for (int j = 0; row_start + j < MAXROW && j < displayr; j++) {
        printf("%-*d", cellwidth, row_start + j + 1);
        for (int i = 0; col_start + i < MAXCOL && i < displayc; i++) {
            int value = sheet[row_start + j][col_start + i];
            if (value == INT_MIN || value == ERROR_VALUE) {  // Check for both INT_MIN and ERROR_VALUE
                printf("%*s", cellwidth, "ERR");
            } else {
                printf("%*d", cellwidth, value);
            }
        }
        printf("\n");
    }
}

/* Still need to add safety features to these i.e. give error when (curr_org_x,curr_org_c) is inappropriate */

void set_org(int r, int c) {
    if (!output_enabled) return;
    // Ensure the new origin stays within bounds
    curr_org_r = (r < 1) ? 1 : 
                 (r > MAXROW - displayr + 1) ? MAXROW - displayr + 1 : r;
    
    curr_org_c = (c < 1) ? 1 : 
                 (c > MAXCOL - displayc + 1) ? MAXCOL - displayc + 1 : c;
}

void scroll(int dr, int dc) {
    if (!output_enabled) return;
    int new_row = curr_org_r + dr;
    int new_col = curr_org_c + dc;
    
    // Ensure we stay within bounds
    curr_org_r = (new_row < 1) ? 1 : 
                 (new_row > MAXROW - displayr + 1) ? MAXROW - displayr + 1 : new_row;
    
    curr_org_c = (new_col < 1) ? 1 : 
                 (new_col > MAXCOL - displayc + 1) ? MAXCOL - displayc + 1 : new_col;
}

void w() {
    if (!output_enabled) return;
    // Try to scroll up by DEFAULT_SCROLL, but don't go below 1
    int new_row = curr_org_r - DEFAULT_SCROLL;
    curr_org_r = (new_row < 1) ? 1 : new_row;
}

void a() {
    if (!output_enabled) return;
    // Try to scroll left by DEFAULT_SCROLL, but don't go below 1
    int new_col = curr_org_c - DEFAULT_SCROLL;
    curr_org_c = (new_col < 1) ? 1 : new_col;
}

void s() {
    if (!output_enabled) return;
    // Try to scroll down by DEFAULT_SCROLL, but don't go beyond MAXROW
    int new_row = curr_org_r + DEFAULT_SCROLL;
    curr_org_r = (new_row > MAXROW - displayr + 1) ? MAXROW - displayr + 1 : new_row;
}

void d() {
    if (!output_enabled) return;
    // Try to scroll right by DEFAULT_SCROLL, but don't go beyond MAXCOL
    int new_col = curr_org_c + DEFAULT_SCROLL;
    curr_org_c = (new_col > MAXCOL - displayc + 1) ? MAXCOL - displayc + 1 : new_col;
}
