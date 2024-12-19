#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>

int main(int argc, char** argv) {
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

    int status = getaddrinfo(host, NULL, &hints, &res);
    if (status != 0) {
        printf("Unable to reach host: %s\n", host);
        exit(EXIT_FAILURE);
    }

    printf("Address resolution successful for host: %s\n", host);

    freeaddrinfo(res);

    return 0;
}