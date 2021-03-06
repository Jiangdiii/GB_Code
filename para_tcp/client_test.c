#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>

#include <error.h>

#define MAXSIZE 100
#define SA struct sockaddr
#define LISTENQ 1024
int str_cli(int sockt_fd)
{
        char sendline[MAXSIZE];
        char receline[MAXSIZE];
        int n;
        printf("please input a string.\n");
        while(fgets(sendline,MAXSIZE,stdin) != NULL)
        {
                write(sockt_fd,sendline,strlen(sendline));
                printf("send:%s\n",sendline);
                printf("messages sended\n");
                n=read(sockt_fd,receline,MAXSIZE);
                if(n <= 0)
                {
                        printf("read failed\n");
                        exit(1);
                }
                printf("read:%s",receline);
                printf("please input a string.\n");
        }
}

int main(int argc, char **argv)
{
        int sock_fd;
        struct sockaddr_in service_addr;
        if(argc != 2)
        {
        printf("usage : ./client_test <ip_address>\n");
        return 1;
        }

        sock_fd = socket(AF_INET,SOCK_STREAM,0);
        if(sock_fd<0)
        {
        printf("socket error \n");
        return 1;
        }

        bzero(&service_addr,sizeof(service_addr));
        service_addr.sin_family = AF_INET;
        service_addr.sin_port = htons(9877);

        if(inet_pton(AF_INET,argv[1],&service_addr.sin_addr) <= 0)
        {
        printf("inet_pton error for %s",argv[1]);
        return 1;
        }

        if(connect(sock_fd,(SA *)&service_addr,sizeof(service_addr)) < 0)
        {
        printf("connect failed\n");
        return 1;
        }
        printf("connected\n");
        str_cli(sock_fd);
        close(sock_fd);
        return 0;
}

