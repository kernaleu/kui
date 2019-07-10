int ROWS;
int COLS;

void putsnn(const char *str);

void initkui();
void endkui();

void cbreak(int mode);
void echo(int mode);

void clear();
void clrline();
void clrtoeol();
void clrtoeos();

void cursmv(int row, int col);
void cursgo(int dirrection, int amount);
void curspos(int *row, int *col);
void cursvis(int visibility);
void curssr(int action);

#define N 0
#define NE 1
#define E 2
#define SE 3
#define S 4
#define SW 5
#define W 6
#define NW 7
