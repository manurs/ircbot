#include "funciones.h"

WINDOW *create_newwin(int height, int width, int starty, int startx);

void intHandler(int);

int main(int argc, char *argv[]){
	int startx, starty, width, height;
	//int ch;
	char msg[512];
	pthread_t /*h,*/ h1, h2;

	initscr();			/* Start curses mode 		*/
	cbreak();			
	echo();
	keypad(stdscr, TRUE);
	//attron(A_ALTCHARSET);

	height = LINES;
	width = COLS/4;
	starty = (LINES - height) / 2;	/* Calculating for a placement */
	startx = (COLS - width);	/* of the window		*/

	refresh();
	title_win = create_newwin(3, 3*COLS/4, 0, 0);
	input_win = newwin(height, width, starty, startx);
	scrollok(input_win, TRUE);
	output_win = newwin(LINES-3, (3*COLS/4) - 2, 3, 1);
	scrollok(output_win, TRUE);

	mvwprintw(title_win, 1, 1, "IRCBot (QUIT to exit, Ctrl+C to enable scrolling, F1 to disable)");
	wrefresh(title_win);

	mvwprintw(input_win, LINES - 3, 1, "Message:");
	wrefresh(input_win);
	wrefresh(output_win);

	/*for(int i = 1; i < 100; i++){
		//mvwprintw(output_win, i, 1, "HOLA MUNDO %i\n", i);
		wprintw(output_win, "HOLA MUNDO %i\n", i);
		wrefresh(output_win);
	}*/

	signal(SIGINT, intHandler);
	plogf = fopen("log", "w+");
	connect_client(&h1, &h2);
	int i = LINES-5;
	char *whitespaces = malloc(width - 2);
	char *whitespaces2 = NULL;
	if((width -2 - strlen("Message: ")) > 0){
		whitespaces2 = malloc(width -2 - strlen("Message: "));
	}
	memset(whitespaces, ' ', width - 3);
	memset(whitespaces2, ' ', width - 3 - strlen("Message: "));
	whitespaces[width-3] = '\0';
	whitespaces[width - 3 - strlen("Message: ")] = '\0';
	while(1){
		//mvwscanw(input_win, LINES-2, strlen("Message: "), "%s", msg);
		//pthread_create(&h, NULL, getarrows, NULL);
		mvwgetnstr(input_win, LINES - 3, strlen("Message: "), msg, 512);
		
		char* p;
		if((p = strchr(msg, '\n')) != NULL) *p = '\0';
		if(strcmp(msg, "QUIT") == 0) break;
		mvwprintw(input_win, i, 1, "%s", msg);

		mvwprintw(input_win, LINES-3, strlen("Message: "), whitespaces2);
		mvwprintw(input_win, LINES-2, 1, whitespaces);
		wrefresh(input_win);
		sprintf(msg,"%s%c%c",msg,0X0d,0X0d);
		escribir(sockfd,msg);
		wrefresh(output_win);
		i -= 2;
		if(i - 2 <= 0){
			i = LINES - 5;
		}
		mvwprintw(input_win, i, 1, whitespaces);
		mvwprintw(input_win, i+1, 1, whitespaces);
		wrefresh(input_win);
		//if((ch = getch()) == KEY_F(1)) break;
	}
	free(whitespaces);
	free(whitespaces2);
	//pthread_cancel(h);
	//pthread_join(h, NULL);
	pthread_cancel(h1);
	pthread_join(h1, NULL);
	pthread_cancel(h2);
	pthread_join(h2, NULL);
	delwin(input_win);
	delwin(title_win);
	delwin(output_win);
	fclose(plogf);
	endwin();			/* End curses mode		  */
	return 0;

}

WINDOW *create_newwin(int height, int width, int starty, int startx){

	WINDOW *local_win;

	local_win = newwin(height, width, starty, startx);
	box(local_win, 0 , 0);		/* 0, 0 gives default characters 
					 			 * for the vertical and horizontal
					 			 * lines			*/
	wrefresh(local_win);		/* Show that box 		*/

	return local_win;
}


void intHandler(int dummy) {
    int c;
    //noecho();
    idlok(output_win, TRUE);
    scrollok(output_win, TRUE);
    wprintw(output_win, "Interrupcion capturada\n");
    wrefresh(output_win);
    while((c = getch()) != KEY_F(1)){
		switch(c)
		{	case KEY_UP:
				wscrl(output_win, 1);
				wrefresh(output_win);
				break;
			case KEY_DOWN:
				wscrl(output_win, -1);
				wrefresh(output_win);
				break;
			default:
				break;
		}
	}
	echo();
}