#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

// ===== 客户端程序主函数 =====
int main()
{
    // ===== socket() 函数 =====
    // 创建一个套接字
    // AF_INET: 指定使用 IPv4 协议族
    // SOCK_STREAM: 指定使用 TCP 流式协议
    // 0: 使用默认协议
    // 返回值：文件描述符(fd)，代表新创建的套接字
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // ===== connect() 准备 =====
    // 创建服务器地址结构体，用于指定要连接的服务器信息
    struct sockaddr_in serv_addr;
    // 将结构体内存清零
    bzero(&serv_addr, sizeof(serv_addr));
    // 配置服务器信息：
    serv_addr.sin_family = AF_INET;                    // 使用 IPv4 协议
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // 设置服务器 IP 地址为本地回环地址
    serv_addr.sin_port = htons(8888);                  // 设置服务器端口号（转换为网络字节序）

    // ===== connect() 函数 =====
    // 连接到服务器
    // sockfd: 客户端套接字文件描述符
    // (struct sockaddr *)&serv_addr: 指向服务器地址信息的结构体指针
    // sizeof(serv_addr): 地址结构体的大小
    // 该函数会阻塞直到连接成功或失败
    connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    
    return 0;
}