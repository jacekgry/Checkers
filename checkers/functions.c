#include "functions.h"


void read_board(struct square board[][9], char turn[6], int *komputer, int* next_capture, int* obligatory_x0, int* obligatory_y0, int continue_game, int load, char name[18]){
    int x, y, occ;
    char color[6], type[6];
    FILE *newgame = fopen("newgame.txt", "r");
    FILE *save = fopen("save.txt", "r");
    FILE *saves = fopen("saves.txt", "r");

    if(!load){
        if(!continue_game){
            fscanf(newgame, "%s", turn);
            while(fscanf(newgame,"%d%d%d%s%s", &x, &y, &occ, color, type)==5){
                board[x][y].occupied = occ;
                strcpy(board[x][y].color, color);
                strcpy(board[x][y].type, type);
            }
        }
        else{
            fscanf(save, "%d%d%d%d", komputer, next_capture, obligatory_x0, obligatory_y0);
            fscanf(save, "%s", turn);
            while(fscanf(save,"%d%d%d%s%s", &x, &y, &occ, color, type)==5){
                board[x][y].occupied = occ;
                strcpy(board[x][y].color, color);
                strcpy(board[x][y].type, type);
            }
        }

    }
    else{
        int count=1;
        char place[18];
        while(strcmp(name, place)) fgets(place, 18, saves);
        fscanf(saves, "%d%d%d%d", komputer, next_capture, obligatory_x0, obligatory_y0);
        fscanf(saves, "%s", turn);
        while(count<=64 && fscanf(saves,"%d%d%d%s%s", &x, &y, &occ, color, type)==5){
            board[x][y].occupied = occ;
            strcpy(board[x][y].color, color);
            strcpy(board[x][y].type, type);
            count++;
        }
    fclose(newgame);
    fclose(save);
    fclose(saves);
    }
}

void save(int perm, struct square board[][9], char turn[6], int komputer, int next_capture, int obligatory_x0, int obligatory_y0){
    if(!perm){
    FILE *tempsave = fopen("save.txt", "w");


    fprintf(tempsave, "%d\n%d\n%d\n%d\n", komputer, next_capture, obligatory_x0, obligatory_y0);
    fprintf(tempsave, "%s\n", turn);
    int i,j;
    for(i=1; i<=8;i++){
        for(j=1; j<=8; j++){
                fprintf(tempsave, "%d %d %d %s %s\n", i, j, board[i][j].occupied, board[i][j].color, board[i][j].type);
        }
    }
    fclose(tempsave);
    }
    else{
        FILE *saves_list = fopen("saves_list.txt", "a+");
        FILE *tempsave = fopen("save.txt", "r");
        FILE *permsave = fopen("saves.txt", "a+");
        time_t rawtime;
        struct tm * timeinfo;
        char buffer [80];
        time (&rawtime);
        timeinfo = localtime (&rawtime);
        strftime (buffer,80,"%c",timeinfo);
        fprintf(permsave, "%s\n", buffer);
        fprintf(saves_list, "%s\n", buffer);

        while(fgets(buffer, 20, tempsave)) fprintf(permsave, "%s", buffer);
        fclose(tempsave);
        fclose(saves_list);
        fclose(permsave);
    }


}



void move(struct square board[][9], int x1, int y1, int x2, int y2){
    int captured_x, captured_y, i;

    if(abs(x1-x2)!=1){
        if (x2-x1>0) captured_x = 1;
        else captured_x = -1;
        if (y2-y1>0) captured_y = 1;
        else captured_y = -1;
        for(i=1; i<abs(x1-x2); i++){

            if (board[x1+i*captured_x][y1+i*captured_y].occupied){
                board[x1+i*captured_x][y1+i*captured_y].occupied = 0;
                break;
            }
        }
    }

    board[x1][y1].occupied = 0;
    board[x2][y2].occupied = 1;
    strcpy(board[x2][y2].color, board[x1][y1].color);
    strcpy(board[x2][y2].type, board[x1][y1].type);
    if(!strcmp("white", board[x1][y1].color) && x2==1 && !capture_possible(x2, y2, board)){
        strcpy(board[x2][y2].type, "king");
    }
    else if (!strcmp("red", board[x1][y1].color) && x2==8 && !capture_possible(x2, y2, board)){
        strcpy(board[x2][y2].type, "king");
    }

}



int check_move1 (int x1, int y1, int x2, int y2, struct square board[][9], char turn[6]){ // <=7 zle,  11,12-bicie,
    int count = 0;
    if (!in_board(x1, y1)) return 0; // nie w planszy
    else if (!board[x1][y1].occupied) return 1; //wybierz pole z figur¹
    else if(strcmp(board[x1][y1].color, turn)) return 2; //nie twoja kolej
    else if(abs(x1-x2)!=abs(y1-y2)) return 3; //poruszaj siê po skosie
    else if(board[x2][y2].occupied )return 4; // chcesz zajac zajete pole


    else if (!strcmp(board[x1][y1].type, "pawn")){
        if(!strcmp(board[x1][y1].color, "white") && x2-x1==-1 && !capture_possible_all(board, turn)) return 8;
        else if(!strcmp(board[x1][y1].color, "red") && x2-x1==1 && !capture_possible_all(board,turn)) return 9;
        else if(abs(x1-x2)==2 && board[(x1+x2)/2][(y1+y2)/2].occupied && strcmp(board[(x1+x2)/2][(y1+y2)/2].color, turn)) return 11;
        else return 5;
    }
    else{ //damka
        int i, x=x1, y=y1, x_to_add, y_to_add;
        if (x2-x1>0) x_to_add = 1;
        else x_to_add = -1;
        if (y2-y1>0) y_to_add = 1;
        else y_to_add = -1;
        for(i=1; i<abs(x2-x1); i++){
            x+=x_to_add;
            y+=y_to_add;
            if (board[x][y].occupied && !strcmp(board[x][y].color, turn)) return 6;
            else if (board[x][y].occupied) count+=1;
            if (count>1) return 7;
        }
        if (count == 0 && !capture_possible_all(board, turn)) return 10;
        else if (capture_possible_all(board, turn) && count==0) {return 0; }
        else if (count == 1) return 12;
    }
}

int in_board(int x, int y){
    if(x>0 && x<9 && y>0 && y<9) return 1;
    else return 0;
}

int capture_possible (int x, int y, struct square board[][9]){
    int i, upright=1, upleft=1, downright=1, downleft=1;
    for(i=1; i<=6; i++){
        if(!strcmp(board[x][y].type, "pawn") && i>1) break;

        if((in_board(x-i,y+i) && upright && board[x-i][y+i].occupied) && ( board[x-i-1][y+i+1].occupied || !strcmp(board[x][y].color, board[x-i][y+i].color))) upright = 0;
        else if (in_board(x-i-1,y+i+1) && upright && board[x-i][y+i].occupied && !board[x-i-1][y+i+1].occupied && strcmp(board[x][y].color, board[x-i][y+i].color)) return 1;

        if((in_board(x-i,y-i) && upleft && board[x-i][y-i].occupied )  && ( board[x-i-1][y-i-1].occupied || !strcmp(board[x][y].color, board[x-i][y-i].color))) upleft = 0;
        else if (in_board(x-i-1,y-i-1) && upleft && board[x-i][y-i].occupied && !board[x-i-1][y-i-1].occupied && strcmp(board[x][y].color, board[x-i][y-i].color)) return 1;

        if((in_board(x+i,y+i) && downright && board[x+i][y+i].occupied) && (board[x+i+1][y+i+1].occupied || !strcmp(board[x][y].color, board[x+i][y+i].color))) downright = 0;
        else if (in_board(x+i+1,y+i+1) && downright && board[x+i][y+i].occupied && !board[x+i+1][y+i+1].occupied && strcmp(board[x][y].color, board[x+i][y+i].color)) return 1;

        if((in_board(x+i,y-i) && downleft && board[x+i][y-i].occupied) && (board[x+i+1][y-i-1].occupied || !strcmp(board[x][y].color, board[x+i][y-i].color))) downleft = 0;
        else if (in_board(x+i+1,y-i-1) && downleft && board[x+i][y-i].occupied && !board[x+i+1][y-i-1].occupied && strcmp(board[x][y].color, board[x+i][y-i].color)) return 1;
    }

    return 0;

}

int capture_possible_all (struct square board[][9], char turn[6])
{
    int i, j;
    for (i=1; i<=8; i++){
        for (j=1; j<=8; j++){
            if (board[i][j].occupied && !strcmp(turn, board[i][j].color) && capture_possible(i, j, board)){
                return 1;
            }
        }
    }
    return 0;
}

int move_possible (struct square board[][9], char turn[6])
{
    int x, y, i;
    for(x=1; x<=8; x++){
        for(y=1; y<=8; y++){
                for(i=-7; i<=7; i++){
                    if (check_move1(x, y, x+i, y+i, board, turn)>7 || check_move1(x, y, x+i, y-i, board, turn)>7) return 1;
                }
        }
    }
    return 0;
}



void computer(struct square board[][9], int comp[], int next_capture, int obligatory_x0, int obligatory_y0){
    int i, j, k, l, possible_moves[100][4], count = 0;
    if(next_capture){
        for(i = 1; i<=8; i++){
            for(j = 1; j<=8; j++){
                    if (check_move1(obligatory_x0, obligatory_y0, i, j, board, "red")>7) {
                        possible_moves[count][0] = obligatory_x0; possible_moves[count][1] = obligatory_y0; possible_moves[count][2] = i; possible_moves[count][3] = j;
                        count++;
                    }
            }
        }
    }


    else{
        for(i=1; i<=8; i++){
            for(j=1; j<=8; j++){
                for(k=1; k<=8; k++){
                    for(l=1; l<=8; l++){
                        if (check_move1(i, j, k, l, board, "red")>7) {
                            possible_moves[count][0] = i; possible_moves[count][1] = j; possible_moves[count][2] = k; possible_moves[count][3] = l;
                            count++;
                        }
                    }
                }
            }
        }
    }

    int max = 0;
    int value[count];
    for (i = 0; i < count; i++){
        value[i] = calc_value(board, possible_moves[i]);
        if(value[i] > value[max]) max = i;
    }
    comp[0] = possible_moves[max][0]; comp[1] = possible_moves[max][1]; comp[2] = possible_moves[max][2]; comp[3] = possible_moves[max][3];

}

int calc_value(struct square board[][9], int moves[4]){
        int i, j;
        int value=0;
        struct square copy[9][9];
        for (i = 1; i<=8; i++){
            for(j = 1; j <= 8; j++){
                copy[i][j] = board[i][j];
            }
        }

        move(copy, moves[0], moves[1], moves[2], moves[3]);
        if(capture_possible_all(copy, "red")) value+=15;
        if(capture_possible_all(copy, "white")) value-=15;
        for(i = 1; i <= 8 ; i++){
            for(j = 1; j <= 8; j++){
                if(copy[i][j].occupied && !strcmp(copy[i][j].color, "red")){
                    if(!strcmp(copy[i][j].type, "king")) value+=25;
                    else if(j>=3 && j<=6 ) value+=3;
                    else if(j==2 || j==7) value+=2;
                    else value+=1;
                }
                else{
                    if(!strcmp(copy[i][j].type, "king")) value-=25;
                    else if(j>=3 && j<=6 ) value-=3;
                    else if(j==2 || j==7) value-=2;
                    else value-=1;
                }
            }
        }
        return value;
}


int getlist(char list[][18]){
    FILE *saves_list = fopen("saves_list.txt", "r");
    int i = 0;
    while(fgets(list[i], 18, saves_list)!=NULL) i++;
    fclose(saves_list);
    return i;
    }


void init(){

    al_init();
    al_install_keyboard();
    al_init_image_addon();
    al_init_primitives_addon();
    al_init_font_addon();
    al_init_ttf_addon();

    white = al_load_bitmap("white.png");
    black = al_load_bitmap("black.png");
    black_white_pawn = al_load_bitmap("black_white_pawn.png");
    black_red_pawn = al_load_bitmap("black_red_pawn.png");
    black_white_king = al_load_bitmap("black_white_king.png");
    black_red_king = al_load_bitmap("black_red_king.png");
    menupng = al_load_bitmap("menu.png");
    white_won = al_load_bitmap("white_won.png");
    red_won = al_load_bitmap("red_won.png");

    event_queue = al_create_event_queue();

    display = al_create_display( 800, 800 );
    al_set_window_position(display, 400, 0);
    al_set_window_title( display, "Checkers" );

    font = al_load_font("timesnewarial.ttf", 20, NULL);

    timer = al_create_timer(1.0 / 60.0);
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_start_timer(timer);

    al_register_event_source(event_queue, al_get_keyboard_event_source());
}

void destroy(){

    al_destroy_bitmap(white);
    al_destroy_bitmap(black);
    al_destroy_bitmap(black_white_pawn);
    al_destroy_bitmap(black_white_king);
    al_destroy_bitmap(black_red_pawn);
    al_destroy_bitmap(black_red_king);
    al_destroy_bitmap(menupng);
    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);



}

void drawmenu(int menustate){
            al_draw_bitmap(menupng, 0, 0, NULL);
            al_draw_filled_circle(150, 50+menustate*100, 40, al_map_rgb(255, 0, 0));
            al_flip_display();
            al_clear_to_color( al_map_rgb( 0, 0, 0 ) );

}

void drawgame(int square_width, int x[2], int y[2], char turn[6], int recstate){
            int i, j;
            for(i=0; i<8; i++){
                for(j=0; j<8; j++){
                    if((i+j)%2==0){
                        al_draw_bitmap(white, j*square_width, i*square_width, ALLEGRO_FLIP_HORIZONTAL);
                    }
                    else if (!board[i+1][j+1].occupied){
                        al_draw_bitmap(black, j*square_width, i*square_width, ALLEGRO_FLIP_HORIZONTAL);
                    }
                    else if (!strcmp(board[i+1][j+1].color, "white") && !strcmp(board[i+1][j+1].type, "pawn") ){
                        al_draw_bitmap(black_white_pawn, j*square_width, i*square_width, ALLEGRO_FLIP_HORIZONTAL);
                    }
                    else if (!strcmp(board[i+1][j+1].color, "red") && !strcmp(board[i+1][j+1].type, "pawn") ){
                        al_draw_bitmap(black_red_pawn, j*square_width, i*square_width, ALLEGRO_FLIP_HORIZONTAL);
                    }
                    else if (!strcmp(board[i+1][j+1].color, "white") && !strcmp(board[i+1][j+1].type, "king") ){
                        al_draw_bitmap(black_white_king, j*square_width, i*square_width, ALLEGRO_FLIP_HORIZONTAL);
                    }
                    else if (!strcmp(board[i+1][j+1].color, "red") && !strcmp(board[i+1][j+1].type, "king") ){
                        al_draw_bitmap(black_red_king, j*square_width, i*square_width, ALLEGRO_FLIP_HORIZONTAL);
                    }
                }

            }
            al_draw_rectangle((x[0]-1)*square_width,(y[0]-1)*square_width , x[0]*square_width,y[0]*square_width, al_map_rgb(0, 0, 255), 5);
            if(recstate==1) al_draw_rectangle((x[1]-1)*square_width,(y[1]-1)*square_width , x[1]*square_width,y[1]*square_width, al_map_rgb(0, 0, 255), 5);
            al_draw_text(font, al_map_rgb(200, 100, 0), 0, 0, NULL, turn);
            al_flip_display();
            al_clear_to_color( al_map_rgb( 0, 100, 0 ) );



}
