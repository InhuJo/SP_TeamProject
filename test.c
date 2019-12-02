#include <stdio.h>
#include <ncursesw/curses.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <locale.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <signal.h>

#define titlerow 4
#define titlecol 11
#define menurow 15
#define menucol 33
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
void tty_mode(int);
void set_nodelay_mode(void);
void set_crmode(void);
void start_screen(void);
void game_play(void);
void game_rule(void);
void game_end(void);
void result_screen(void);

int main(void)
{
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
	result_screen();
	return 0;
}
void result_screen(void)
{
	clear();
	move(titlerow, titlecol);
	attron(COLOR_PAIR(1));
	addstr("We win");
	tty_mode(1);
	endwin();
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
		else if(input == 'f')  //enter키
		{

			if(cnt == menurow) //game start
			{
				game_play();
				break;
			}
			else if(cnt == menurow+1)
				game_rule();
			else
				game_end();
		}
	}
	//return cnt;
}
void game_play(void)
{
	int input, i;
	clear();
	init_ball();
	init_bar();

	signal(SIGALRM, update_ball);

	if(set_ticker(70) == -1)
		perror("set_ticker");

	init_block(BLOCKCOUNT);
	/* init first game screen */
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
			case 'k':
				if(ball.ready == 0)
				{
					ball.direct = TOP;
					ball.ready = 1;
				}
				break;
			case 'j':
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
void game_rule(void)
{
	clear();
	char input;
	move(LINES/2, COLS/2-40);
	addstr(" 이 게임은 공을 튀기어 벽돌을 깨는 프로그램입니다");
	move(LINES/2+1, COLS/2-40);
	addstr("막대는 a,d로 좌우로 움직일 수 있습니다");
	move(LINES/2+2, COLS/2-40);
	addstr("공은 h키로 잡을 수 있고, j는 왼쪽 대각선, k는 위로, l은 오른쪽 대각선으로 공을 튀깁니다");
	move(LINES/2+3, COLS/2-40);
	addstr("라이프는 3개이며, 모두 없어질 시에 게임이 끝이 납니다");

	move(LINES/2+10, COLS/2-20);
	addstr("BACK");
	refresh();
	while(1)
	{
		input = getchar();
		if(input == 'f')
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

