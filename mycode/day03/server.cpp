#include <sys/socket.h>  // 套接字相关函数
#include <stdio.h>       // 标准输入输出
#include <arpa/inet.h>   // 网络地址转换函数
#include <string.h>      // 字符串操作
#include <unistd.h>      // Unix 标准函数
#include "util.h"        // 自定义错误处理函数

#define MAX_EVENTS 1024
#define READ_BUFFER 1024



int main() {
    // 第一步：创建套接字
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    // 第二步：准备服务器地址结构
    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));              // 清零内存
    serv_addr.sin_family = AF_INET;                    // IPv4协议
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // 本地回环地址
    serv_addr.sin_port = htons(8888);                  // 端口号8888
    
    // 第三步：绑定套接字与地址
    bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr));
    
    // 第四步：监听连接请求
    listen(sockfd, SOMAXCONN);  // SOMAXCONN为系统建议的最大值
    
    // 第五步：接受客户端连接
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_len = sizeof(clnt_addr);
    bzero(&clnt_addr, sizeof(clnt_addr));
    int clnt_sockfd = accept(sockfd, (sockaddr*)&clnt_addr, &clnt_addr_len);
    
    // 打印客户端信息
    printf("new client fd %d! IP: %s Port: %d\n", 
           clnt_sockfd, 
           inet_ntoa(clnt_addr.sin_addr),  // 转换IP地址为字符串
           ntohs(clnt_addr.sin_port));     // 转换端口为主机字节序
    
    // 第六步：与客户端通信循环
    while(true) {
        // 创建缓冲区
        char buf[1024];
        bzero(&buf, sizeof(buf));  // 清空缓冲区
        
        // 读取客户端数据
        ssize_t read_bytes = read(clnt_sockfd, buf, sizeof(buf));
        
        if(read_bytes > 0) {
            // 收到数据，打印并回显给客户端
            printf("message to client fd %d: %s\n", clnt_sockfd, buf);
            write(clnt_sockfd, buf, sizeof(buf));
        } else if(read_bytes == 0) {
            // 客户端断开连接
            printf("client fd %d disconnected\n", clnt_sockfd);
            close(clnt_sockfd);
            break;
        } else if(read_bytes == -1) {
            // 读取错误
            close(clnt_sockfd);
            errif(true, "socket read error");
        }
    }
    
    // 第七步：关闭服务器套接字
    close(sockfd);
    return 0;
}