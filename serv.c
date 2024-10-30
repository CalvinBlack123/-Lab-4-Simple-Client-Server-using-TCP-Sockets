#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "linked_list.h"  // Assuming your Linked List library is included here

#define PORT 8080
#define ACK "ACK: "
#define MAX_BUFFER 1024

// Function prototypes for handling server operations
void handleClient(int clientSocket, list_t *mylist);

int main(int argc, char const *argv[]) {
    int servSockD, clientSocket;
    struct sockaddr_in servAddr;

    // Initialize the linked list
    list_t *mylist = list_alloc();

    // Create server socket
    servSockD = socket(AF_INET, SOCK_STREAM, 0);
    if (servSockD < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Define server address
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(PORT);
    servAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket to IP and port
    if (bind(servSockD, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
        perror("Bind failed");
        close(servSockD);
        exit(EXIT_FAILURE);
    }

    // Listen for client connections
    if (listen(servSockD, 1) < 0) {
        perror("Listen failed");
        close(servSockD);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    // Accept client connection
    clientSocket = accept(servSockD, NULL, NULL);
    if (clientSocket < 0) {
        perror("Client connection failed");
        close(servSockD);
        exit(EXIT_FAILURE);
    }

    // Handle client requests
    handleClient(clientSocket, mylist);

    // Close sockets and free memory
    close(clientSocket);
    close(servSockD);
    list_free(mylist);  // Free the linked list

    return
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "linked_list.h"  // Assuming your Linked List library is included here

#define PORT 8080
#define ACK "ACK: "
#define MAX_BUFFER 1024

// Function prototypes for handling server operations
void handleClient(int clientSocket, list_t *mylist);

int main(int argc, char const *argv[]) {
    int servSockD, clientSocket;
    struct sockaddr_in servAddr;

    // Initialize the linked list
    list_t *mylist = list_alloc();

    // Create server socket
    servSockD = socket(AF_INET, SOCK_STREAM, 0);
    if (servSockD < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Define server address
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(PORT);
    servAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket to IP and port
    if (bind(servSockD, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
        perror("Bind failed");
        close(servSockD);
        exit(EXIT_FAILURE);
    }

    // Listen for client connections
    if (listen(servSockD, 1) < 0) {
        perror("Listen failed");
        close(servSockD);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    // Accept client connection
    clientSocket = accept(servSockD, NULL, NULL);
    if (clientSocket < 0) {
        perror("Client connection failed");
        close(servSockD);
        exit(EXIT_FAILURE);
    }

    // Handle client requests
    handleClient(clientSocket, mylist);

    // Close sockets and free memory
    close(clientSocket);
    close(servSockD);
    list_free(mylist);  // Free the linked list

    return 0;
}

void handleClient(int clientSocket, list_t *mylist) {
    char buf[MAX_BUFFER];
    char sbuf[MAX_BUFFER];
    int n, val, idx;
    char *token;

    while (1) {
        // Receive message from client
        n = recv(clientSocket, buf, sizeof(buf) - 1, 0);
        if (n <= 0) {
            break;
        }
        buf[n] = '\0';

        // Parse the command and arguments
        token = strtok(buf, " ");
        if (token == NULL) continue;

        if (strcmp(token, "exit") == 0) {
            sprintf(sbuf, "Server exiting. Goodbye!\n");
            send(clientSocket, sbuf, strlen(sbuf), 0);
            break;
        }
        else if (strcmp(token, "get_length") == 0) {
            val = list_length(mylist);
            sprintf(sbuf, "Length = %d", val);
        }
        else if (strcmp(token, "add_front") == 0) {
            token = strtok(NULL, " ");
            val = atoi(token);
            list_add_to_front(mylist, val);
            sprintf(sbuf, "%s%d", ACK, val);
        }
        else if (strcmp(token, "add_back") == 0) {
            token = strtok(NULL, " ");
            val = atoi(token);
            list_add_to_back(mylist, val);
            sprintf(sbuf, "%s%d", ACK, val);
        }
        else if (strcmp(token, "add_position") == 0) {
            token = strtok(NULL, " ");
            idx = atoi(token);
            token = strtok(NULL, " ");
            val = atoi(token);
            list_add_at_index(mylist, idx, val);
            sprintf(sbuf, "%s%d at index %d", ACK, val, idx);
        }
        else if (strcmp(token, "remove_back") == 0) {
            val = list_remove_from_back(mylist);
            sprintf(sbuf, "Removed from back: %d", val);
        }
        else if (strcmp(token, "remove_front") == 0) {
            val = list_remove_from_front(mylist);
            sprintf(sbuf, "Removed from front: %d", val);
        }
        else if (strcmp(token, "remove_position") == 0) {
            token = strtok(NULL, " ");
            idx = atoi(token);
            val = list_remove_at_index(mylist, idx);
            sprintf(sbuf, "Removed from position %d: %d", idx, val);
        }
        else if (strcmp(token, "get") == 0) {
            token = strtok(NULL, " ");
            idx = atoi(token);
            val = list_get_elem_at(mylist, idx);
            sprintf(sbuf, "Element at index %d: %d", idx, val);
        }
        else if (strcmp(token, "print") == 0) {
            char *list_str = listToString(mylist);
            snprintf(sbuf, MAX_BUFFER, "%s", list_str);
            free(list_str);  // Free the string returned by listToString
        }
        else {
            sprintf(sbuf, "Invalid command");
        }

        // Send response to client
        send(clientSocket, sbuf, strlen(sbuf), 0);

        // Clear the buffers
        memset(buf, 0, MAX_BUFFER);
        memset(sbuf, 0, MAX_BUFFER);
    }
}
