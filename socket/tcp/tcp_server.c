#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
	int lfd = socket(AF_INET, SOCK_STREAM, 0);
	if (lfd == -1)
	{
		perror("socket");
		exit(0);
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(10000);
	addr.sin_addr.s_addr = INADDR_ANY;

	int ret = bind(lfd, (struct sockaddr *)&addr, sizeof(addr));
	if (ret == -1)
	{
		perror("bind");
		exit(0);
	}

	ret = listen(lfd, 128);
	if (ret == -1)
	{
		perror("listen");
		exit(0);
	}

	struct sockaddr_in client_addr;
	int client_len = sizeof(client_addr);
	int cfd = accept(lfd, (struct sockaddr *)&client_addr, &client_len);
	if (cfd == -1)
	{
		perror("accept");
		exit(0);
	}

	char ip[24] = {0};
	printf("client ip: %s, port: %d\n",
		   inet_ntop(AF_INET, &client_addr.sin_addr, ip, sizeof(ip)),
		   ntohs(client_addr.sin_port));

	while (1)
	{
		char buf[1024] = {0};
		memset(buf, 0, sizeof(buf));
		int len = read(cfd, buf, sizeof(buf));

		if (len > 0)
		{
			printf("recv: %s\n", buf);
			write(cfd, buf, len);
		}
		else if (len = 0)
		{
			printf("client close\n");
			break;
		}
		else
		{
			perror("read");
			break;
		}
	}
	close(cfd);
	close(lfd);
	return 0;
}