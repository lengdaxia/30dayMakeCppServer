#include <sys/socket.h>  // 套接字相关函数
#include <arpa/inet.h>   // 网络地址转换函数
#include <string.h>      // 字符串操作
#include "util.h"        // 自定义错误处理
#include <_stdio.h>      // 标准输入输出
#include <unistd.h>      // Unix 标准函数

int main() {
    // 第一步：创建客户端套接字
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // 第二步：设置服务器地址信息
    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));              // 清零地址结构
    serv_addr.sin_family = AF_INET;                    // 使用IPv4协议
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // 服务器IP地址
    serv_addr.sin_port = htons(8888);                  // 服务器端口号

    // 第三步：连接到服务器
    int result = connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    errif(result == -1, "socket connect error");

    // 第四步：与服务器通信的主循环
    while(true) {
        char buf[1024];  // 创建缓冲区
        bzero(&buf, sizeof(buf));  // 清空缓冲区

        // 从标准输入读取用户输入
        scanf("%s", buf);
        
        // 发送数据到服务器
        ssize_t write_bytes = write(sockfd, buf, sizeof(buf));
        if(write_bytes == -1) {
            printf("write error\n");
            break;
        }

        // 清空缓冲区，准备接收服务器响应
        bzero(&buf, sizeof(buf));

        // 读取服务器响应
        ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
        if(read_bytes == -1) {
            // 读取错误处理
            close(sockfd);
            printf("read error\n");
            break;
        } else if (read_bytes == 0) {
            // 服务器关闭连接
            printf("server closed\n");
            break;
        } else if (read_bytes > 0) {
            // 打印服务器响应
            printf("message from server: %s\n", buf);
        }
    }

    // 第五步：关闭连接
    close(sockfd);
    return 0;
}