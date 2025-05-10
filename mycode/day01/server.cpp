#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>

int main() {
    // ===== socket() 函数 =====
    // 创建一个套接字
    // AF_INET: 指定使用 IPv4 协议族
    // SOCK_STREAM: 指定使用 TCP 流式协议
    // 0: 使用默认协议
    // 返回值：文件描述符(fd)，代表新创建的套接字
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // ===== bind() 准备 =====
    // 创建并初始化服务器地址结构体
    struct sockaddr_in serv_addr;
    // 将结构体内存清零
    bzero(&serv_addr, sizeof(serv_addr));
    // 配置服务器信息：
    serv_addr.sin_family = AF_INET;                    // 使用 IPv4 协议
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // 设置服务器 IP 地址
    serv_addr.sin_port = htons(8888);                  // 设置端口号（转换为网络字节序）

    // ===== bind() 函数 =====
    // 将套接字与特定的 IP 地址和端口号绑定
    // sockfd: 要绑定的套接字文件描述符
    // (sockaddr*)&serv_addr: 指向存有服务器 IP 和端口信息的结构体
    // sizeof(serv_addr): 结构体的大小
    bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr));

    // ===== listen() 函数 =====
    // 将套接字设置为监听模式，准备接受客户端连接
    // sockfd: 要监听的套接字文件描述符
    // SOMAXCONN: 监听队列的最大长度（等待连接的队列）
    listen(sockfd, SOMAXCONN);

    // ===== accept() 准备 =====
    // 创建客户端地址结构体，用于存储客户端连接信息
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_len = sizeof(clnt_addr);
    // 清零客户端地址结构体
    bzero(&clnt_addr, sizeof(clnt_addr));

    // ===== accept() 函数 =====
    // 接受客户端的连接请求（阻塞函数）
    // sockfd: 监听的套接字文件描述符
    // (sockaddr*)&clnt_addr: 用于存储客户端地址信息的结构体
    // &clnt_addr_len: 客户端地址结构体的长度
    // 返回值：新的套接字文件描述符，用于与该客户端通信
    int clnt_sockfd = accept(sockfd, (sockaddr*)&clnt_addr, &clnt_addr_len);

    // 打印客户端连接信息
    // inet_ntoa(): 将网络字节序的 IP 地址转换为字符串
    // ntohs(): 将网络字节序转换为主机字节序
    printf("new client fd %d! IP: %s Port: %d\n", 
           clnt_sockfd, 
           inet_ntoa(clnt_addr.sin_addr), 
           ntohs(clnt_addr.sin_port));
    
    return 0;
}