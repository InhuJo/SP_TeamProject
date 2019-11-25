#include <stdio.h>
#include <ncursesw/curses.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include <locale.h>
#include <termios.h>
#include <fcntl.h>

#define LEFTEDGE 0
#define RIGHTEDGE 78
#define TOPEDGE 0
#define BOTTOMEDGE 24

typedef enum DIRECT 
{ TOP, LEFT_TOP, LEFT_DOWN, DOWN, RIGHT_DOWN, RIGHT_TOP } DIRECT;

typedef struct Ball
{
	int HP;
	int x, y;
	DIRECT direct;
}Ball;

typedef struct Bar
{
	int x[3];
	int y;
}Bar;

void initball();
void update();
void set_crmode(void);
void set_nodelay_mode(void);
void tty_mode(int);
int set_ticker(int n_msecs);
char blank[] = "  ";
Ball ball;

int main()
{
	int input;

	setlocale(LC_ALL, "");
	
	/*tty_mode(0);
	set_crmode();
	set_nodelay_mode();*/

	initscr();
	clear();

	initball();

	//set_ticker(500);

	move(ball.y, ball.x);
	addstr("●");
	move(LINES-1, COLS-1);
	refresh();

/*	while(1)
	{
		input = getchar();

		switch(input)
		{
			case '0':
				break;
			case '1':
				break;
			case '2':
				break;
			case '3':
				break;
			case '4':
				break;
			case '5':
				break;
			case '6':
				break;
		}
	}*/
	getch();

	endwin();
	tty_mode(1);

	return 0;
}

void initball()
{
	ball.x = 38;
	ball.y = 12;
	ball.direct = TOP;
}

/*void update()
{
	clock_t curTime = clock();

	if(curTime - ball.oldTime > ball.moveTime)
	{
		ball.oldTime = curTime;

		switch(ball.direct)
		{
			case TOP:
				ball.y--;
				break;
			case LEFT_TOP:
				ball.x++;
				ball.y--;
				break;
			case LEFT_DOWN:
				ball.x++;
				ball.y++;
				break;
			case DOWN:
				ball.y++;
				break;
			case RIGHT_DOWN:
				ball.x--;
				ball.y++;
				break;
			case RIGHT_TOP:
				ball.x--;
				ball.y--;
				break;
		}
	}

	if(ball.x < 0 || ball.x > 78 || ball.y > 24 || ball.y < 0)
	{
		ball.x = 38;
		ball.y = 12;
		ball.direct = TOP;
	}
}
*/
void set_crmode()
{
	struct termios ttystate;

	tcgetattr(0, &ttystate);
	ttystate.c_lflag &= ~ICANON;
	ttystate.c_cc[VMIN] = 1;

	tcsetattr(0, TCSANOW, &ttystate);
}

void set_nodelay_mode()
{
	int termflag;

	termflag = fcntl(0, F_GETFL);
	termflag |= O_NDELAY;

	fcntl(0, F_SETFL, termflag);
}

void tty_mode(int how)
{
	static struct termios original_mode;
	static int original_flags;

	if(how == 0)
	{
		tcgetattr(0, &original_mode);
		original_flags = fcntl(0, F_GETFL);
	}
	else
	{
		tcsetattr(0, TCSANOW, &original_mode);
		original_flags = fcntl(0, F_SETFL, original_flags);
	}
}

int set_ticker(int n_msecs)
{
	struct itimerval new_timeset;
	long n_sec, n_usecs;

	n_sec = n_msecs / 1000;
	n_usecs = ( n_msecs % 1000 ) * 1000L;

	new_timeset.it_interval.tv_sec = n_sec;
	new_timeset.it_interval.tv_usec = n_usecs;
	new_timeset.it_value.tv_sec = n_sec;
	new_timeset.it_value.tv_usec = n_usecs;

	return setitimer(ITIMER_REAL, &new_timeset, NULL);
}
