#include "init.h"

#ifndef __DISPLAY_FUNCS__
    #define __DISPLAY_FUNCS__

    void scroll(int dr, int dy);
    void display_sheet();
    void int_to_alpha(int x, char* alpha);
    void w();
    void a();
    void s();
    void d();
    void set_org(int r, int c);

#endif

#ifndef __DISPLAY_VALS__
    #define __DISPLAY_VALS__

    #define DEFAULT_SCROLL 10  // Default scroll amount

    extern int cellwidth;  // Keep as constant if not required to change
    extern int displayr;
    extern int displayc;
    extern int curr_org_r;
    extern int curr_org_c;

#endif
