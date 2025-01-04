#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h> 
#include <unistd.h>

#define SERVER_PORT 1069

int main(int argc, char** argv) {
    //verifier le nombre d'arguments pour éviter les comportements indésirables comme des erreurs de segmentation
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
    //vérifie que getaddrinfo n'echoue pas sinon le probleme se repercutera plus tard aec le socket par exemple
    if (status != 0) {
        printf("Unable to reach host: %s\n", host);
        exit(EXIT_FAILURE);
    }

    //configuration pour communiquer avec le bon serveur
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

    close(sfd);

    return 0;
}