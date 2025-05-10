#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/event.h> // 替换 sys/epoll.h
#include <errno.h>
#include "util.h"

#define MAX_EVENTS 1024
#define READ_BUFFER 1024

void setnonblocking(int fd){
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    errif(sockfd == -1, "socket create error");

    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);

    errif(bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)) == -1, "socket bind error");

    errif(listen(sockfd, SOMAXCONN) == -1, "socket listen error");

    // 创建 kqueue 实例
    int kq = kqueue();
    errif(kq == -1, "kqueue create error");

    struct kevent events[MAX_EVENTS], ev;

    // 注册监听套接字
    EV_SET(&ev, sockfd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
    if (kevent(kq, &ev, 1, NULL, 0, NULL) == -1) {
        errif(true, "kevent register error");
    }

    while(true) {
        int nev = kevent(kq, NULL, 0, events, MAX_EVENTS, NULL);
        errif(nev == -1, "kevent wait error");

        for(int i = 0; i < nev; i++) {
            int fd = events[i].ident;

            if(fd == sockfd) {
                // 处理新连接
                struct sockaddr_in clnt_addr;
                socklen_t clnt_addr_len = sizeof(clnt_addr);
                bzero(&clnt_addr, sizeof(clnt_addr));

                int clnt_sockfd = accept(sockfd, (sockaddr*)&clnt_addr, &clnt_addr_len);
                errif(clnt_sockfd == -1, "socket accept error");

                setnonblocking(clnt_sockfd);

                // 注册新客户端连接
                EV_SET(&ev, clnt_sockfd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
                if (kevent(kq, &ev, 1, NULL, 0, NULL) == -1) {
                    errif(true, "kevent register client error");
                }

                printf("new client fd %d! IP: %s Port: %d\n", 
                       clnt_sockfd, 
                       inet_ntoa(clnt_addr.sin_addr), 
                       ntohs(clnt_addr.sin_port));
            } else {
                // 处理客户端数据
                char buf[READ_BUFFER];
                bzero(&buf, sizeof(buf));
                ssize_t bytes_read = read(fd, buf, sizeof(buf));

                if(bytes_read > 0) {
                    printf("message from client fd %d: %s\n", fd, buf);
                    write(fd, buf, bytes_read);
                } else if(bytes_read == 0) {
                    printf("EOF, client fd %d disconnected\n", fd);
                    close(fd);
                }
            }
        }
    }
    close(sockfd);
    close(kq);
    return 0;
}
