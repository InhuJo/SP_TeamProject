#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PORTNUM 12000
#define oops(x,y) { perror(x); exit(y); }

void sort(int n);

int main()
{
	FILE *f1, *f2;
	char *buf1, *buf2, flag_buf[10], recv_buf[50], temp[20];
	int size1, size2, num;
	double time;
	struct sockaddr_in serv, client;
	int sock_id, nsock_id, clientlen = sizeof(client);


	if((sock_id = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		oops("socket", 1);

	memset((char*)&serv, '\0', sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_port = htons(PORTNUM);
	serv.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(sock_id, (struct sockaddr *)&serv, sizeof(serv)))
		oops("bind", 1);

	if(listen(sock_id, 5))
		oops("listen", 1);

	while(1)
	{
		if((nsock_id = accept(sock_id, (struct sockaddr*)&client, &clientlen)) == -1)
			oops("accept", 1);

		if(recv(nsock_id, flag_buf, sizeof(flag_buf), 0) == -1)
			oops("recv", 1);

		num = atoi(flag_buf);

		if(num == 0)
		{
			f1 = fopen("1.txt", "r");

			fseek(f1, 0, SEEK_END);
			size1 = ftell(f1);

			buf1 = malloc(size1 + 1);
			memset(buf1, 0, size1 + 1);

			fseek(f1, 0, SEEK_SET);
			fread(buf1, size1, 1, f1);

			if(send(nsock_id, buf1, size1 + 1, 0) == -1)
				oops("send", 1);

			fclose(f1);
		}
		else if(num == 1)
		{
			/*fseek(f2, 0, SEEK_END);
			size2 = ftell(f2);

			buf2 = malloc(size2 + 1);
			memset(buf2, 0, size2 + 1);

			fseek(f2,*/
			f2 = fopen("1.txt", "a+");

			recv(nsock_id, recv_buf, sizeof(recv_buf), 0);
			printf("%s\n", recv_buf);

			fprintf(f2, "%s %s\n", inet_ntoa(client.sin_addr), recv_buf);

			fclose(f2);
		}
	}
	close(sock_id);
	close(nsock_id);

	return 0;
}
