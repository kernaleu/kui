#include <stdio.h>
#include <signal.h>
#include <termios.h>
#include <sys/ioctl.h>

#include "kui.h"

#define CSI "\e["

/* Save the default terminal atributes to restore them later */
static struct termios old_attr;

/* Regular puts just with no '\n' */
void putsnn(const char *str)
{
    fputs(str, stdout);
}

/* Moves the cursor to the given position */
void cursmv(int row, int col)
{
    printf(CSI"%u;%uH", row + 1, col + 1);
}

/* Moves the cursot to given dirrection via given amount */
void cursgo(int dirrection, int amount)
{
    char c[2];
    if (dirrection % 2 != 0) {
        switch (dirrection) {
            case NE:
                c[0] = 'A';
                c[1] = 'C';
                break;
            case SE:
                c[0] = 'B';
                c[1] = 'C';
                break;
            case SW:
                c[0] = 'B';
                c[1] = 'D';
                break;
            case NW:
                c[0] = 'A';
                c[1] = 'D';
        }
        printf(CSI"%d%c"CSI"%d%c", amount, c[0], amount, c[1]);
    }
    else {
        switch (dirrection) {
            case N:
                c[0] = 'A';
                break;
            case E:
                c[0] = 'C';
                break;
            case S:
                c[0] = 'B';
                break;
            case W:
                c[0] = 'D';
        }
        printf(CSI"%d%c", amount, c[0]);
    }

}

static void termflag(int action, tcflag_t lflag)
{
    struct termios tmp_attr;
    tcgetattr(0, &tmp_attr);

    if (action == 1) tmp_attr.c_lflag &= lflag;
    else tmp_attr.c_lflag &= ~lflag;

    tcsetattr(0, TCSAFLUSH, &tmp_attr);

}

void echo(int mode)
{
    termflag(mode, ECHO);
}

void cbreak(int mode)
{
    termflag(mode, ICANON);
}

/*
 * void cursvis(int visibility); (VT320<)
 *   0. Hides the cursor
 *   1. Shows the cursor
 */
void cursvis(int visibility) {
    if (visibility == 0) putsnn(CSI"?25l");
    else putsnn(CSI"?25h");
}

/* Gets current cursor position */
void curspos(int *row, int *col)
{
    struct termios cur_attr, tmp_attr;
    tcgetattr(0, &cur_attr);
    tmp_attr = cur_attr;

    tmp_attr.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(0, TCSAFLUSH, &tmp_attr);

    putsnn(CSI"6n");
    scanf(CSI"%d;%dR", row, col);

    *row -= 1;
    *col -= 1;

    tcsetattr(0, TCSAFLUSH, &cur_attr);
}

/*
 * void curssr(int action);
 *   0. Saves the cursor position/state
 *   1. Restores the cursor position/state
 */
void curssr(int action)
{
    if (action == 0) putsnn(CSI"s");
    else putsnn(CSI"u");
}

/* Clears the entire screen */
void clear()
{
    putsnn(CSI"2J");
}

/* Clears from the cursor to the end of line */
void clrtoeol()
{
    putsnn(CSI"K");
}

/* Clear from the cursor to the end of screen */
void clrtoeos()
{
    putsnn(CSI"J");
}

/* Clear entite line containing the cursor */
void clrline()
{
    putsnn(CSI"2J");
}

/* Updates terminal size gets called on SIGWINCH */
static void termsize()
{
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    ROWS = w.ws_row;
    COLS = w.ws_col;
}

/* Initializes kui and saves terminal attributes */
void initkui()
{
    tcgetattr(0, &old_attr);
    signal(SIGWINCH, termsize);
    termsize();
    cursvis(0);
    echo(0);
    putsnn(CSI"?1049h");
    cursmv(0, 0);
    clear();
}

/* Ends the kui and restores terminal attributes */
void endkui()
{
    clear();
    cursvis(1);
    signal(SIGWINCH, SIG_IGN);
    tcsetattr(0, TCSAFLUSH, &old_attr);
    putchar('\n');
    putsnn(CSI"?1049l");
}


