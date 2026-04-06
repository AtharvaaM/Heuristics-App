#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> 

#define SERVER_PORT 8080
#define SERVER_IP "127.0.0.1"

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in servaddr;
    memset(servaddr.sin_zero, '\0', 8);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &servaddr.sin_addr);

    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) 
    {
        perror("Connection failed");
        return 1;
    }
    printf("Connected to the server!\n");

    char buffer[1024] = {0};
    int bytes_recd = recv(sockfd, buffer, sizeof(buffer) - 1, 0); //receive welcome message
    if (bytes_recd > 0) 
    {
        printf("%d bytes received: %s", bytes_recd, buffer);
    }

    const char* message = "Hello Server"; //send hello message
    send(sockfd, message, strlen(message), 0);
    printf("Message sent\n");

    close(sockfd); //hang up
    return 0;
}
