#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MYPORT 8080
#define BACKLOG 1

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in myaddr;   //setup my port and IP
    myaddr.sin_family = AF_INET;
    myaddr.sin_port = htons(MYPORT);
    myaddr.sin_addr.s_addr = INADDR_ANY;
    memset(myaddr.sin_zero, '\0', 8);

    bind(sockfd, (struct sockaddr*)&myaddr, sizeof(struct sockaddr)); //bind all connections to this port and IP here
    listen(sockfd, BACKLOG);

    struct sockaddr_in theiraddr; //sockaddr_in is just like int
    socklen_t addrlen = sizeof(struct sockaddr_in);

    printf("Waiting for a connection...\n");
    int newfd = accept(sockfd, (struct sockaddr*)&theiraddr, &addrlen);   //new connection

    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &theiraddr.sin_addr, client_ip, INET_ADDRSTRLEN);   //convert IP from binary to normally used form
    printf("Got a connection from IP: %s on Port: %d\n", client_ip, ntohs(theiraddr.sin_port));  //ntohs required to solve endian problem

    send(newfd, "WELCOME\n", 8, 0); //send welcome message

    char buffer[1024] = {0}; 
    int bytes_received = recv(newfd, buffer, sizeof(buffer) - 1, 0); 

    if (bytes_received < 0) printf("ERROR OCCURRED\n");
    else if (bytes_received == 0) printf("CLIENT DISCONNECTED\n");
    else {
        printf("%d bytes received\n", bytes_received);
        printf("Client says: %s\n", buffer);
    }

    close(newfd);   //hang up on client
    close(sockfd);   //shut down server
    return 0;
}
