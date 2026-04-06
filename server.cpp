#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>
#include <fcntl.h>
#include <time.h>   
#include <errno.h>

#define MYPORT 8080

//struct sockaddr_in
//sin_family
//sin_port
//struct in_addr sin_addr
//sin_zero
int main()
{
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  int newfd;
  socklen_t addrlen;
  addrlen = sizeof(struct sockaddr_in);

  struct sockaddr_in myaddr;
  struct sockaddr_in theiraddr;

  myaddr.sin_family = AF_INET;
  myaddr.sin_port = htons(MYPORT);
  myaddr.sin_addr.s_addr = INADDR_ANY;
  memset(myaddr.sin_zero, '\0', 8);

  bind(sockfd, (struct sockaddr*)&myaddr, sizeof(struct sockaddr_in));

  listen(sockfd, 1);
  newfd = accept(sockfd, (struct sockaddr*)&theiraddr, &addrlen);

  char client_ip[INET_ADDRSTRLEN];
  strcpy(client_ip,inet_ntoa(theiraddr.sin_addr));
  printf("Got a connection from IP: %s on port %d\n", client_ip, ntohs(theiraddr.sin_port));

  int bytes_sent = send(newfd, "WELCOME\n", strlen("WELCOME\n"), 0);

  char buffer[1024]; 
  int bytes_received;

  fcntl(newfd, F_SETFL, O_NONBLOCK);
  time_t start_time = time(NULL);

  while(1)
  {
    
    bytes_received = recv(newfd, buffer, sizeof(buffer) - 1, 0);

    if (bytes_received > 0) 
    {
      start_time = time(NULL); 
      buffer[bytes_received] = '\0'; 
      printf("Client says: %s", buffer);

      char message[1024];
      int len = sprintf(message, "Received %d bytes\n", bytes_received);
      send(newfd, message, len, 0);
    } 
    else if (bytes_received == 0) 
    {
      printf("The client hung up!\n");
      break;
    } 
    else 
    {
      if (errno != EAGAIN && errno != EWOULDBLOCK) 
      {
        perror("recv error");
        break;
      }
    }

    if (difftime(time(NULL), start_time) > 90) 
    {
      printf("Manual Timeout: No data for 90 seconds\n");
      send(newfd, "Timeout!\n Closing...\n", sizeof("Timeout!\n Closing...\n"), 0);
      break;
    }

    usleep(10000);
  }
}
