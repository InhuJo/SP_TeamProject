#include <stdio.h>
#include <ncursesw/curses.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include <locale.h>
#include <termios.h>
#include <fcntl.h>
#include <signal.h>

#define LEFTEDGE 0
#define RIGHTEDGE 78
#define TOPEDGE 0
#define DOWNEDGE 22

typedef enum DIRECT_BALL
{ TOP, RIGHT_TOP, RIGHT_DOWN, DOWN, LEFT_DOWN, LEFT_TOP } DIRECT_BALL;

typedef enum DIRECT_BAR
{ LEFT, RIGHT } DIRECT_BAR;

typedef struct Ball
{
	int ready;
	int HP;
	int x, y;
	DIRECT_BALL direct;
}Ball;

typedef struct Bar
{
	int x[7];
	int y;
	DIRECT_BAR direct;

}Bar;

int ball_status[4][6] = {
	{ 3, 2, -1, -1, -1, 4 },
	{ -1, 5, 4, -1, -1, -1 },
	{ -1, -1, 1, 0, 5, -1 },
	{ -1, -1, -1, -1, 2, 1} };

int block_status[6] = { 3, 2, 1, 0, 5, 4 };

/* function and variable for ball */
Ball ball;
void init_ball();
void update_ball();
int check_collision(int, int);

/* function and variable for bar */
Bar bar;
void init_bar();
void update_bar();

void set_crmode(void);
void set_nodelay_mode(void);
void tty_mode(int);
int set_ticker(int n_msecs);

char *blank = " ";
int game_status = 1;

int main()
{
	int input;

	setlocale(LC_ALL, "");

	tty_mode(0);
	set_crmode();
	set_nodelay_mode();

	initscr();
	clear();

	move(10, 31);
	addstr("BREAKOUT GAME!");
	
	move(15, 33);
	standout();
	addstr("GAME START");
	
	move(16, 34);
	standend();
	addstr("GAME END");

	move(LINES - 1, 0);

	while(1)
	{
		input = getchar();

		if(input == 's')
		{
			mvdelch(15, 33);
			standend();
			addstr("GAME START");

			move(16, 34);
			standout();
			addstr("GAME END");

			game_status = 0;
		}
		else if(input == 'w')
		{
			mvdelch(16, 34);
			standend();
			addstr("GAME END");

			move(15, 33);
			standout();
			addstr("GAME START");

			game_status = 1;
		}
		else if(input == 'f')
			break;

		move(LINES - 1, COLS - 1);
		refresh();
	}
	standend();

	if(game_status == 0)
	{
		endwin();
		tty_mode(1);

		return 0;
	}

	clear();

	init_ball();
	init_bar();

	signal(SIGALRM, update_ball);

	if(set_ticker(70) == -1)
		perror("set_ticker");

	move(ball.y, ball.x);
	addstr("●");

	for(int i = 0; i < 7; i++)
	{
		move(bar.y, bar.x[i]);
		addstr("■");
	}
	refresh();

	while((input = getchar()) != 'q')
	{
		switch(input)
		{
			case '1':
				ball.ready = 1;
				ball.direct = TOP;
				break;
			case '2':
				ball.ready = 1;
				ball.direct = RIGHT_TOP;
				break;
			case '3':
				ball.ready = 1;
				ball.direct = LEFT_TOP;
				break;
			case 'a':
				bar.direct = LEFT;
				update_bar();
				break;
			case 'd':
				bar.direct = RIGHT;
				update_bar();
				break;
		}
	}

	endwin();
	tty_mode(1);

	return 0;
}

void init_ball()
{
	ball.ready = 0;
	ball.HP = 3;
	ball.x = 33;
	ball.y = 21;
	ball.direct = TOP;
}

void update_ball()
{
	move(ball.y, ball.x);
	addstr(blank);

	if(ball.ready == 1)
	{	
		switch(ball.direct)
		{
			case TOP:
				if(check_collision(ball.x, ball.y - 1) == 0)
					ball.y--;
				break;
			case RIGHT_TOP:
				if(check_collision(ball.x + 1, ball.y - 1) == 0)
				{
					ball.x++;
					ball.y--;
				}
				break;
			case RIGHT_DOWN:
				if(check_collision(ball.x + 1, ball.y + 1) == 0)
				{
					ball.x++;
					ball.y++;
				}
				break;
			case DOWN:
				if(check_collision(ball.x, ball.y + 1) == 0)
					ball.y++;
				break;
			case LEFT_DOWN:
				if(check_collision(ball.x - 1, ball.y + 1) == 0)
				{
					ball.x--;
					ball.y++;
				}
				break;
			case LEFT_TOP:
				if(check_collision(ball.x - 1, ball.y  - 1) == 0)
				{
					ball.x--;
					ball.y--;
				}
				break;
		}
	}

	move(ball.y, ball.x);
	addstr("●");
	move(LINES-1, 0);
	refresh();
}

int check_collision(int x, int y)
{
	int i;

	for(i = 0; i < 7; i++)
		if(y == bar.y)
			if(x >= bar.x[0] && x <= (bar.x[6] + 1) ||
				((x + 1) >= bar.x[0] && (x + 1) <= (bar.x[6] + 1)))
			{
				ball.direct = block_status[ball.direct];
				return 1;
			}

	if(y < TOPEDGE)
	{
		ball.direct = ball_status[0][ball.direct];
		return 1;
	}

	if(x > RIGHTEDGE)
	{
		ball.direct = ball_status[1][ball.direct];
		return 1;
	}

	if(y > DOWNEDGE)
	{
		ball.ready = 0;
		ball.HP--;
		ball.x = bar.x[3];
		ball.y = bar.y - 1;

		return -1;
	}

	if(x < LEFTEDGE)
	{
		ball.direct = ball_status[3][ball.direct];
		return 1;
	}

	return 0;
}

void init_bar()
{
	bar.x[0] = 30;
	bar.x[1] = 31;
	bar.x[2] = 32;
	bar.x[3] = 33;
	bar.x[4] = 34;
	bar.x[5] = 35;
	bar.x[6] = 36;
	bar.y = 22;
}

void update_bar()
{
	int i;

	if(bar.direct == LEFT)
	{
		/* check edge of leftmost bar */
		if(bar.x[0] - 1 >= LEFTEDGE)
		{
			/* remove previous bar */
			for(i = 0; i < 7; i++)
			{
				move(bar.y, bar.x[i]);
				addstr(blank);
			}

			/* draw new bar */
			for(i = 0; i < 7; i++)
			{
				move(bar.y, --bar.x[i]);
				addstr("■");
			}

		}
	}
	else if(bar.direct == RIGHT)
	{
		/* check edge of rightmost bar */
		if(bar.x[6] + 1 <= RIGHTEDGE)
		{
			/* remove previous bar */
			for(i = 0; i < 7; i++)
			{
				move(bar.y, bar.x[i]);
				addstr(blank);
			}

			/* draw new bar */
			for(i = 0; i < 7; i++)
			{
				move(bar.y, ++bar.x[i]);
				addstr("■");
			}
		}
	}

	if(ball.ready == 0)
	{
		move(ball.y, ball.x);
		addstr(blank);
		
		ball.x = bar.x[3];
		move(ball.y, ball.x);
		addstr("●");
	}

	move(LINES-1, 0);
	refresh();
}			

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
