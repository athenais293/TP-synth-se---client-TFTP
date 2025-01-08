#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h> 
#include <unistd.h>

#define SERVER_PORT 1069
#define TFTP_RRQ 1 
#define BUFFER_SIZE 516

int main(int argc, char** argv) {
     printf("TFTP Client \n");

    //check the number of arguments to avoid undesirable behaviors such as segmentation faults
    if (argc != 4) {
        printf("Wrong command \n");
        exit(EXIT_FAILURE);
    }

    struct addrinfo hints, *result;
    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_INET;      
    hints.ai_protocol = IPPROTO_UDP; 
    hints.ai_socktype = SOCK_DGRAM;

    int status = getaddrinfo(argv[1], argv[2], &hints, &result);

    char ip_string[128] = {0};
    char port_string[128] = {0};

    //convert the address to a human-readable form
    getnameinfo(result->ai_addr, result->ai_addrlen,
                ip_string, 128, port_string, 128,
                NI_NUMERICHOST | NI_NUMERICSERV);

    printf("%s is resolved at : %s:%s\n", argv[1], ip_string, port_string);

    
    //ensure getaddrinfo succeeds, or it will affect socket creation later
    if (status != 0) {
        printf("Unable to reach host: %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

     //create a socket for communication
    int sd = socket(result->ai_family, result->ai_socktype,
                    result->ai_protocol);
    if (sd == -1) {
        perror("Socket creation failed");
        freeaddrinfo(result);
        exit(EXIT_FAILURE);
    }

    //RRQ message
    char rrq[BUFFER_SIZE] = {0};
    rrq[1] = TFTP_RRQ; 
    sprintf(rrq + 2, "%s", argv[3]);  
    sprintf(rrq + 3 + strlen(argv[3]), "octet"); 

     //send the RRQ message to the server
    if (sendto(sd, rrq, strlen(argv[3]) + 9, 0,
               result->ai_addr, result->ai_addrlen) == -1) {
        perror("Failed to send RRQ");
        freeaddrinfo(result);
        close(sd);
        exit(EXIT_FAILURE);
    }

     printf("RRQ sent to server for file: %s\n", argv[3]);

    freeaddrinfo(result);
    close(sd);

    return 0;
}