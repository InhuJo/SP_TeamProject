#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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
#define BLOCKCOUNT 70

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

typedef struct Block
{
	int life;
	int x, y;
}Block;

int ball_status[4][6] = {
	{ 3, 2, -1, -1, -1, 4 },
	{ -1, 5, 4, -1, -1, -1 },
	{ -1, -1, 1, 0, 5, -1 },
	{ -1, -1, -1, -1, 2, 1} };

int block_status[6] = { 3, 2, 1, 0, 5, 4 };

/* function and variable for ball */
Ball ball;
void init_ball();
int update_xball();
void update_ball();
int check_collision(int, int);

/* function and variable for bar */
Bar bar;
void init_bar();
void update_bar();

/* function and varibale for block */
Block block[BLOCKCOUNT];
int search(int, int, int);
void init_block(int);

void set_crmode(void);
void set_nodelay_mode(void);
void tty_mode(int);
int set_ticker(int n_msecs);

char *blank = " ";
int game_status = 1;
int x_ball;

int main()
{
	int input, i;

	setlocale(LC_ALL, "");

	tty_mode(0);
	set_crmode();
	set_nodelay_mode();

	initscr();
	clear();

	init_ball();
	init_bar();

	signal(SIGALRM, update_ball);

	if(set_ticker(70) == -1)
		perror("set_ticker");


	/* init first game screen */
	init_block(BLOCKCOUNT);
	
	for(i = 0; i < BLOCKCOUNT; i++)
	{
		move(block[i].y, block[i].x);
		addstr("□");
	}
	refresh();

	move(ball.y, ball.x);
	addstr("●");

	for(i = 0; i < 7; i++)
	{
		move(bar.y, bar.x[i]);
		addstr("■");
	}
	refresh();

	/* game start */
	move(LINES-1, 0);
	addstr("HP ");
	move(LINES-1, 3);
	addstr("♥");
	move(LINES-1, 5);
	addstr("♥");
	move(LINES-1, 7);
	addstr("♥");
	
	while(ball.HP > 0)
	{
		input = getchar();

		switch(input)
		{
			case 'j':
				if(ball.ready == 0)
				{
					ball.direct = TOP;
					ball.ready = 1;
				}
				break;
			case 'k':
				if(ball.ready == 0)
				{
					ball.direct = LEFT_TOP;
					ball.ready = 1;
				}
				break;
			case 'l':
				if(ball.ready == 0)
				{
					ball.direct = RIGHT_TOP;
					ball.ready = 1;
				}
				break;
			case 'h':
				if(ball.ready == 1 && (ball.y == bar.y || ball.y + 1 == bar.y)) 
					if(ball.x >= bar.x[0] && ball.x <= (bar.x[6] + 1) ||
						((ball.x + 1) >= bar.x[0] && (ball.x + 1) <= (bar.x[6] + 1)))
					{
						ball.y = bar.y - 1;
						ball.ready = 0;
						x_ball = update_xball();
					}
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

int update_xball()
{
	for(int i = 0; i < 7; i++)
		if(ball.x == bar.x[i])
			return i;
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
	move(LINES-1, COLS-1);
	refresh();
}

int check_collision(int x, int y)
{
	int i, count = 0;

	/* check collision with bar */
	for(i = 0; i < 7; i++)
		if(y == bar.y)
			if((x >= bar.x[0] && x <= (bar.x[6] + 1)) ||
				((x + 1) >= bar.x[0] && (x + 1) <= (bar.x[6] + 1)))
			{
				ball.direct = block_status[ball.direct];
				return -1;
			}

	/* check collision with block */
	for(i = 0; i < BLOCKCOUNT; i++)
		if(block[i].life == 1)
			if(block[i].y == y)
				if((block[i].x == x || block[i].x + 1 == x ||
					block[i].x == x + 1 || block[i].x + 1 == x + 1))
				{
					move(block[i].y, block[i].x);
					addstr(blank);
					block[i].life = 0;
					ball.direct = block_status[ball.direct];
					count++;
				}

	if(count != 0)
		return 1;

	/* check collision with edge */
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
		ball.x = bar.x[3];
		ball.y = bar.y - 1;

		move(LINES-1, ball.HP * 2 + 1);
		addstr(blank);
		ball.HP--;

		return 1;
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

	x_ball = 3;
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
		
		ball.x = bar.x[x_ball];
		move(ball.y, ball.x);
		addstr("●");
	}

	move(LINES-1, COLS-1);
	refresh();
}

int search(int end, int x, int y)
{
	int i;

	for(i = 0; i < end; i++)
		if(block[i].y == y)
			if(block[i].x == x || (block[i].x + 1) == x ||
				block[i].x == x + 1 || (block[i].x + 1) == x + 1)
				return 1;
	
	return 0;
}

void init_block(int count)
{
	int x, y, i;

	srand((unsigned int)time(NULL));

	for(i = 0; i < count; i++)
	{
		block[i].life = 1;

		while(1)
		{
			x = rand() % 79;
			y = rand() % 11;

			if(search(i, x, y) == 0)
			{
				block[i].x = x;
				block[i].y = y;
				break;
			}
		}
	}
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
