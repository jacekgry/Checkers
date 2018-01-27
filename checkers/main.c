#include "functions.h"
int main(){
    int i,j,choice,k=1, action, recstate=0, menustate=1, check, continue_game, next_capture=0, obligatory_x0=NULL, obligatory_y0=NULL, komputer = 1, draw = 1, keypressed = 1, x[]={1, 1}, y[]={1, 1};
    const int square_width = 100;
    enum key {UP, DOWN, LEFT, RIGHT, ENTER, ESCAPE};
    char turn[6], num[1], list[100][18] ;
    init();
menu:
    draw = 1;
    while(1){

        if(draw){
            draw = 0;
            drawmenu(menustate);
        }

        al_wait_for_event(event_queue, &events);
        if(events.type == ALLEGRO_EVENT_KEY_DOWN){
            keypressed = 1;
            switch(events.keyboard.keycode){
                case ALLEGRO_KEY_DOWN:
                    action = DOWN;
                    break;
                case ALLEGRO_KEY_UP:
                    action = UP;
                    break;
                case ALLEGRO_KEY_ENTER:
                    action = ENTER;
                    break;
                case ALLEGRO_KEY_ESCAPE:
                    action = ESCAPE;
                    break;
            }

        }

        if(keypressed && events.type == ALLEGRO_EVENT_TIMER){
            keypressed = 0;
            switch(action){
                case DOWN:
                    if(menustate<6) menustate+=1;
                    break;
                case UP:
                    if(menustate>1) menustate-=1;
                    break;
                case ENTER:
                    switch(menustate){
                        case 1:
                            komputer = 0;
                            continue_game = 0;
                            read_board(board, turn, &komputer, &next_capture, &obligatory_x0, &obligatory_y0, continue_game, 0, NULL);

                            goto game;
                            break;
                        case 2:
                            komputer = 1;
                            continue_game = 0;
                            read_board(board, turn, &komputer, &next_capture, &obligatory_x0, &obligatory_y0, continue_game, 0, NULL);

                            goto game;
                            break;

                        case 3:
                            continue_game = 1;
                            read_board(board, turn, &komputer, &next_capture, &obligatory_x0, &obligatory_y0, continue_game, 0, NULL);

                            goto game;
                            break;

                        case 4:
                            save(1, board, turn, komputer, next_capture, obligatory_x0, obligatory_y0);
                            al_show_native_message_box(display, NULL, "Game saved", "Game has been successfully saved!", NULL, NULL);
                            break;
                        case 5:

                            i=getlist(list);
                            al_flip_display();

                            while(1){
                                k=1;
                                for(j=i-2; j>=0 && k<=9;j-=2){
                                    sprintf(num, "%d.",k);
                                    al_draw_text(font, al_map_rgb(200, 100, 0), 250, 18*k, NULL, num);
                                    al_draw_text(font, al_map_rgb(200, 200, 220), 300, 18*k, NULL, list[j]);
                                    k++;
                                }
                                al_flip_display();
                                al_clear_to_color( al_map_rgb(0 , 0, 0 ) );
                                al_wait_for_event(event_queue, &events);
                                if(events.type == ALLEGRO_EVENT_KEY_DOWN && events.keyboard.keycode<=i/2+27 && events.keyboard.keycode>=28 && events.keyboard.keycode<=36){
                                    read_board(board, turn, &komputer, &next_capture, &obligatory_x0, &obligatory_y0, continue_game, 1, list[i-2*(events.keyboard.keycode-27)]);
                                    goto game;
                                }
                                else if(events.type == ALLEGRO_EVENT_KEY_DOWN && events.keyboard.keycode==ALLEGRO_KEY_ESCAPE ) goto menu;

                            }
                            read_board(board, turn, &komputer, &next_capture, &obligatory_x0, &obligatory_y0, continue_game, 1, list[i-2*choice]);
                            goto game;
                            break;
                        case 6:
                            goto exit;
                            break;
                    }
                    break;
                case ESCAPE:
                    goto exit;
                    break;

            }
        draw = 1;
        }
    }


game:
    draw = 1;
    while( 1 )
    {
        if(draw){
            draw = 0;
            drawgame(square_width, x, y, turn, recstate);
        }

        al_wait_for_event(event_queue, &events);

        if(events.type == ALLEGRO_EVENT_KEY_DOWN){
            switch(events.keyboard.keycode){
                case ALLEGRO_KEY_DOWN:
                    action = DOWN;
                    break;
                case ALLEGRO_KEY_UP:
                    action = UP;
                    break;
                case ALLEGRO_KEY_RIGHT:
                    action = RIGHT;
                    break;
                case ALLEGRO_KEY_LEFT:
                    action = LEFT;
                    break;
                case ALLEGRO_KEY_ENTER:
                    action = ENTER;
                    break;
                case ALLEGRO_KEY_ESCAPE:
                    action = ESCAPE;
                    break;
            }
            keypressed = 1;
        }




            if(keypressed && events.type == ALLEGRO_EVENT_TIMER){
            switch(action){
                case DOWN:
                    if(y[recstate]<8){
                        y[recstate]+=1;
                    }
                    break;
                case UP:
                    if(y[recstate]>1){
                        y[recstate]-=1;
                    }
                    break;
                case RIGHT:
                    if(x[recstate]<8){
                        x[recstate]+=1;
                    }
                    break;
                case LEFT:
                    if(x[recstate]>1){
                        x[recstate]-=1;
                    }
                    break;
                case ENTER:
                    check = check_move1(y[0], x[0], y[1], x[1], board, turn);
                    if(recstate==0){
                        x[1]=x[0];
                        y[1]=y[0];
                        recstate=(recstate+1)%2;
                    }

                    else if((!next_capture || (y[0]==obligatory_x0 && x[0]==obligatory_y0 ) ) && check>7){
                        move(board, y[0], x[0], y[1], x[1]);
                        if(!capture_possible(y[1], x[1], board) || check<11){
                            if(!strcmp(turn, "white")) strcpy(turn, "red");
                            else strcpy(turn, "white");
                            next_capture = 0;
                        }
                        else{
                            next_capture = 1;
                            obligatory_x0 = y[1];
                            obligatory_y0 = x[1];
                        }
                        save(0, board, turn, komputer, next_capture, obligatory_x0, obligatory_y0);
                        recstate=(recstate+1)%2;
                    }
                    else{
                        recstate=(recstate+1)%2;
                      }
                    break;
                case ESCAPE:
                    keypressed = 0;
                    goto menu;
                    break;
            }
        draw = 1;
        keypressed = 0;
        }


        if (!move_possible(board, turn)){
            if(strcmp(turn, "white")){
                    al_draw_bitmap(white_won, 0, 0, NULL);
                    al_flip_display();
                    al_clear_to_color(al_map_rgb(0,0,0));
                    al_rest(1);
                    goto menu;
            }
            else{
                    al_draw_bitmap(red_won, 0, 0, NULL);
                    al_flip_display();
                    al_clear_to_color(al_map_rgb(0,0,0));
                    al_rest(1);
                    goto menu;

            }

        }

        if(komputer && !strcmp(turn, "red")){
                int comp_move[4];
                computer(board, comp_move, next_capture, obligatory_x0, obligatory_y0);
                check = check_move1(comp_move[0], comp_move[1], comp_move[2], comp_move[3], board, turn);
                move(board, comp_move[0], comp_move[1], comp_move[2], comp_move[3]);
                if(!capture_possible(comp_move[2], comp_move[3], board) || check<11){
                    if(!strcmp(turn, "white")) strcpy(turn, "red");
                    else strcpy(turn, "white");
                    next_capture = 0;
                }
                else{
                    next_capture = 1;
                    obligatory_x0 = comp_move[2];
                    obligatory_y0 = comp_move[3];
                }
                save(0, board, turn, komputer, next_capture, obligatory_x0, obligatory_y0);
        }

     }

exit:
    destroy();
    return 0;
}
