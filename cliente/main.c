#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 4242
#define LEN 4096
#define SERVER_ADDR "127.0.0.1"

int main( int argc, char *argv[])
{
    struct sockaddr_in server;
    /*descriptor client*/
    int sockfd;

    int len = sizeof(server);
    int slen;
    char ip[32];
    strcpy(ip,SERVER_ADDR);

    /*Receive buffer*/
    char buffer_in[LEN];
    /*Send buffer*/
    char buffer_out[LEN];

    fprintf(stdout,"Starting Client !!\n");


    /*Creates a socket for the client*/
    if((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1)
    {
        perror("Error on client socket: ");
        return EXIT_FAILURE;
    }
    fprintf(stdout,"Client socket created with fd: %d\n",sockfd);

    /*Defines the connection properties*/
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    inet_pton(AF_INET,ip,&server.sin_addr);              //server.sin_addr.s_addr = inet_addr(SERVER_ADDR);
    memset(server.sin_zero,0x0,8);

    /*Tries to connect to the server*/
    if(connect(sockfd,(struct sockaddr*)&server, len) == -1) /*sizeof(server) em len era melhor*/
    {
        perror("Can`t connect to server\n");
        return EXIT_FAILURE;

    }

    /*Receives the presentation message from the server*/
    if((slen = recv(sockfd,buffer_in,LEN,0))>0)
    {
        buffer_in[slen + 1] = '\0';
        fprintf(stdout,"Server say: %s\n",buffer_in);
    }


     /* Commuicate with the server until the exit message come*/

     while (1) {

        /* Zeroing the buffers */
        memset(buffer_in, 0x0, LEN);
        memset(buffer_out, 0x0, LEN);

        fprintf(stdout, "Say something to the server: ");
        fgets(buffer_out, LEN, stdin);

        /* Sends the read message to the server through the socket */
        send(sockfd, buffer_out, strlen(buffer_out), 0);

        /* Receives an answer from the server */
        slen = recv(sockfd, buffer_in, LEN, 0);
        printf("Server answer: %s\n", buffer_in);

        /* 'bye' message finishes the connection */
        if(strcmp(buffer_in, "bye") == 0)
            break;
    }

    /* Close the connection whith the server */
    close(sockfd);

    fprintf(stdout, "\nConnection closed\n\n");

    return EXIT_SUCCESS;
}
