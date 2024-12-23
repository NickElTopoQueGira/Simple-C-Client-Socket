// stringa magica per compilare
// gcc -o main.exe main.c -lws2_32

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    WSADATA wsa;
    SOCKET sockfd;
    struct sockaddr_in serv_addr;
    char buffer[256];
    char *address = "172.20.10.5"; // Server IP address
    int port = 500; // Server port

    // Initialize Winsock
    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0) {
        printf("Failed. Error Code : %d",WSAGetLastError());
        return 1;
    }
    printf("Initialised.\n");

    // Create socket
    if((sockfd = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET) {
        printf("Could not create socket : %d" , WSAGetLastError());
    }
    printf("Socket created.\n");

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = inet_addr(address);

    // Connect to server
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        error("Connection Failed");
    }

    printf("Connected\n");

    // Read from stdin and send to server
    while (1) {
        memset(buffer, 0, 256);
        printf("Enter message: ");
        fgets(buffer, 255, stdin);

        // Remove newline character from the input
        buffer[strcspn(buffer, "\n")] = 0;

        // Append newline to ensure message is sent
        strcat(buffer, "\n");

        if (send(sockfd, buffer, strlen(buffer), 0) < 0) {
            error("ERROR writing to socket");
        }

        if (strcmp(buffer, "Over\n") == 0) {
            break;
        }
    }

    // Close the connection
    closesocket(sockfd);
    WSACleanup();
    return 0;
}
