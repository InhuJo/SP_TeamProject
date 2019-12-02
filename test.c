#include <stdio.h>
//#include <ncursesw/curses.h>
#include <ncurses.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <locale.h>
#include <stdlib.h>

#define titlerow 4
#define titlecol 11
#define menurow 15
#define menucol 33

void tty_mode(int);
void set_nodelay_mode(void);
void set_crmode(void);
void start_screen(void);
void game_play(void);
void game_rule(void);
void game_end(void);

int main(void)
{
	int cnt;
	setlocale(LC_ALL,"");
	tty_mode(0);
	set_nodelay_mode();
	set_crmode();

	initscr();
	clear();
	start_color(); //터미널에 color를 시작하겠다
	init_pair(1, COLOR_RED, COLOR_RED);  //글자색과 터미널바탕색 pair를 1로 초기화시킴
	init_pair(2, COLOR_GREEN, COLOR_GREEN);
	init_pair(3, COLOR_YELLOW, COLOR_YELLOW);
	init_pair(4, COLOR_BLUE, COLOR_BLUE);
	init_pair(5, COLOR_MAGENTA, COLOR_MAGENTA);
	init_pair(6, COLOR_CYAN, COLOR_CYAN);
	init_pair(7, COLOR_WHITE, COLOR_WHITE);

	 start_screen();
	/*clear();
	if (cnt == menurow+1)
		game_rule();
	refresh();*/

	//initscr();
	clear();
	move(30,20);
	addstr("오쥬이누아이라브유♥");
	move(LINES-1,0);
	refresh();
	sleep(2);
	tty_mode(1);
	endwin();
	return 0;
}
void start_screen(void)
{
	int i;
	char input;
	int cnt, cnt2, cnt3; //처음에 s눌렀을 때 이동안함 수정위

	//B
	move(titlerow, titlecol);
	attron(COLOR_PAIR(1));
	addstr("     ");

	move(titlerow+2, titlecol);
	attron(COLOR_PAIR(1));
	addstr("     ");

	move(titlerow+4, titlecol);
	attron(COLOR_PAIR(1));
	addstr("      ");

	for( i = titlerow; i < titlerow+4; i = i+1)
	{
		move(i, titlecol);
		attron(COLOR_PAIR(1));
		addstr("  ");
	}
	for( i = titlerow; i < titlerow+4; i = i+1)
	{
		move(i, titlecol+4);
		attron(COLOR_PAIR(1));
		addstr("  ");
	}
	//R
	for(i = titlerow; i < titlerow+5; i = i+1)
	{
		move(i, titlecol+6);
		attron(COLOR_PAIR(3));
		addstr("  ");
	}
	move(titlerow, titlecol +6);
	attron(COLOR_PAIR(3));
	addstr("     ");

	move(titlerow+2, titlecol+6);
	attron(COLOR_PAIR(3));
	addstr("      ");

	move(titlerow+3, titlecol+9);
	attron(COLOR_PAIR(3));
	addstr("  ");

	move(titlerow+4, titlecol+10);
	attron(COLOR_PAIR(3));
	addstr("  ");

	for(i = titlerow ; i< titlerow+3 ; i = i+1)
	{
		move(i, titlecol+10);
		attron(COLOR_PAIR(3));
		addstr("  ");
	}
	//E
	for(i = titlerow ; i < titlerow + 5; i = i +1)
	{
		move(i, titlecol + 12);
		attron(COLOR_PAIR(2));
		addstr("  ");
	}
	move(titlerow, titlecol+12);
	attron(COLOR_PAIR(2));
	addstr("      ");

	move(titlerow+2, titlecol+12);
	attron(COLOR_PAIR(2));
	addstr("      ");

	move(titlerow+4, titlecol+12);
	attron(COLOR_PAIR(2));
	addstr("      ");

	//A
	for(i = titlerow ; i<titlerow+5; i = i+1)
	{
		move(i, titlecol+18);
		attron(COLOR_PAIR(7));
		addstr("  ");
		move(i, titlecol + 22);
		attron(COLOR_PAIR(7));
		addstr("  ");
	}
	move(titlerow, titlecol+18);
	attron(COLOR_PAIR(7));
	addstr("      ");
	move(titlerow+2, titlecol+18);
	attron(COLOR_PAIR(7));
	addstr("      ");

	//K
	for(i = titlerow; i<titlerow+5; i = i+1)
	{
		move(i, titlecol+24);
		attron(COLOR_PAIR(5));
		addstr("  ");
	}
	move(titlerow+2, titlecol+26);
	attron(COLOR_PAIR(5));
	addstr("  ");

	move(titlerow+1, titlecol+27);
	attron(COLOR_PAIR(5));
	addstr("  ");

	move(titlerow, titlecol+28);
	attron(COLOR_PAIR(5));
	addstr("   ");
	move(titlerow+3, titlecol+27);
	attron(COLOR_PAIR(5));
	addstr("  ");
	move(titlerow+4, titlecol+28);
	attron(COLOR_PAIR(5));
	addstr("   ");

	//O
	for(i = titlerow ; i<titlerow+5; i = i+1)
	{
		move(i,titlecol+36);
		attron(COLOR_PAIR(6));
		addstr("  ");
		move(i, titlecol+40);
		attron(COLOR_PAIR(6));
		addstr("  ");
	}

	move(titlerow, titlecol+38);
	attron(COLOR_PAIR(6));
	addstr("  ");
	move(titlerow+4, titlecol+38);
	attron(COLOR_PAIR(6));
	addstr("  ");

	//U
	for(i = titlerow; i<titlerow+5; i=i+1)
	{
		move(i, titlecol+42);
		attron(COLOR_PAIR(4));
		addstr("  ");
		move(i, titlecol+46);
		attron(COLOR_PAIR(4));
		addstr("  ");
	}
	move(titlerow+4, titlecol+42);
	attron(COLOR_PAIR(4));
	addstr("    ");


	//T
	for(i = titlerow; i < titlerow+5; i = i+1)
	{
		move(i, titlecol+50);
		attron(COLOR_PAIR(2));
		addstr("  ");
	}
	move(titlerow, titlecol+48);
	attron(COLOR_PAIR(2));
	addstr("      ");

	//!
	for(i = titlerow; i<titlerow+3; i = i+1)
	{
		move(i, titlecol +55);
		attron(COLOR_PAIR(3));
		addstr("  ");
	}
	move(titlerow+4, titlecol+55);
	attron(COLOR_PAIR(3));
	addstr("  ");

	//game start, game end
	move(menurow,menucol);
	standout();
	addstr("GAME START");
	cnt = menurow;

	move(menurow+2, menucol);
	standend();
	addstr(" game end");

	move(menurow+1, menucol);
	standend();
	addstr("game rule");
	refresh();

	while(1)
	{
		input = getchar();
		if(input == 's')
		{
			if(cnt==menurow)
			{
				mvdelch(menurow,menucol);
				standend();
				addstr("game start");
				move(menurow+1,menucol);
				standout();
				addstr(" GAME RULE");
				cnt = menurow+1;
				refresh();
			}
			else if(cnt == menurow+1)
			{
				mvdelch(menurow+1,menucol);
				standend();
				addstr("game rule");
				move(menurow+2,menucol);
				standout();
				addstr(" GAME END");
				cnt = menurow+2;
				refresh();
			}

		}

		if(input == 'w' )
		{
			if(cnt==menurow+2)
			{
				mvdelch(menurow+2,menucol);
				standend();
				addstr(" game end");
				move(menurow+1,menucol);
				standout();
				addstr("GAME RULE");
				cnt = menurow+1;
				refresh();
			}
			else if(cnt == menurow+1)
			{
				mvdelch(menurow+1, menucol);
				standend();
				addstr(" game rule");
				move(menurow, menucol);
				standout();
				addstr("GAME START");
				cnt = menurow;
				refresh();
			}

		}
		else if(input == 'l')
		{

			if(cnt == menurow) //game start
			{
				game_play();
			}
			else if(cnt == menurow+1)
				game_rule();
			else
				game_end();
			break;
		}
	}
	//return cnt;
}
void game_play(void)
{
	clear();
	move(LINES/2, COLS/2);
	addstr("안녕");
	refresh();
}
void game_rule(void)
{
	clear();
	char input;
	move(LINES/2, COLS/2-20);
	addstr(" 이 게임은 공을 튀기어 벽돌을 깨는 프로그램입니다");
	move(LINES/2+10, COLS/2);
	addstr("BACK");
	refresh();
	while(1)
	{
		input = getchar();
		if(input == 'l')
		{
			clear();
			start_screen();
		}
	}
}
void game_end(void)
{
	endwin();
	tty_mode(1);
	exit(1);
}



	void set_crmode(void)
	{	
		struct termios ttystate;
		tcgetattr(0, &ttystate);
		ttystate.c_lflag &= ~ICANON;
		ttystate.c_cc[VMIN] = 1;

		tcsetattr(0, TCSANOW, &ttystate);
	}

	void set_nodelay_mode(void)
	{
		int termflags;
		termflags = fcntl(0, F_GETFL);
		termflags |= O_NDELAY;
		fcntl(0, F_SETFL, termflags);
	}
	void tty_mode(int how)
	{
		static struct termios original_mode;
		static int original_flags;

		if(how == 0)
		{
			tcgetattr(0, &original_mode);
			original_flags = fcntl(0,F_GETFL);
		}
		else 
		{
			tcsetattr(0, TCSANOW, &original_mode);
			original_flags = fcntl(0, F_SETFL, original_flags);
		}
	}

