/* timeclnt.c - a client for timeserv.c
 *		usage: timeclnt hostname portnumber
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <stdlib.h>
#include <strings.h>

#define oops(msg)	{ perror(msg); exit(1); }

typedef struct result
{
	char name[10];
	int lifenum;
	int second;
	int minute;
}result;

result top[10];

void print_rank();
int main(int ac, char *av[])
{
	struct	sockaddr_in servadd;	/* the number to call	*/
	struct	hostent *hp;		/* used to get number	*/
	int	sock_id/*, sock_fd*/;	/* the socket and fd	*/
	char	message[BUFSIZ];	/* to receive message	*/
	int	messlen;		/* for message length	*/
	result myresult;
	
	/*
	 * Step 1: Get a socket
	 */

	//random data 
	//확인용
	myresult.name = "오주영";
	myresult.lifenum = 3;
	myresult.second = 1000;

	//확인용

	sock_id = socket( AF_INET, SOCK_STREAM, 0);	/* get a line */
	if( sock_id == -1)
		oops( "socket" );			/* or fail */

	/* 
	 * Step 2: connect to server
	 *	   need to build address ( host, port ) of server first
	 */
	bzero ( &servadd, sizeof(servadd) ); 	/* zero the address */
	hp = gethostbyname(av[1]);		/* lookup host's ip # */
	if ( hp == NULL)
		oops(av[1]);
	bcopy( hp->h_addr, (struct sockaddr *)&servadd.sin_addr, hp->h_length);
	servadd.sin_port = htons(atoi(av[2]));	/* fill in port number */
	servadd.sin_family = AF_INET;		/* fill in socket type */

						/* now dial */
	if(connect(sock_id, (struct sockaddr *)&servadd, sizeof(servadd)) != 0 )
		oops( "bind" );

	/* 
	 * Step 3: transfer data from server, then hangup
	 */

	write(sock_id,(void *)&myresult,sizeof(result));

	messlen = read(sock_id,(void *)top,sizeof((result)*10);	/* read stuff	*/
	if ( messlen == -1 )
		oops( "read" );
	if( write(1, message, messlen ) != messlen )	/* and write to */
		oops( "write" );			/* stdout	*/
	close( sock_id );
}
void print_rank()
{
	int i;
	printf("rank  name  minute second lifenum\n");
	for(i=0;i<10;i++)
	{

		printf("%d  %s  %d  %d  %d\n",i+1,top[i].name,top[i].minute,top[i].second,top[i].lifenum);
	}


}
