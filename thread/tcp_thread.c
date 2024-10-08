#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>

struct SockInfo
{
	int fd;					 // 通信
	pthread_t tid;			 // 线程ID
	struct sockaddr_in addr; // 地址信息
};

struct SockInfo infos[128]; // 最大支持128个客户端

void *working(void *arg)
{
	while (1)
	{
		struct SockInfo *info = (struct SockInfo *)arg;
		// 接收数据
		char buf[1024];
		int ret = read(info->fd, buf, sizeof(buf)); // 阻塞 IO
		if (ret == 0)
		{
			printf("客户端已经关闭连接...\n");
			info->fd = -1;
			break;
		}
		else if (ret == -1)
		{
			printf("接收数据失败...\n");
			info->fd = -1;
			break;
		}
		else
		{
			write(info->fd, buf, strlen(buf) + 1); // 回发数据
		}
	}
	return NULL;
}

int main()
{
	// 1. 创建用于监听的套接字
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == -1)
	{
		perror("socket");
		exit(0);
	}

	// 2. 绑定
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;		   // ipv4
	addr.sin_port = htons(8989);	   // 字节序应该是网络字节序
	addr.sin_addr.s_addr = INADDR_ANY; // == 0, 获取IP的操作交给了内核
	int ret = bind(fd, (struct sockaddr *)&addr, sizeof(addr));
	if (ret == -1)
	{
		perror("bind");
		exit(0);
	}

	// 3.设置监听
	ret = listen(fd, 100);
	if (ret == -1)
	{
		perror("listen");
		exit(0);
	}

	// 4. 等待, 接受连接请求
	int len = sizeof(struct sockaddr);

	// 数据初始化
	int max = sizeof(infos) / sizeof(infos[0]);
	for (int i = 0; i < max; ++i)
	{
		bzero(&infos[i], sizeof(infos[i])); // 重置
		infos[i].fd = -1;
		infos[i].tid = -1;
	}

	// 父进程监听, 子进程通信
	while (1)
	{
		// 创建子线程
		struct SockInfo *pinfo;
		for (int i = 0; i < max; ++i)
		{
			if (infos[i].fd == -1)
			{
				pinfo = &infos[i];
				break;
			}
			if (i == max - 1)
			{
				sleep(1);
				i--;
			}
		}

		int connfd = accept(fd, (struct sockaddr *)&pinfo->addr, &len);
		printf("parent thread, connfd: %d\n", connfd);
		if (connfd == -1)
		{
			perror("accept");
			exit(0);
		}
		pinfo->fd = connfd;
		pthread_create(&pinfo->tid, NULL, working, pinfo); // 创建线程
		pthread_detach(pinfo->tid);						   // 线程分离, 父线程不等待子线程结束
	}

	// 释放资源
	close(fd); // 监听

	return 0;
}