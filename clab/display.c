#include <stdio.h>
#include "init.h"
#include "display.h"

int curr_org_r = 1;
int curr_org_c = 1;
int cellwidth = 8;  // Default cell width
int displayr = 10;  // Default rows in display
int displayc = 10;  // Default columns in display


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
            printf("%*d", cellwidth, sheet[row_start + j][col_start + i]);
        }
        printf("\n");
    }
}

/* Still need to add safety features to these i.e. give error when (curr_org_x,curr_org_c) is inappropriate */

void set_org(int r, int c){
    curr_org_r = r;
    curr_org_c = c;
    // display_sheet();
}

void scroll(int dr, int dc){
    curr_org_r += dr;
    curr_org_c += dc;
    // display_sheet();
}

void w() {
    curr_org_r -= displayr;  // Scroll up by the number of rows in the display
    // display_sheet();
}

void a() {
    curr_org_c -= displayc;  // Scroll left by the number of columns in the display
    // display_sheet();
}

void s() {
    curr_org_r += displayr;  // Scroll down by the number of rows in the display
    // display_sheet();
}

void d() {
    curr_org_c += displayc;  // Scroll right by the number of columns in the display
    // display_sheet();
}
