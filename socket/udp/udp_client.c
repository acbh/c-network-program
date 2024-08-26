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

	struct sockaddr_in seraddr;
	seraddr.sin_family = AF_INET;
	seraddr.sin_port = htons(9999);
	inet_pton(AF_INET, "127.0.0.1", &seraddr.sin_addr);

	char buf[1024];
	char ipbuf[64];
	struct sockaddr_in cliaddr;
	int len = sizeof(cliaddr);
	int num = 0;

	while (1)
	{
		sprintf(buf, "hello, udp %d ...\n", num++);
		sendto(fd, buf, strlen(buf), 0, (struct sockaddr *)&seraddr, sizeof(seraddr));

		memset(buf, 0, sizeof(buf));
		recvfrom(fd, buf, sizeof(buf), 0, NULL, NULL);
		printf("server say: %s\n", buf);
		sleep(1);
	}

	close(fd);
	return 0;
}