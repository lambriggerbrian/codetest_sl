// sl.cpp created for OpenDrives code test
// Brian Lambrigger <lambrigger.brian@gmail.com>
// Original copyright below

/*========================================
 *    sl.c: SL version 5.03
 *        Copyright 1993,1998,2014-2015
 *                  Toyoda Masashi
 *                  (mtoyoda@acm.org)
 *        Last Modified: 2014/06/03
 *========================================
 */
/* sl version 5.03 : Fix some more compiler warnings.                        */
/*                                              by Ryan Jacobs    2015/01/19 */
/* sl version 5.02 : Fix compiler warnings.                                  */
/*                                              by Jeff Schwab    2014/06/03 */
/* sl version 5.01 : removed cursor and handling of IO                       */
/*                                              by Chris Seymour  2014/01/03 */
/* sl version 5.00 : add -c option                                           */
/*                                              by Toyoda Masashi 2013/05/05 */
/* sl version 4.00 : add C51, usleep(40000)                                  */
/*                                              by Toyoda Masashi 2002/12/31 */
/* sl version 3.03 : add usleep(20000)                                       */
/*                                              by Toyoda Masashi 1998/07/22 */
/* sl version 3.02 : D51 flies! Change options.                              */
/*                                              by Toyoda Masashi 1993/01/19 */
/* sl version 3.01 : Wheel turns smoother                                    */
/*                                              by Toyoda Masashi 1992/12/25 */
/* sl version 3.00 : Add d(D51) option                                       */
/*                                              by Toyoda Masashi 1992/12/24 */
/* sl version 2.02 : Bug fixed.(dust remains in screen)                      */
/*                                              by Toyoda Masashi 1992/12/17 */
/* sl version 2.01 : Smoke run and disappear.                                */
/*                   Change '-a' to accident option.                         */
/*                                              by Toyoda Masashi 1992/12/16 */
/* sl version 2.00 : Add a(all),l(long),F(Fly!) options.                     */
/*                                              by Toyoda Masashi 1992/12/15 */
/* sl version 1.02 : Add turning wheel.                                      */
/*                                              by Toyoda Masashi 1992/12/14 */
/* sl version 1.01 : Add more complex smoke.                                 */
/*                                              by Toyoda Masashi 1992/12/14 */
/* sl version 1.00 : SL runs vomiting out smoke.                             */
/*                                              by Toyoda Masashi 1992/12/11 */

#include <curses.h>
#include <signal.h>
#include <unistd.h>
#include <cassert>
#include <stdlib.h>
#include <time.h>
#include <string>
#include "sl.h"
#include "vehicles.h"

int add_uss(int x);
int add_mx(int x);
void add_smoke(int y, int x);
void add_man(int y, int x);
int add_C51(int x);
int add_D51(int x);
int add_sl(int x);
void nselect(int n);
void chooserandom();
void handleopts(int argc, char* argv[]);
int my_mvaddstr(int y, int x, char *str);


#define VALIDOPTS ":aFlcxun:m:"

typedef int (*drawfunc)(int);

int RANDOM        = 0;
int ACCIDENT      = 0;
int LOGO          = 0;
int FLY           = 0;
int C51           = 0;
int CARS          = 1;
int MX            = 0;
int USS           = 0;
drawfunc SELECTED = add_D51;

#define NUMFUNCTIONS 5
const drawfunc FUNCTIONS[] = { add_sl, add_D51, add_C51, add_mx, add_uss }; 

int my_mvaddstr(int y, int x, char *str)
{
    for ( ; x < 0; ++x, ++str)
        if (*str == '\0')  return ERR;
    for ( ; *str != '\0'; ++str, ++x)
        if (mvaddch(y, x, *str) == ERR)  return ERR;
    return OK;
}

void nselect(int n) {
    int selection = n % NUMFUNCTIONS; 
    extern drawfunc SELECTED; 
    SELECTED = FUNCTIONS[selection]; 
}

void chooserandom() {
    srand(time(0));
    extern int RANDOM, CARS, FLY, ACCIDENT;
    RANDOM = 1; 
    CARS = rand() % 5; 
    FLY = rand() % 2;
    ACCIDENT = rand() % 2; 
    int selection = rand() % NUMFUNCTIONS; 
    nselect(selection);
}

void handleopts(int argc, char* argv[]) {
    if (argc == 1) chooserandom(); 
    extern int ACCIDENT, LOGO, FLY, C51, CARS, MX;
    int opt;
    char* ptr; 
    while ((opt = getopt(argc, argv, VALIDOPTS)) != -1) {
        switch (opt) {
            case 'a': ACCIDENT = 1; break;
            case 'F': FLY      = 1; break;
            case 'l': LOGO     = 1; break;
            case 'c': C51      = 1; break;
            case 'x': MX       = 1; break; 
            case 'u': USS      = 1; break;
            case 'n': 
                assert(optarg != NULL); 
                nselect(strtol(optarg, &ptr, 10)); 
                break; 
            case 'm': 
                assert(optarg != NULL); 
                CARS = strtol(optarg, &ptr, 10); 
                break;
            case ':': break; 
            case '?': break; 
            default:                break;
        }
    }
}

int main(int argc, char *argv[])
{
    int x, i;
    handleopts(argc, argv);
    initscr();
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);
    leaveok(stdscr, TRUE);
    scrollok(stdscr, FALSE);
    
    for (x = COLS - 1; ; --x) {
        if (RANDOM == 1) {
            if (SELECTED(x) == ERR) break;
        } 
        else if (LOGO == 1) {
            if (add_sl(x) == ERR) break;
        }
        else if (C51 == 1) {
            if (add_C51(x) == ERR) break;
        }
        else if (MX == 1) {
            if (add_mx(x) == ERR) break;
        } 
        else if (USS == 1) {
            if (add_uss(x) == ERR) break;
        } 
        else if (ACCIDENT == 1 || CARS != 1) {
            if (add_D51(x) == ERR) break;
        }
        else {
            if (SELECTED(x) == ERR) break;
        }
        getch();
        refresh();
        usleep(40000);
    }
    mvcur(0, COLS - 1, LINES - 1, 0);
    endwin();

    return 0;
}

int add_sl(int x)
{
    static char *sl[LOGOPATTERNS][LOGOHEIGHT + 1]
        = {{LOGO1, LOGO2, LOGO3, LOGO4, LWHL11, LWHL12, DELLN},
           {LOGO1, LOGO2, LOGO3, LOGO4, LWHL21, LWHL22, DELLN},
           {LOGO1, LOGO2, LOGO3, LOGO4, LWHL31, LWHL32, DELLN},
           {LOGO1, LOGO2, LOGO3, LOGO4, LWHL41, LWHL42, DELLN},
           {LOGO1, LOGO2, LOGO3, LOGO4, LWHL51, LWHL52, DELLN},
           {LOGO1, LOGO2, LOGO3, LOGO4, LWHL61, LWHL62, DELLN}};

    static char *coal[LOGOHEIGHT + 1]
        = {LCOAL1, LCOAL2, LCOAL3, LCOAL4, LCOAL5, LCOAL6, DELLN};

    static char *car[LOGOHEIGHT + 1]
        = {LCAR1, LCAR2, LCAR3, LCAR4, LCAR5, LCAR6, DELLN};

    int i, j, y, dy = 0;
    int totallength = LOGOLENGTH + (CARS*LCARLENGTH);
    if (x < - totallength)  return ERR;
    y = LINES / 2 - 3;

    if (FLY == 1) {
        y = (x / 6) + LINES - (COLS / 6) - LOGOHEIGHT;
        dy = 2; 
    }
    for (i = 0; i <= LOGOHEIGHT; ++i) {
        int pattern = (LOGOLENGTH + x) / 3 % LOGOPATTERNS;
        if (pattern >= 0) {
            my_mvaddstr(y + i, x, sl[pattern][i]);
            if (ACCIDENT == 1) add_man(y + 1, x + 14); 
        } 
        my_mvaddstr(y + i + dy, x + 21, coal[i]);
        int x_offset = x + 42;
        int y_offset = y + i + (2*dy); 
        for (j = 0; j < CARS; ++j) {
            my_mvaddstr(y_offset + (j*dy), x_offset + (j*LCARLENGTH), car[i]);
        }
        if (ACCIDENT == 1) {
            int x_offset = x + 45;
            int y_offset = y + 1 + (2*dy); 
            for (j = 0; j < CARS; ++j) {
                add_man(y_offset + (j*dy), x_offset + (j*LCARLENGTH));  
                add_man(y_offset + (j*dy), x_offset + 11 + (j*LCARLENGTH));
            } 
        } 
    }
    add_smoke(y - 1, x + LOGOFUNNEL);
    return OK;
}


int add_D51(int x)
{
    static char *d51[D51PATTERNS][D51HEIGHT + 1]
        = {{D51STR1, D51STR2, D51STR3, D51STR4, D51STR5, D51STR6, D51STR7,
            D51WHL11, D51WHL12, D51WHL13, D51DEL},
           {D51STR1, D51STR2, D51STR3, D51STR4, D51STR5, D51STR6, D51STR7,
            D51WHL21, D51WHL22, D51WHL23, D51DEL},
           {D51STR1, D51STR2, D51STR3, D51STR4, D51STR5, D51STR6, D51STR7,
            D51WHL31, D51WHL32, D51WHL33, D51DEL},
           {D51STR1, D51STR2, D51STR3, D51STR4, D51STR5, D51STR6, D51STR7,
            D51WHL41, D51WHL42, D51WHL43, D51DEL},
           {D51STR1, D51STR2, D51STR3, D51STR4, D51STR5, D51STR6, D51STR7,
            D51WHL51, D51WHL52, D51WHL53, D51DEL},
           {D51STR1, D51STR2, D51STR3, D51STR4, D51STR5, D51STR6, D51STR7,
            D51WHL61, D51WHL62, D51WHL63, D51DEL}};
    static char *coal[D51HEIGHT + 1]
        = {COAL01, COAL02, COAL03, COAL04, COAL05,
           COAL06, COAL07, COAL08, COAL09, COAL10, COALDEL};

    int y, i, j, dy = 0;
    int totallength = D51LENGTH + (CARS-1)*COALLENGTH;
    if (x < - totallength)  return ERR;
    y = LINES / 2 - 5;

    if (FLY == 1) {
        y = (x / 7) + LINES - (COLS / 7) - (D51HEIGHT + CARS);
        dy = 1;
    }
    for (i = 0; i <= D51HEIGHT; ++i) {
        int pattern = (D51LENGTH + x) % D51PATTERNS;
        // Make sure we use a valid pattern 
        if (pattern >= 0) {
            my_mvaddstr(y + i, x, d51[pattern][i]);
        }
        // Using locomotive offset, print the coal car CARS times 
        int x_offset = x + 52; 
        int y_offset = y + i + dy; 
        for (j = 0; j < CARS; ++j) {
            my_mvaddstr(y_offset + (j*dy), x_offset + (j*COALLENGTH), coal[i]);
        } 
    }
    if (ACCIDENT == 1) {
        add_man(y + 2, x + 43);
        add_man(y + 2, x + 47);
    }
    add_smoke(y - 1, x + D51FUNNEL);
    return OK;
}

int add_C51(int x)
{
    static char *c51[C51PATTERNS][C51HEIGHT + 1]
        = {{C51STR1, C51STR2, C51STR3, C51STR4, C51STR5, C51STR6, C51STR7,
            C51WH11, C51WH12, C51WH13, C51WH14, C51DEL},
           {C51STR1, C51STR2, C51STR3, C51STR4, C51STR5, C51STR6, C51STR7,
            C51WH21, C51WH22, C51WH23, C51WH24, C51DEL},
           {C51STR1, C51STR2, C51STR3, C51STR4, C51STR5, C51STR6, C51STR7,
            C51WH31, C51WH32, C51WH33, C51WH34, C51DEL},
           {C51STR1, C51STR2, C51STR3, C51STR4, C51STR5, C51STR6, C51STR7,
            C51WH41, C51WH42, C51WH43, C51WH44, C51DEL},
           {C51STR1, C51STR2, C51STR3, C51STR4, C51STR5, C51STR6, C51STR7,
            C51WH51, C51WH52, C51WH53, C51WH54, C51DEL},
           {C51STR1, C51STR2, C51STR3, C51STR4, C51STR5, C51STR6, C51STR7,
            C51WH61, C51WH62, C51WH63, C51WH64, C51DEL}};
    static char *coal[C51HEIGHT + 1]
        = {COALDEL, COAL01, COAL02, COAL03, COAL04, COAL05,
           COAL06, COAL07, COAL08, COAL09, COAL10, COALDEL};

    int y, i, j, dy = 0;
    int totallength = C51LENGTH + (CARS-1)*COALLENGTH; 
    if (x < - totallength)  return ERR;
    y = LINES / 2 - 5;

    if (FLY == 1) {
        y = (x / 7) + LINES - (COLS / 7) - C51HEIGHT;
        dy = 1;
    }
    for (i = 0; i <= C51HEIGHT; ++i) {
        int pattern = (C51LENGTH + x) % C51PATTERNS;
        // Make sure we use a valid pattern 
        if (pattern >= 0) {
            my_mvaddstr(y + i, x, c51[pattern][i]);
        }
        // Print coal car CAR times 
        for (j = 0; j < CARS; ++j) {
            int x_offset = x + 55; 
            int y_offset = y + i + dy; 
            my_mvaddstr(y_offset + (j*dy), x_offset + (j*COALLENGTH), coal[i]);
        } 
    }
    if (ACCIDENT == 1) {
        add_man(y + 3, x + 45);
        add_man(y + 3, x + 49);
    }
    add_smoke(y - 1, x + C51FUNNEL);
    return OK;
}


void add_man(int y, int x)
{
    static char *man[2][2] = {{"", "(O)"}, {"Help!", "\\O/"}};
    int i;

    for (i = 0; i < 2; ++i) {
        my_mvaddstr(y + i, x, man[(LOGOLENGTH + x) / 12 % 2][i]);
    }
}


void add_smoke(int y, int x)
#define SMOKEPTNS        16
{
    static struct smokes {
        int y, x;
        int ptrn, kind;
    } S[1000];
    static int sum = 0;
    static char *Smoke[2][SMOKEPTNS]
        = {{"(   )", "(    )", "(    )", "(   )", "(  )",
            "(  )" , "( )"   , "( )"   , "()"   , "()"  ,
            "O"    , "O"     , "O"     , "O"    , "O"   ,
            " "                                          },
           {"(@@@)", "(@@@@)", "(@@@@)", "(@@@)", "(@@)",
            "(@@)" , "(@)"   , "(@)"   , "@@"   , "@@"  ,
            "@"    , "@"     , "@"     , "@"    , "@"   ,
            " "                                          }};
    static char *Eraser[SMOKEPTNS]
        =  {"     ", "      ", "      ", "     ", "    ",
            "    " , "   "   , "   "   , "  "   , "  "  ,
            " "    , " "     , " "     , " "    , " "   ,
            " "                                          };
    static int dy[SMOKEPTNS] = { 2,  1, 1, 1, 0, 0, 0, 0, 0, 0,
                                 0,  0, 0, 0, 0, 0             };
    static int dx[SMOKEPTNS] = {-2, -1, 0, 1, 1, 1, 1, 1, 2, 2,
                                 2,  2, 2, 3, 3, 3             };
    int i;

    if (x % 4 == 0) {
        for (i = 0; i < sum; ++i) {
            my_mvaddstr(S[i].y, S[i].x, Eraser[S[i].ptrn]);
            S[i].y    -= dy[S[i].ptrn];
            S[i].x    += dx[S[i].ptrn];
            S[i].ptrn += (S[i].ptrn < SMOKEPTNS - 1) ? 1 : 0;
            my_mvaddstr(S[i].y, S[i].x, Smoke[S[i].kind][S[i].ptrn]);
        }
        my_mvaddstr(y, x, Smoke[sum % 2][0]);
        S[sum].y = y;    S[sum].x = x;
        S[sum].ptrn = 0; S[sum].kind = sum % 2;
        sum ++;
    }
}

int add_mx(int x)
{
    static std::string mx[MXHEIGHT + 1] 
        = {MX01, MX02, MX03, MX04, MX05, MX06, MX07, MX08, MX09,
           MX10, MX11, MX12, MX13, MX14, MX15, MX16, MX17, MX18,
           MX19, MX20, MX21, MX22, MX23, MX24, MX25, MX26, MX27, MX28, MXDEL};

    int i, j, y, dy = 0;
    int totallength = MXLENGTH;
    if (x < - totallength)  return ERR;
    y = LINES / 2 - 3;

    if (FLY == 1) {
        y = (x / 6) + LINES - (COLS / 6) - MXHEIGHT;
    }
    for (i = 0; i <= MXHEIGHT; ++i) {
        my_mvaddstr(y + i, x, const_cast<char*>(mx[i].c_str()));
    }
    return OK;
}

int add_uss(int x)
{
    static std::string uss[USSHEIGHT + 1] 
        = {USS01, USS02, USS03, USS04, USS05, USS06, USS07, USS08, USS09,
           USS10, USS11, USS12, USS13, USSDEL};

    int i, j, y, dy = 0;
    int totallength = USSLENGTH;
    if (x < - totallength)  return ERR;
    y = LINES / 2 - 3;

    if (FLY == 1) {
        y = (x / 6) + LINES - (COLS / 6) - USSHEIGHT;
    }
    for (i = 0; i <= USSHEIGHT; ++i) {
        my_mvaddstr(y + i, x, const_cast<char*>(uss[i].c_str()));
    }
    return OK;
}
