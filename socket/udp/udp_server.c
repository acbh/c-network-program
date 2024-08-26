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
	addr.sin_port = htons(9999);
	addr.sin_addr.s_addr = INADDR_ANY;							// bind to any address
	int ret = bind(fd, (struct sockaddr *)&addr, sizeof(addr)); // bind to port 9999
	if (ret < 0)
	{
		perror("bind");
		exit(0);
	}

	char buf[1024];
	char ipbuf[64];				// ip address buffer
	struct sockaddr_in cliaddr; // client address
	int len = sizeof(cliaddr);

	while (1)
	{
		memset(buf, 0, sizeof(buf));
		int rlen = recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr *)&cliaddr, &len);
		printf("client ip: %s, port: %d\n",
			   inet_ntop(AF_INET, &cliaddr.sin_addr, ipbuf, sizeof(ipbuf)),
			   ntohs(cliaddr.sin_port));

		printf("client data: %s\n", buf);

		sendto(fd, buf, rlen, 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
	}

	close(fd);
	return 0;
}