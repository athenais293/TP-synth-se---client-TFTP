#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h> 
#include <unistd.h>

#define SERVER_PORT 1069
#define TFTP_RRQ 1 
#define BUFFER_SIZE 512

int main(int argc, char** argv) {
    //check the number of arguments to avoid undesirable behaviors such as segmentation faults
    if (argc != 3) {
        printf("Invalid number of arguments: expected 3, received %d\n", argc);
        exit(EXIT_FAILURE);
    }

    char* host = argv[1];
    char* file = argv[2];

    printf("Server: %s\n", host);
    printf("File: %s\n", file);

    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;      
    hints.ai_protocol = IPPROTO_UDP; 
    hints.ai_socktype = SOCK_DGRAM;

    int status = getaddrinfo(host, NULL, &hints, &res);
    //ensure getaddrinfo succeeds, or it will affect socket creation later
    if (status != 0) {
        printf("Unable to reach host: %s\n", host);
        exit(EXIT_FAILURE);
    }

    //configure the correct server
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = res->ai_family;
    serverAddr.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, host, &serverAddr.sin_addr.s_addr) <= 0) {
        printf("Invalid IP address: %s\n", host);
        exit(EXIT_FAILURE);
    }

    printf("Address resolution successful for host: %s\n", host);

    int sfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sfd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(res);

    //RRQ message
    char buffer[BUFFER_SIZE];
    int rrq_len = 2 + strlen(file) + 1 + strlen("octet") + 1;
    memset(buffer, 0, BUFFER_SIZE);
    buffer[0] = 0; 
    buffer[1] = TFTP_RRQ;  
    strcpy(&buffer[2], file); 
    strcpy(&buffer[2 + strlen(file) + 1], "octet");  

    if (sendto(sfd, buffer, rrq_len, 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Failed to send RRQ");
        close(sfd);
        exit(EXIT_FAILURE);
    }

    printf("RRQ sent to server for file: %s\n", file);

    close(sfd);

    return 0;
}