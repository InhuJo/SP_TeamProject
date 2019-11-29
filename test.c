#include <stdio.h>
//#include <ncursesw/curses.h>
#include <ncurses.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <locale.h>

void tty_mode(int);
void set_nodelay_mode(void);
void set_crmode(void);

int main(void)
{
	setlocale(LC_ALL,"");
	tty_mode(0);
	set_nodelay_mode();
	set_crmode();

	int i;
	char input;

	initscr();
	clear();
	keypad(stdscr, TRUE);
	for(i = 30 ; i< 70; i= i+10)
	{
		move(10,i);
		addstr("GAME START  ");
	}
	move(15,55);
	standout();
	addstr("GAME START");
	move(16,55);
	standend();
	addstr("game end");
	move(LINES-1,0);
	while(1)
	{
		input = getch();
		if(input == 'j')
		{
			mvdelch(15,55);
			standend();
			addstr("game start");
			move(16,55);
			standout();
			addstr("GAME END");
		}
		if(input == 'u' )
		{
			mvdelch(16,55);
			standend();
			addstr("game end");
			move(15,55);
			standout();
			addstr("GAME START");
		}
		else if(input == '\n')
		{
			break;
		}
	}
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

