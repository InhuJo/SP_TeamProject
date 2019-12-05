#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PORTNUM 12052
#define oops(x,y) { perror(x); exit(y); }

void sort(int n);

int main()
{
	FILE *f1 = fopen("score.txt", "r"), *f2 = fopen("score.txt", "a+");
	char *buf, flag_buf[10], recv_buf[10], temp[20];
	int size, num;
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

	if((nsock_id = accept(sock_id, (struct sockaddr*)&client, &clientlen)) == -1)
		oops("accept", 1);

	while(1)
	{
		if((nsock_id = accept(sock_id, (struct sockaddr*)&client, &clientlen)) == -1)
			oops("accept", 1);

		if(recv(nsock_id, flag_buf, sizeof(flag_buf), 0) == -1)
			oops("recv", 1);

		num = atoi(flag_buf);

		if(num == 0)
		{
			fseek(f1, 0, SEEK_END);
			size = ftell(f1);

			buf = malloc(size + 1);
			memset(buf, 0, size + 1);

			fseek(f1, 0, SEEK_SET);
			fread(buf, size, 1, f1);

			if(send(nsock_id, buf, size + 1, 0) == -1)
				oops("send", 1);
		}
		else if(num == 1)
		{
			if(recv(nsock_id, recv_buf, sizeof(recv_buf), 0) == -1)
				oops("recv", 1);

			time = atof(recv_buf);
			fprintf(f2, "%s %.3lf\n", inet_ntoa(client.sin_addr), time);
		}
	}

	close(sock_id);
	close(nsock_id);

	return 0;
}
