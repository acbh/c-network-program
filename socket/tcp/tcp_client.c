#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0)
	{
		perror("socket");
		exit(0);
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(10000);
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

	int ret = connect(fd, (struct sockaddr *)&addr, sizeof(addr));
	if (ret < 0)
	{
		perror("connect");
		exit(0);
	}
	int number = 0;
	while (1)
	{
		char buf[1024];
		sprintf(buf, "Hello, I am client %d\n", number++);
		write(fd, buf, strlen(buf) + 1);

		memset(buf, 0, sizeof(buf));
		int len = read(fd, buf, sizeof(buf));
		if (len > 0)
		{
			printf("Server: %s\n", buf);
		}
		else if (len == 0)
		{
			printf("Server closed the connection\n");
			break;
		}
		else
		{
			perror("read");
			break;
		}
		sleep(1);
	}
	close(fd);
	return 0;
}