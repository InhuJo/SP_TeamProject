#include <stdio.h>
#include <ncursesw/curses.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <termios.h>
#include <locale.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

#define titlerow 4
#define titlecol 11
#define menurow 15
#define menucol 33
#define LEFTEDGE 0
#define RIGHTEDGE 78
#define TOPEDGE 0
#define DOWNEDGE 22
#define BLOCKCOUNT 1
#define PORTNUM 12000
#define oops(x,y) { perror(x); exit(y); }
#define swap(x,y,z) z=x, x=y, y=z

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

typedef struct
{
	char user[20];
	double time;
}score;

/* function and variable for ball */
Ball ball;
int x_ball;
void init_ball();
int update_xball();
void update_ball();
int check_collision(int, int);
int set_ticker(int n_msecs);

/* function and variable for bar */
Bar bar;
void init_bar();
void update_bar();

/* function and varibale for block */
Block block[BLOCKCOUNT];
int block_count = 0;
int search(int, int, int);
void init_block(int);

/* function for terminal */
void set_crmode(void);
void tty_mode(int);

/* function for time */
struct timeval start_time, end_time;
double operating_time;

char *blank = " ";
int game_status = 1;

void start_screen(void);
void game_play(void);
void game_rule(void);
void game_end(void);
void win_screen(void);
void lose_screen(void);
void rank_screen(void);
void rank_screen2(void);
void sort(score*, int);

int main(void)
{
	setlocale(LC_ALL,"");
	tty_mode(0);
	set_crmode();

	initscr();
	clear();
	start_color();
	init_pair(0, COLOR_WHITE, COLOR_BLACK);
	init_pair(1, COLOR_RED, COLOR_RED);  
	init_pair(2, COLOR_GREEN, COLOR_GREEN);
	init_pair(3, COLOR_YELLOW, COLOR_YELLOW);
	init_pair(4, COLOR_BLUE, COLOR_BLUE);
	init_pair(5, COLOR_MAGENTA, COLOR_MAGENTA);
	init_pair(6, COLOR_CYAN, COLOR_CYAN);
	init_pair(7, COLOR_WHITE, COLOR_WHITE);

	start_screen();

	operating_time = 
		(double)(end_time.tv_sec) + 
		(double)(end_time.tv_usec) / 1000000.0 - 
		(double)(start_time.tv_sec) + 
		(double)(start_time.tv_usec) / 1000000.0;

	signal(SIGALRM, SIG_IGN);

	if(ball.HP <= 0)
	{
		lose_screen();
	}
	else
	{
		win_screen();	
	}
	endwin();
	tty_mode(1);
	return 0;
}

void lose_screen(void)
{
	int i;

	clear();
	move(titlerow+7, titlecol+20);
	attron(COLOR_PAIR(0));
	addstr("   IT IS OKAY!");

	//Y
	move(titlerow, titlecol);
	attron(COLOR_PAIR(5));
	addstr("  ");
	move(titlerow+1, titlecol+1);	
	addstr("  ");
	move(titlerow+2, titlecol+2);
	addstr("  ");

	move(titlerow+2, titlecol+4);
	addstr("  ");
	move(titlerow+1, titlecol+5);
	addstr("  ");
	move(titlerow, titlecol+6);
	addstr("  ");

	for(i = titlerow+2; i<titlerow+5; i = i+1)
	{
		move(i, titlecol+3);
		addstr("  ");
	}

	//O
	move(titlerow, titlecol+8);
	attron(COLOR_PAIR(6));
	addstr("       ");

	move(titlerow+4, titlecol+8);
	addstr("       ");

	for(i = titlerow ; i<titlerow+5;i = i+1)
	{
		move(i, titlecol+8);
		addstr("  ");
		move(i, titlecol+13);
		addstr("  ");
	}

	//U
	for(i = titlerow; i<titlerow+5; i=i+1)
	{
		move(i, titlecol+15);
		attron(COLOR_PAIR(7));
		addstr("  ");
		move(i, titlecol+20);
		addstr("  ");
	}
	move(titlerow+4, titlecol+17);
	addstr("     ");

	//L
	move(titlerow+4, titlecol+30);
	attron(COLOR_PAIR(1));
	addstr("     ");
	for(i = titlerow ; i<titlerow+5; i = i+1)
	{
		move(i, titlecol+28);
		addstr("  ");
	}

	//O
	move(titlerow, titlecol+35);
	attron(COLOR_PAIR(2));
	addstr("       ");

	move(titlerow+4, titlecol+35);
	attron(COLOR_PAIR(2));
	addstr("       ");

	for(i = titlerow ; i<titlerow+5;i = i+1)
	{
		move(i, titlecol+35);
		attron(COLOR_PAIR(2));
		addstr("  ");
		move(i, titlecol+40);
		attron(COLOR_PAIR(2));
		addstr("  ");
	}

	//S
	move(titlerow, titlecol+42);
	attron(COLOR_PAIR(3));
	addstr("       ");

	move(titlerow+2, titlecol+42);
	attron(COLOR_PAIR(3));
	addstr("       ");

	move(titlerow+4, titlecol+42);
	addstr("       ");

	move(titlerow+1,titlecol+42);
	addstr("  ");

	move(titlerow+3,titlecol+47);
	addstr("  ");

	//E
	for(i = titlerow ; i < titlerow + 5; i = i +1)
	{
		move(i, titlecol +49);
		attron(COLOR_PAIR(4));
		addstr("  ");
	}
	move(titlerow, titlecol+49);
	attron(COLOR_PAIR(4));
	addstr("      ");

	move(titlerow+2, titlecol+49);
	attron(COLOR_PAIR(4));
	addstr("      ");

	move(titlerow+4, titlecol+49);
	attron(COLOR_PAIR(4));
	addstr("      ");

	//!
	for(i = titlerow; i<titlerow+3; i = i+1)
	{
		move(i, titlecol +57);
		attron(COLOR_PAIR(6));
		addstr("  ");
	}
	move(titlerow+4, titlecol+57);
	attron(COLOR_PAIR(6));
	addstr("  ");

	move(LINES-1, COLS-1);
	refresh();
	sleep(4);
}

void win_screen(void)
{
	int input, i;
	int sock_id;
	char buf[256], flag_buf[10];
	struct sockaddr_in c;

	if((sock_id = socket(PF_INET, SOCK_STREAM, 0)) == -1)
		oops("socket", 1);

	memset(&c, '\0', sizeof(c));
	c.sin_family = AF_INET;
	c.sin_port = htons(PORTNUM);
	c.sin_addr.s_addr = inet_addr("15.164.4.71");

	if(connect(sock_id, (struct sockaddr*)&c, sizeof(c)))
		oops("connect", 1);

	strcpy(flag_buf, "1");

	if(send(sock_id, flag_buf, strlen(flag_buf) + 1, 0) == -1)
		oops("send", 1);

	sprintf(buf, "%.3lf", operating_time);

	if(send(sock_id, buf, strlen(buf) + 1, 0) == -1)
		oops("send", 1);

	clear();
	move(titlerow+7, titlecol+20);
	attron(COLOR_PAIR(0));
	addstr("CONGRATULATION!");
	move(titlerow+8, titlecol+17);
	addstr("you broke all blocks!");
	move(titlerow +10, titlecol+22);
	standout();
	addstr("Show ranking");
	move(LINES-1, COLS-1);
	standend();

	//Y
	move(titlerow, titlecol+2);
	attron(COLOR_PAIR(1));
	addstr("  ");
	move(titlerow+1, titlecol+3);	
	addstr("  ");
	move(titlerow+2, titlecol+4);
	addstr("    ");

	move(titlerow+1, titlecol+7);
	addstr("  ");
	move(titlerow, titlecol+8);
	addstr("  ");

	for(i = titlerow+2; i<titlerow+5; i = i+1)
	{
		move(i, titlecol+5);
		addstr("  ");
	}

	//O
	move(titlerow, titlecol+10);
	attron(COLOR_PAIR(6));
	addstr("       ");

	move(titlerow+4, titlecol+10);
	addstr("       ");

	for(i = titlerow ; i<titlerow+5;i = i+1)
	{
		move(i, titlecol+10);
		addstr("  ");
		move(i, titlecol+15);
		addstr("  ");
	}

	//U
	for(i = titlerow; i<titlerow+5; i=i+1)
	{
		move(i, titlecol+17);
		attron(COLOR_PAIR(7));
		addstr("  ");
		move(i, titlecol+22);
		addstr("  ");
	}
	move(titlerow+4, titlecol+17);
	addstr("     ");

	//W
	for( i = titlerow ; i<titlerow+4; i = i+1)
	{
		move(i, titlecol+32);
		attron(COLOR_PAIR(3));
		addstr("  ");

		move(i, titlecol+35);
		attron(COLOR_PAIR(3));
		addstr("  ");

		move(i, titlecol+38);
		attron(COLOR_PAIR(3));
		addstr("  ");

	}
	move(titlerow+4, titlecol+32);
	attron(COLOR_PAIR(3));
	addstr("         ");

	//I
	for( i = titlerow; i<titlerow+4; i= i+1)
	{
		move(i, titlecol+42);
		attron(COLOR_PAIR(5));
		addstr("  ");
	}
	move(titlerow, titlecol+40);
	attron(COLOR_PAIR(5));
	addstr("      ");
	move(titlerow+4, titlecol+40);
	attron(COLOR_PAIR(5));
	addstr("      ");

	//N
	for( i = titlerow ; i<titlerow+5; i = i+1)
	{
		move(i, titlecol+46);
		attron(COLOR_PAIR(4));
		addstr("  ");

		move(i, titlecol+51);
		addstr("  ");
	}
	move(titlerow+1, titlecol+48);
	addstr(" ");
	move(titlerow+2, titlecol+49);
	addstr(" ");
	move(titlerow+3, titlecol+50);
	addstr(" ");

	//!
	for(i = titlerow; i<titlerow+3; i = i+1)
	{
		move(i, titlecol +54);
		attron(COLOR_PAIR(6));
		addstr("  ");
	}
	move(titlerow+4, titlecol+54);
	addstr("  ");

	move(LINES-1, COLS-1);
	refresh();
	while(1)
	{
		input=getchar();
		if(input == 'f')
		{
			clear();
			rank_screen2();
		}
	}
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

	//menu
	move(menurow,menucol);
	standout();
	addstr("GAME START");
	cnt = menurow;

	move(menurow+2, menucol);
	standend();
	addstr(" ranking  ");

	move(menurow+1, menucol);
	addstr("game  rule");

	move(menurow+3, menucol);
	addstr(" game end ");
	move(LINES-1, COLS-1);
	refresh();

	while(1)
	{
		input = getchar();

		if(input == 's')
		{
			if(cnt == menurow)
			{
				mvdelch(menurow,menucol);
				standend();
				addstr("game start");
				move(menurow+1,menucol);
				standout();
				addstr("GAME  RULE");
				cnt = menurow+1;
			}
			else if(cnt == menurow+1)
			{
				mvdelch(menurow+1,menucol);
				standend();
				addstr("game  rule");
				move(menurow+2,menucol);
				standout();
				addstr(" RANKING  ");
				cnt = menurow+2;
			}
			else if(cnt == menurow+2)
			{
				mvdelch(menurow+2,menucol);
				standend();
				addstr(" ranking  ");
				move(menurow+3,menucol);
				standout();
				addstr(" GAME END ");
				cnt = menurow+3;
			}

			move(LINES-1, COLS-1);
			refresh();

		}

		if(input == 'w' )
		{
			if(cnt == menurow+3)
			{
				mvdelch(menurow+3,menucol);
				standend();
				addstr(" game end ");
				move(menurow+2 ,menucol);
				standout();
				addstr(" RANKING  ");
				cnt = menurow+2;
			}
			else if(cnt==menurow+2)
			{
				mvdelch(menurow+2,menucol);
				standend();
				addstr(" ranking  ");
				move(menurow+1,menucol);
				standout();
				addstr("GAME  RULE");
				cnt = menurow+1;
			}
			else if(cnt == menurow+1)
			{
				mvdelch(menurow+1, menucol);
				standend();
				addstr("game  rule");
				move(menurow, menucol);
				standout();
				addstr("GAME START");
				cnt = menurow;
			}
			move(LINES-1, COLS-1);
			refresh();

		}
		else if(input == 'f')  //enter키
		{

			if(cnt == menurow) //game start
			{
				gettimeofday(&start_time, NULL);
				game_play();
				gettimeofday(&end_time, NULL);
				break;
			}
			else if(cnt == menurow+1)
				game_rule();
			else if(cnt == menurow+2)
				rank_screen();
			else
				game_end();
		}
	}
}

void rank_screen(void)
{
	int input, size, i = 0, j = 0;
	int row = 5, col = 25;
	int sock_id;
	char temp[BUFSIZ], flag_buf[10], t[10];
	char *buf;
	struct sockaddr_in c;
	score s[100];

	if((sock_id = socket(PF_INET, SOCK_STREAM, 0)) == -1)
		oops("socket", 1);

	memset(&c, '\0', sizeof(c));
	c.sin_family = AF_INET;
	c.sin_port = htons(PORTNUM);
	c.sin_addr.s_addr = inet_addr("15.164.4.71");

	if(connect(sock_id, (struct sockaddr*)&c, sizeof(c)))
		oops("connect", 1);

	strcpy(flag_buf, "0");

	if(send(sock_id, flag_buf, strlen(flag_buf) + 1, 0) == -1)
		oops("send", 1);

	if((size = recv(sock_id, temp, sizeof(temp), 0)) < 0)
		oops("recv", 1);

	buf = malloc(size);
	memset(buf, 0, size);
	strcpy(buf, temp);

	clear();
	standend();

	move(3, 22);
	addstr("-------------Ranking-------------");
	while(strcmp(buf, "\0"))
	{
		sscanf(buf, "%s %s", s[i].user, t);
		size = strlen(s[i].user) + strlen(t) + 2;
		s[i].time = atof(t);
		buf = buf + size;
		i++;
	}
	sort(s, i);

	for(j = 0; j < i; j++)
	{
		move(row+j, col);
		printw("%d. %13s %10.3lf", j+1, s[j].user, s[j].time);
	}
	move(LINES-1, COLS-1);

	refresh();
	close(sock_id);

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
void rank_screen2(void)
{
	int input, size, i = 0, j = 0;
	int row = 5, col = 25;
	int sock_id;
	char temp[BUFSIZ], flag_buf[10], t[10];
	char *buf;
	struct sockaddr_in c;
	score s[100];

	if((sock_id = socket(PF_INET, SOCK_STREAM, 0)) == -1)
		oops("socket", 1);

	memset(&c, '\0', sizeof(c));
	c.sin_family = AF_INET;
	c.sin_port = htons(PORTNUM);
	c.sin_addr.s_addr = inet_addr("15.164.4.71");

	if(connect(sock_id, (struct sockaddr*)&c, sizeof(c)))
		oops("connect", 1);

	strcpy(flag_buf, "0");

	if(send(sock_id, flag_buf, strlen(flag_buf) + 1, 0) == -1)
		oops("send", 1);

	if((size = recv(sock_id, temp, sizeof(temp), 0)) < 0)
		oops("recv", 1);

	buf = malloc(size);
	memset(buf, 0, size);
	strcpy(buf, temp);

	clear();
	standend();

	move(3, 22);
	addstr("-------------Ranking-------------");
	while(strcmp(buf, "\0"))
	{
		sscanf(buf, "%s %s", s[i].user, t);
		size = strlen(s[i].user) + strlen(t) + 2;
		s[i].time = atof(t);
		buf = buf + size;
		i++;
	}
	sort(s, i);

	for(j = 0; j < i; j++)
	{
		move(row+j, col);
		printw("%d. %13s %10.3lf", j+1, s[j].user, s[j].time);
	}
	move(LINES-1, COLS-1);

	refresh();
	close(sock_id);

	while(1)
	{
		input = getchar();
		if(input == 'f')
		{
			clear();
			game_end();
		}
	}
}

void sort(score *s, int n)
{
	int i, j, least;
	score temp;

	for(i = 0; i < n - 1; i++)
	{
		least = i;

		for(j = i + 1; j < n; j++)
			if(s[j].time < s[least].time)
				least = j;

		if(i != least)
			swap(s[i], s[least], temp);
	}
}

void game_play(void)
{
	int input, i;

	clear();
	standend();

	init_ball();
	init_bar();

	signal(SIGALRM, update_ball);

	if(set_ticker(80) == -1)
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
		if(block_count == BLOCKCOUNT)
			break;

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
	//return;
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
					block_count++;
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
	char input, i;

	clear();
	standend();

	move(3, 20);
	addstr("--------------HOW TO PLAY--------------");

	move(5, 30);
	addstr("w: 위 메뉴로 이동");
	move(6, 29);
	addstr("s: 아래 메뉴로 이동");
	move(7, 32);
	addstr("f: 메뉴 선택");

	move(9, 28);
	addstr("a: 막대 왼쪽으로 이동");
	move(10,27);
	addstr("d: 막대 오른쪽으로 이동");

	move(12, 33);
	addstr("h: 공잡기");
	move(13, 27);
	addstr("j: 왼쪽 대각선으로 공 발사");
	move(11, 29);
	addstr("k: 위쪽으로 공 발사");
	move(12, 26);
	addstr("l: 오른쪽 대각선으로 공 발사");

	move(15, 20);
	addstr("---------------------------------------");

	move(22, 36);
	standout();
	addstr("BACK");
	standend();
	move(LINES-1, COLS-1);
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
