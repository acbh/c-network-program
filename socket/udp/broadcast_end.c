#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
	int fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0)
	{
		perror("socket");
		exit(0);
	}

	int opt = 1;
	setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt));

	char buf[1024];
	struct sockaddr_in cliaddr;
	int len = sizeof(cliaddr);
	cliaddr.sin_family = AF_INET;
	cliaddr.sin_port = htons(9999);							  // port number
	inet_pton(AF_INET, "255.255.255.255", &cliaddr.sin_addr); // broadcast address

	int num = 0;
	while (1)
	{
		sprintf(buf, "hello client %d\n", num++);
		sendto(fd, buf, strlen(buf) + 1, 0, (struct sockaddr *)&cliaddr, len);
		printf("send to broadcast: %s\n", buf);
		sleep(1);
	}

	close(fd);

	return 0;
}