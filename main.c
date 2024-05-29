#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len;
    ssize_t num_bytes_received;

    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Zero out server_addr and client_addr structures
    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));

    // Fill server information
    server_addr.sin_family = AF_INET; // IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket with the server address
    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("UDP server is listening on port %d...\n", PORT);

    addr_len = sizeof(client_addr);

    while (1) {
        // Receive message from client
        num_bytes_received = recvfrom(sockfd, (char *)buffer, BUFFER_SIZE, MSG_WAITALL, (struct sockaddr *)&client_addr, &addr_len);
        if (num_bytes_received < 0) {
            perror("Receive failed");
            break;
        }

        buffer[num_bytes_received] = '\0'; // Null-terminate the received string
        printf("Client : %s\n", buffer);

        // Echo the message back to the client
        if (sendto(sockfd, (const char *)buffer, num_bytes_received, MSG_CONFIRM, (const struct sockaddr *)&client_addr, addr_len) < 0) {
            perror("Send failed");
            break;
        }
    }

    close(sockfd);
    return 0;
}
