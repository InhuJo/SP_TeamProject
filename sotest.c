#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int ac, char* av[])
{
	struct sockaddr_in saddr;
	int sockfd; //socket descipter

