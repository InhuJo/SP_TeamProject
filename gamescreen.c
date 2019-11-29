#include<stdio.h>
#include<ncursesw/curses.h>
#include<time.h>
#include<stdlib.h>
#include<locale.h>
int arr[30][5];
int main()
{
	setlocale(LC_ALL,"");
	int i,j;
	char stone[] = "□";

	initscr();
	clear();

	srand(time(NULL));
	for(i=0;i<30;i++)
	{
		for(j=0;j<5;j++)
		{
			arr[i][j] = rand()%2;
		}
	}


	for(i=0;i<30;i++)
	{
		for(j=0;j<5;j++)
		{
			if(arr[i][j]==1)
			{
				
				mvprintw(j,i,stone);
			}
			
		}
	}
	refresh();
	sleep(9);
	endwin();
}
