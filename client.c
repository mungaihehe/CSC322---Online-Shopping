#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <conio.h>
#include <errno.h>


int main(int argc, char *argv[]) {
    WSADATA d;
    if(WSAStartup(MAKEWORD(2, 2), &d)) {
        fprintf(stderr, "Failed to initialize.\n");
        return 1;
    }
    if(argc < 3) {
        fprintf(stderr, "usage: tcp_client hostname port\n");
        return 1;
    }

    printf("Configuring remote address...\n");
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_DGRAM;
    struct addrinfo *peer_address;
    if(getaddrinfo(argv[1], argv[2], &hints, &peer_address)) {
        fprintf(stderr, "getaddrinfo() failed. (%d)\n", WSAGetLastError());
        return 1;
    }

    printf("Remote address is: ");
    char address_buffer[100];
    char service_buffer[100];
    getnameinfo(peer_address->ai_addr, peer_address->ai_addrlen, address_buffer, sizeof(address_buffer), service_buffer, sizeof(service_buffer), NI_NUMERICHOST);
    printf("%s %s\n", address_buffer, service_buffer);

    printf("Creating socket...\n");
    SOCKET socket_peer;
    socket_peer = socket(peer_address->ai_family, peer_address->ai_socktype, peer_address->ai_protocol);
    if(socket == INVALID_SOCKET) {
        fprintf(stderr, "socket() failed. (%d)\n", WSAGetLastError());
        return 1;
    }



    printf("Connected.\n");
    printf("Online shopping application. Select an action \n");
    printf(" 1 - Add Product \n 2 - Search Catalog \n 3 - Display catalog \n 4 - Catalog size \n 5 - Purchase Item (make payment) \n 6 - Exit \n");
    while(1) {
        fd_set reads;
        FD_ZERO(&reads);
        FD_SET(socket_peer, &reads);
        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 100000;
        if(select(socket_peer + 1, &reads, 0, 0, &timeout) < 0) {
            fprintf(stderr, "select() failed. (%d)\n", WSAGetLastError());
            return 1;
        }
        if(FD_ISSET(socket_peer, &reads)) {
            char read[4096];

            int bytes_received = recvfrom(socket_peer, read, 4096, 0, peer_address->ai_addr, (int*)&peer_address->ai_addrlen);

            printf("Received (%d bytes)\n", bytes_received);
            printf("%.*s\n", bytes_received, read);
            if(bytes_received < 0) {
                printf("Error occurred: %d\n", WSAGetLastError());
                return 1;
            }


        }
        if(_kbhit()) {
            char read[512];
            if(!fgets(read, 512, stdin)) break;
            printf("Sending: %s", read);
            int bytes_sent = sendto(socket_peer, read, 512, 0, peer_address->ai_addr, peer_address->ai_addrlen);
            printf("Sent %d bytes.\n", bytes_sent);
        }
    }
    freeaddrinfo(peer_address);
    printf("Closing socket...\n");
    closesocket(socket_peer);
    WSACleanup();
    printf("Finished.\n");
    return 0;
}
