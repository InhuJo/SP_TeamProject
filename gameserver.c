/* timeserv.c - a socket-based time of day server
 */
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#define PORTNUM 13000 
#define HOSTLEN 256
#define oops(msg)	{ perror(msg); exit(1); }

int topnum=0;
typedef struct result
{
	char name[10];
	int lifenum;
	int second;
	int minute;

}result;

result top[10];
void sort();

int main(int ac, char *av[])
{
	struct sockaddr_in saddr;	/* build our address here	*/
	struct hostent *hp;		/* this part of our		*/
	char hostname[HOSTLEN];		/* address			*/
	int sock_id, sock_fd;		/* line id, file descriptor	*/
	FILE *sock_fp;			/* use socket as steam		*/
	char *ctime();			/* convert secs to string	*/
	time_t thetime;	/* the time we report		*/
	result user;
	int i,j;

	for(i=0;i<10;i++)
	{
		strcpy(top[i].name,"----------");
		top[i].lifenum = 0;
		top[i].second = 0;
		top[i].minute = 0;

	}
	sock_id = socket(PF_INET,SOCK_STREAM,0);
	if(sock_id == -1)
		oops("socket");

	bzero((void *)&saddr,sizeof(saddr));

	gethostname(hostname,HOSTLEN);
	hp = gethostbyname(hostname);

	bcopy((void *) hp->h_addr,(void *)&saddr.sin_addr, hp->h_length);
	saddr.sin_port = htons(PORTNUM);
	saddr.sin_family = AF_INET;

	if(bind(sock_id, (struct sockaddr *) &saddr,sizeof(saddr))!=0)
		oops("bind");

	if(listen(sock_id,10)!=0)
		oops("listen");

	while(1)
	{
		sock_fd = accept(sock_id,NULL,NULL);
		//printf("Wow get a call\n");
		
		if(sock_fd == -1)
			oops("accept");

		sock_fd = fdopen(sock_fd,"r");
		if(sock_fd == NULL)
			oops("fdopen");

		read(sock_fd,(void*)&user,sizeof(result));

		if(topnum<10)
		{
		
			//no data as many as 10
			strcpy(top[topnum].name,user.name);
			top[topnum].lifenum = user.lifenum;
			if(user.second>=60)
			{
				top[topnum].minute= user.second/60;
				top[topnum].second = user.second - (top[topnum].minute * 60);

			}

			sort();
			
			write(sock_fd,(void *)top,sizeof(result)*10);
			topnum++;
		}
		else
		{
			//there are data over 10
			//compare top 10 data

			for(i=0;i<10;i++)
			{
				if((top[i].minute * 60) + top[i].second > user.second)
				{
					for(j = i;j<9;j++)
					{
						top[j+1] = top[j]; 
					}
					top[i] = user;
					if(top[i].second>=60)
					{
						top[i].minute = top[i].second/60;
						top[i].second = top[i].second -(top[i].minute * 60);
					}

					write(sock_fd,(void *)top,sizeof(result)*10);

				}

			}

		}

	
		fclose(sock_fp);
	}

	
}
void sort()
{
	int i,j,min;
	result temp;

	for(i=0;i<topnum-1;i++)
	{
		min =i;

		for(j=i+1;j<topnum;j++)
		{
			if(top[j].minute*60 + top[j].second < top[i].minute*60 + top[i].second)
				min = j;

		}
		temp = top[i];
		top[i] = top[min];
		top[min] = temp;
		
	}
}
