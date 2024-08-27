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

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(9999); // listen port
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	int ret = bind(fd, (struct sockaddr *)&addr, sizeof(addr));
	if (ret < 0)
	{
		perror("bind");
		exit(0);
	}

	char buf[1024];
	int len = sizeof(buf);
	while (1)
	{
		memset(buf, 0, sizeof(buf));
		int ret = recvfrom(fd, buf, len, 0, NULL, NULL);
		printf("received data: %s\n", buf);
	}

	close(fd);

	return 0;
}