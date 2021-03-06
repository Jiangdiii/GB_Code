# TCP/IP网络编程笔记 Part III
# 网络编程进阶 2
## 套接字和IO标准？套接字如何使用IO函数读写？相较于系统函数有何优劣？
## I/O流如何分离？分离“流”的好处？
## 如何理解epoll？epoll相较于select的优点？
基于前面select的例程可以得知，select在程序中存在两点不合理的地方。
* 调用select函数后 针对所有的文件描述符进行遍历循环，不适用于监控过多的事件。
* 每次调用select函数都需要向该函数传递监控的对象。

epoll服务器端实现需要以下三个函数：
* epoll_create() 创建保存epoll文件描述符的空间
* epoll_ctl()   向空间注册并销毁文件描述符
* epoll_wait()  与select函数类似，等待文件描述符发生变化

```c
struct epoll_event{
    __uint32_t events;
    epoll_data_t data;
}

typedef union epoll_data{
    void * ptr;
    int fd;
    __uint32_t u32;
    __uint64_t u64;

}epoll_data_t;
```

```c
#include <sys/epoll.h>
int epoll_create(int size);
//--->成功是返回epoll文件描述符
int epoll_ctl(int epfd,int op,int fd,struct epoll_event *event);
/*
-->epfd 用于注册监控对象epoll例程的文件描述符
-->op 用于指定监控对象的添加，删除或者更改操作
----->EPOLL_CTL_ADD 将文件描述符注册到epoll例程
----->EPOLL_CTL_DEL 从epoll例程中删除文件描述符
----->EPOLL_CTL_MOD 更改注册的文件描述符的关注事件发生情况
-->fd 需要注册的监控对象文件描述符
-->event 监控对象的事件类型
EPOLLIN 读取数据的情况
EPOLLOUT    输出缓冲为空，可以立即发送数据的情况
EPOLLPRI    
EPOLLRDHUP
EPOLLERR
EPOLLET
EPOLLONESHOT
*/

struct epoll_event event;
event.events=EPOLLIN;
event.data.fd=sockfd;
epoll_ctl(epfd,EPOLL_CTL_ADD,sockfd,&event);


int epoll_wait(int epfd,struct epoll_event *events,int maxevents,int timeout);
/*
->epfd  表示事件发送监视范围 epoll例程的文件描述符
->保存发生事件的文件描述符集合的结构体地址值
->第二个参数中可以保存的最大时间数
-> 等待事件，若为-1则一直等待
*/
int event_cnt;
struct epoll_event *ep_events;
ep_events = malloc(sizeof(struct epoll_event)*EPOLL_SIZE);
event_cnt=epoll_wait(epfd,ep_events,EPOLL_SIZE,-1);

```


## 如何使用epoll？
## epoll的边缘触发和条件触发？？

## POSIX是什么？？
适用于计算机环境的可移植操作系统接口

## 线程与进程之间的关系？他们之间的差异是什么？
## 上下文切换 Context Switching 是什么意思？
## 线程的创建和执行流程？ 如何销毁进程？
## 临界区的概念？
## 多线程并发服务器如何实现?
