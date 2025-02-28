/**
 * display.h
 * Display and navigation functions for the spreadsheet
 * Handles screen output, scrolling, and viewport management
 */

#include "init.h"

#ifndef __DISPLAY_FUNCS__
    #define __DISPLAY_FUNCS__

    // Navigation and display functions
    void scroll(int dr, int dy);              // Scroll by delta row/col
    void display_sheet();                     // Display current viewport
    void int_to_alpha(int x, char* alpha);    // Convert column number to letters
    void w();                                 // Scroll up
    void a();                                 // Scroll left
    void s();                                 // Scroll down
    void d();                                 // Scroll right
    void set_org(int r, int c);              // Set viewport origin
    void disable_output();                    // Disable screen updates
    void enable_output();                     // Enable screen updates

#endif

#ifndef __DISPLAY_VALS__
    #define __DISPLAY_VALS__

    // Display configuration
    #define DEFAULT_SCROLL 10  // Number of cells to scroll by default

    // Display state variables
    extern int cellwidth;      // Width of each cell in characters
    extern int displayr;       // Number of rows in viewport
    extern int displayc;       // Number of columns in viewport
    extern int curr_org_r;     // Current viewport origin row
    extern int curr_org_c;     // Current viewport origin column

#endif
