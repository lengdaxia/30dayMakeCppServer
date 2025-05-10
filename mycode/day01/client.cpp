
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>


// create a client socket
int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // create a sockaddr_in structure to specify the address and port
    struct sockaddr_in serv_addr;
    // initialize the sockaddr_in structure to zero
    bzero(&serv_addr, sizeof(serv_addr));
    // set the address family to IPv4
    serv_addr.sin_family = AF_INET;
    // set the address to localhost ( 127.0.0.1 )
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    // set the port to 8888
    serv_addr.sin_port = htons(8888);

    // connect to the server
    connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    
    return 0;
}