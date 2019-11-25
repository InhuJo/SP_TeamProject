#include <stdio.h>
#include <curses.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

void tty_mode(int);
void set_nodelay_mode(void);
void set_crmode(void);

int main(void)
{
	tty_mode(0);
	set_nodelay_mode();
	set_crmode();

	int i;
	char input;

	initscr();
	clear();
	for(i = 50 ; i< 80; i= i+10)
	{
		move(10,i);
		addstr("GAME START  ");
	}
	move(40, 65);
	standout();
	addstr("Game START");
	move(50,65);
	standend();
	addstr("Game End");
	while(1)
	{
		input = getchar();
		if(input == 'u')
		{
			move(50,65);
			standout();
		}
		else if(input == 'k')
		{
			break;
		}
	}
	refresh();
	endwin();
	sleep(3);

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

