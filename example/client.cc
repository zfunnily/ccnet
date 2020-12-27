#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>

#include <errno.h>
#include <string.h>
#include <thread>
#include <mutex>
#include <chrono>

struct sockaddr_in addr;
void client()
{
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1)
    {
        fprintf(stderr, "socket error %s!!!\n", strerror(errno));
        exit(1);
    }

    int ret = connect(sock_fd,(struct sockaddr*)&addr,sizeof(struct sockaddr));
    if (ret == -1)
    {
        fprintf(stderr,"connect error %s\n", strerror(errno));
        exit(1);
    }

   std::mutex mutex_;
	char buff[1024] = {0};
	sprintf(buff, "%s", "helloworld\n");
     while(true)
	 {
		printf("send msg %s\n", buff);
		send(sock_fd, buff, strlen(buff),0);
		memset(buff,0,1024);

		recv(sock_fd,buff,sizeof(buff), 0);
		printf("recv msg %s\n", buff);
	 }
  
}

int main(int argc, char const *argv[])
{
	addr.sin_port = htons(8883);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_family = AF_INET;
	if (argc < 2)
	{
		printf("cmd len must >= 2. %d\n", argc);
		return -1;
	}
	const char* lne = argv[1];
	printf("开启client %d个\n", atoi(lne));
	std::thread t([&]{
			for(int i=0; i< atoi(lne); i ++)
			{
			client();
			}
			});
	t.join();


	std::this_thread::sleep_for(std::chrono::seconds(10000));
	return 0;
}
