#ifndef functions_h
#define functions_h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <time.h>


ALLEGRO_BITMAP *white;
ALLEGRO_BITMAP *black;
ALLEGRO_BITMAP *black_white_pawn;
ALLEGRO_BITMAP *black_red_pawn;
ALLEGRO_BITMAP *black_white_king;
ALLEGRO_BITMAP *black_red_king;
ALLEGRO_BITMAP *menupng;
ALLEGRO_BITMAP *white_won;
ALLEGRO_BITMAP *red_won;
ALLEGRO_EVENT events;
ALLEGRO_KEYBOARD_STATE keyboard;
ALLEGRO_EVENT_QUEUE *event_queue;
ALLEGRO_DISPLAY * display;
ALLEGRO_FONT *font;
ALLEGRO_TIMER *timer;

struct square{
    int occupied;
    char color[7];
    char type[7];
};
struct square board[9][9];
int comp[4];
void read_board(struct square board[][9], char turn[6], int*, int*, int*, int*, int, int, char[18]);
void save(int, struct square board[][9], char turn[6], int, int, int, int);
void move(struct square board[][9], int x1, int y1, int x2, int y2);
int pawns_between(int x1, int y1, int x2, int y2, struct square board[][9]);
int check_move1(int x1, int y1, int x2, int y2, struct square board[][9], char turn[6]);
int in_board(int x, int y);
int capture_possible (int x, int y, struct square board[][9]);
int capture_possible_all (struct square board[][9], char turn[6]);
int move_possible (struct square board[][9], char turn[6]);
void computer(struct square board[][9], int comp[], int next_capture, int obligatory_x0, int obligatory_y0);
int getlist(char[][18]);
void init();
void destroy();
void drawmenu(int);
void drawgame(int square_width, int x[2], int y[2], char turn[6], int recstate);
int calc_value(struct square board[][9], int[4]);
#endif
