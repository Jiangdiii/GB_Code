# TCP/IP网络编程笔记 Part I
# 网络编程基础
## 什么是套接字？
Sockets allow communication between two different processes on the same or different machines. To be more precise, it's a way to talk to other computers/programs using standard Unix file descriptors. In early, Sockets were designed to implement the client-server model for interprocess communication.
### 套接字类型
#### Stream socket : 
After the connection has been established, data can be read from and written to these socets as a byte stream. The socket type is `SOCK_STREAM`.
* bidirectional （双向通信）
* reliable（可靠传输）
* sequenced(有序)
* unduplicated flow of data with no record boundaries（无数据边界）
#### Datagram sockset
The socket type is `SOCK_DGRAM`
* bidirectional（双向通信）
* unsequenced（乱序，非可靠）
* boundaries（数据边界，且存在传输数据大小限制）
## 如何创建套接字？
创建套接字通常是为了进行通信，我们以服务器-客户端为模型，简单阐述如何创建套接字？创建套接字都需要调用哪些函数？调用函数中有哪些需要注意的地方？
***
### 服务器端
1. 创建 套接字
2. 注册 服务器的IP及端口
3. 绑定 服务器IP和端口
4. 设置 监听端口
5. 等待 连接

以上就是创建服务器端套接字的5个步骤。
例程如下：
```c
//传入参数1 argv[1] 为端口号
#define BUF_SIZE 128

int serv_sock,clnt_sock;
struct sockaddr_in serv_add,clnt_addr;
chat buf[BUF_SIZE]={0};
socklen_t clnt_addr_size;

serv_sock=socket(PF_INET,SOCK_STREAM,0);

serv_addr.sin_familt=AF_INET;
serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
serv_addr.sin_port=htns(argv[1]);

bind(serv_sock,(struct sockaddr*)&serv_sock,sizeof(serv_addr);
listen(serv_sock,10);
accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_addr_size)
```
* 第8行代码   创建套接字  
* 第10-12行代码   填充套接字IP和端口信息。
* 第14行代码 给套接字绑定IP地址   
注：A remote process has no way to refer to a socket until an address is bound to it. Communicating processes are connected through addresses. The bind(3SOCKET) call allows a process to specify the local address of the socket
* 第15行代码   设置监听并规定最大等待队列  
注：specifies the maximum number of outstanding connections that might be queued.
第16行代码     负责处理连接的套接字，并存储客户端套接字信息。
***
### 客户端
1. 创建 套接字
2. 注册 待连接服务器的IP及端口
3. 连接 服务器

以上是创建客户端套接字的3个步骤

```c
int serv_sock;
struct sockaddr_in serv_addr;
char buf[BUF_SIZE];

serv_sock=socket(PF_INET,SOCK_STREAM,0);

serv_addr.sin_family=AF_INET;
serv_addr.sin_address.s_addr=inet_addr(argv[1]);
serv_addr.sin_port=htons(atoi(argv[2]));
connect(serv_sock,(struct sockaddr* )&serv_addr,sizeof(serv_addr));
```

## Q&A环节
### Q:套接字与文件描述符有何关系？为什么write,read等文件IO函数同样适用于套接字的读写？
>《Beej's Guide Network Programming》
>>you may have heard some Unix hacker state, “Jeez, everything in Unix is a file!” What that person may have been talking about is the fact that when Unix programs do any sort of I/O, they do it by reading or writing to a file descriptor. A file descriptor is simply an integer associated with an open file. But (and here’s the catch), that file can be a network connection, a FIFO, a pipe, a terminal, a real on-the-disk file, or just about anything else. Everything in Unix is a file! So when you want to communicate with another program over the Internet you’re gonna do it through a file descriptor, you’d better believe it. 
### Q:sockaddr_in 与 sockaddr 之间的关系？
the struct `sockaddr` holds socket address information for many types of sockets.
```
struct sockaddr {
        unsigned short    sa_family;    // address family, AF_xxx
        char              sa_data[14];  // 14 bytes of protocol address
    }; 
```
sa_family can be a variety of things, but it’ll be AF_INET (IPv4) or AF_INET6 (IPv6) for everything we do in this document. sa_data contains a destination address and port number for the socket. This is rather unwieldy since you don’t want to tediously pack the address in the sa_data by hand.

To deal with struct sockaddr, programmers created a parallel structure: struct sockaddr_in (“in” for “Internet”) to be used with IPv4.

And this is the important bit: a pointer to a struct `sockaddr_in` can be cast to a pointer to a struct sockaddr and vice-versa. So even though connect() wants a struct sockaddr*, you can still use a struct `sockaddr_in` and cast it at the last minute!
```
  // (IPv4 only--see struct sockaddr_in6 for IPv6)
    
    struct sockaddr_in {
        short int          sin_family;  // Address family, AF_INET
        unsigned short int sin_port;    // Port number
        struct in_addr     sin_addr;    // Internet address
        unsigned char      sin_zero[8]; // Same size as struct sockaddr
    };
```
This structure makes it easy to reference elements of the socket address. Note that `sin_zero` (which is included to pad the structure to the length of a struct `sockaddr`) should be set to all zeros with the function `memset()`. Also, notice that `sin_family` corresponds to sa_family in a struct `sockaddr` and should be set to `AF_INET`. Finally, the `sin_port` must be in Network Byte Order 
### Q:为网络字节序是什么？为什么填充套接字信息时需要转换字节序？转换字节序一般使用哪些函数？ 
字节序 指**Byte（字节）**存储在内存中的顺序。大端字节序指数据在内存中按照MSB到LSB的顺序存储，小端字节序指数据从LSB到MSB的顺序存储。
>if you want to represent the two-byte hex number, say b34f, you’ll store it in two sequential bytes b3 followed by 4f. This number, stored with the big end first, is called Big-Endian.  
However, a few computers scattered here and there throughout the world, namely anything with an Intel or Intel-compatible processor, store the bytes reversed, so b34f would be stored in memory as the sequential bytes 4f followed by b3. This storage method is called Little-Endian.

在计算机网络中，网络字节序指大端字节序，但是inter系列的处理器均为小端字节序。因此，在填充套接字信息时，需要将小端序转换为大端序。我们可以使用如下的函数进行字节序的转换：

端口字节序转换：
```
#include <arpa/inet.h>
```
|Function|Description|
|---|---|
|htons()|host to network short|
|htonl()|host to network long|
|ntohs()|network to host short|
|ntohs()|network to host long|
IP字符串信息转换：
```
#include <arpa/inet.h>
```
|Function|Description|
|---|---|
|in_addr_t inet_addr(const char * string)|将IP地址字符串转换为4字节数据,同时进行网络字节序转换|
|int aton(const char * string ,struct in_addr *addr)|将字符串地址转换为4字节数据，同时进行网络字节序转换。使用频率更高。|
|char * inet_ntoa(struct in_addr addr) |将网络字节序4字节IP地址转换为字符串形式|
注意：` char * inet_ntoa(struct in_addr addr)`函数返回值为char型指针，返回字符串地址意味着已保存到内存空间。也就是说调用完该函数应该立即将字符串信息复制到其他内存空间，否则再次调用会覆盖之前的值。
### Q:INADDR_ANY 是什么？
INADDR_ANY是一个常数，会默认分配local host的IP地址。
>In the above code, you could also assign INADDR_ANY to the s_addr field if you wanted to bind to your local IP address.
### Q:IP和域名之间如何转？通过什么函数可以做到？
IP2DNS
```
#include <sys/socket.h>
#include <netdb.h>
struct hostent *gethostbyaddr(const char *addr, int len, int type);
```
DNS2IP
```
#include <sys/socket.h>
#include <netdb.h>
struct hostent* gethostbyname(const char *hostname)
```
>n particular, gethostbyname() doesn’t work well with IPv6.
### Q:为什么需要半关闭套接字？通过什么实现？
>When you `close()` a socket descriptor, it closes both sides of the socket for reading and writing, and frees the socket descriptor. If you just want to close one side or the other, you can use this `shutdown() `call. It allows you to cut off communication in a certain direction, or both ways.  
It’s important to note that shutdown() doesn’t actually close the file descriptor—it just changes its usability. To free a socket descriptor, you need to use close().

```
#include <sys/socket.h>  
int shutdown(int socked, int how);
```
### Q:通常出现bind error时是什么导致的？
事实上，套接字不单单只是具备通信的作用，套接字还具备不同的特性。bind error 意味着地址分配错误，这往往有可能时套接字配置参数选项`SO_REUSEADDR`关联的time_out参数决定的。
情况如下：在服务器和客户端已经连接的情况下，服务器控制台突然终止。那服务器重新运行时就会出现`bind error`，并且服务器无法再次运行。因为4次握手结束后，服务器端还处于time-out阶段，避免最后一个ACK信号发送失败，占用的端口和地址还没有释放出来，

# Reference
For who want to know much more detail about network programming, You can read follow stuff.
* [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/)
* [TCP/IP网络编程](https://www.ituring.com.cn/book/1284)