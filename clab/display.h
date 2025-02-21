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

    #define cellwidth 8
    #define displayr 10
    #define displayc 10

#endif