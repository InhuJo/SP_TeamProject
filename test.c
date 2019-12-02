#include <stdio.h>
//#include <ncursesw/curses.h>
#include <ncurses.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <locale.h>

#define titlerow 4
#define titlecol 11
#define menurow 15
#define menucol 35

void tty_mode(int);
void set_nodelay_mode(void);
void set_crmode(void);
void start_screen(void);

int main(void)
{
	setlocale(LC_ALL,"");
	tty_mode(0);
	set_nodelay_mode();
	set_crmode();

	initscr();
	clear();
	keypad(stdscr, TRUE);   //키보드 입력
	start_color(); //터미널에 color를 시작하겠다
	init_pair(1, COLOR_RED, COLOR_RED);  //글자색과 터미널바탕색 pair를 1로 초기화시킴
	init_pair(2, COLOR_GREEN, COLOR_GREEN);
	init_pair(3, COLOR_YELLOW, COLOR_YELLOW);
	init_pair(4, COLOR_BLUE, COLOR_BLUE);
	init_pair(5, COLOR_MAGENTA, COLOR_MAGENTA);
	init_pair(6, COLOR_CYAN, COLOR_CYAN);
	init_pair(7, COLOR_WHITE, COLOR_WHITE);

	start_screen();

	refresh();

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
	//ㅂ
	/*for(i = titlerow ; i< titlerow + 5 ; i= i +1 )
	{
		move(i,10);
		attron(COLOR_PAIR(1));
		addstr("■ ");
	}
	move(titlerow+1, titlecol);
	attron(COLOR_PAIR(1));
	addstr("■   ");
	
	move(titlerow+4, titlecol);
	attron(COLOR_PAIR(1));
	addstr("    ");
	
	for(i = titlerow; i<titlerow + 5; i = i +1)
	{
		move(i, titlecol +3);
		attron(COLOR_PAIR(1));
		addstr("  ");
	}
	//ㅕ
	move(titlerow+1, titlecol+6);
	attron(COLOR_PAIR(1));
	addstr("  ");

	move(titlerow+3, titlecol+6);
	attron(COLOR_PAIR(1));
	addstr("  ");

	for(i = titlerow; i<titlerow +5 ; i = i+1)
	{
		move(i, titlecol+7);
		attron(COLOR_PAIR(1));
		addstr("  ");
	}
	//ㄱ
	move(titlerow+6, titlecol-1);
	attron(COLOR_PAIR(1));
	addstr("        ");

	for(i = titlerow+6; i < titlerow+8; i = i+1)
	{
		move(i, titlecol+7);
		attron(COLOR_PAIR(1));
		addstr("  ");
	}
	//ㄷ
	move(titlerow, titlecol + 9);
	attron(COLOR_PAIR(3));
	addstr("         ");
	
	for(i = titlerow; i < titlerow+2; i = i+1)
	{
		move(i, titlecol+9);
		attron(COLOR_PAIR(3));
		addstr("  ");
	}

	move(titlerow+2, titlecol +9);
	attron(COLOR_PAIR(3));
	addstr("         ");*/

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
		attron(COLOR_PAIR(4));
		addstr("  ");
		move(i, titlecol + 23);
		attron(COLOR_PAIR(4));
		addstr("  ");
	}
	move(titlerow, titlecol+18);
	attron(COLOR_PAIR(4));
	addstr("      ");
	move(titlerow+2, titlecol+18);
	attron(COLOR_PAIR(4));
	addstr("      ");
	
	//K
	for(i = titlerow; i<titlerow+5; i = i+1)
	{
		move(i, titlecol+25);
		attron(COLOR_PAIR(5));
		addstr("  ");
	}
	move(titlerow+2, titlecol+27);
	attron(COLOR_PAIR(5));
	addstr("   ");

	move(titlerow+1, titlecol+29);
	attron(COLOR_PAIR(5));
	addstr("   ");

	move(titlerow, titlecol+31);
	attron(COLOR_PAIR(5));
	addstr("  ");
	move(titlerow+3, titlecol+29);
	attron(COLOR_PAIR(5));
	addstr("   ");
	move(titlerow+4, titlecol+31);
	attron(COLOR_PAIR(5));
	addstr("  ");

	move(menurow,menucol);
	standout();
	addstr("GAME START");
	move(menurow+1, menucol);
	standend();
	addstr("game end");
	move(LINES-1,0);
	while(1)
	{
		input = getch();
		if(input == 'j')
		{
			mvdelch(menurow,menucol);
			standend();
			addstr("game start");
			move(menurow+1,menucol);
			standout();
			addstr("GAME END");
		}
		if(input == 'u' )
		{
			mvdelch(menurow+1,menucol);
			standend();
			addstr("game end");
			move(menurow,menucol);
			standout();
			addstr("GAME START");
		}
		else if(input == '\n')
		{
			break;
		}
	}
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

