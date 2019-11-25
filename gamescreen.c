#include<stdio.h>
#include<ncursesw/curses.h>
#include<time.h>
#include<stdlib.h>
#include<locale.h>
int arr[10][20];
int main()
{
	setlocale(LC_ALL,"ko_KR.utf8");
	int i,j;
	char stone[] = "안";

	initscr();
	clear();

	srand(time(NULL));
	for(i=0;i<10;i++)
	{
		for(j=0;j<20;j++)
		{
			arr[i][j] = rand()%2;
		}
	}


	for(i=0;i<10;i++)
	{
		for(j=0;j<20;j++)
		{
			if(arr[i][j]==1)
			{
				
				mvprintw(j,i,stone);
			}
			
		}
	}
	refresh();
	sleep(10);
	endwin();
}
