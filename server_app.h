#ifndef SERVER_APP_H_INCLUDED
#define SERVER_APP_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include "product.h"
#include "catalog.h"
#include <winsock2.h>
#include <ws2tcpip.h>

int send_and_await(SOCKET socket_client, const char* message, char* response) {
    int bytes_sent = send(socket_client, message, strlen(message), 0);
    int bytes_received = recv(socket_client, response, 1024, 0);
    printf("Bytes received: %d\n", bytes_received);
    printf("%.*s\n", bytes_received, response);
    return bytes_received;
}

void send_cli_options(SOCKET socket_client) {
    const char* response =
                    "Online shopping application. Select an action \n"
                    " 1 - Add Product \n 2 - Search Catalog \n 3 - Display catalog \n 4 - Catalog size \n 5 - Purchase Item (make payment) \n 6 - Exit \n";
    int bytes_sent = send(socket_client, response, strlen(response), 0);
    printf("Sent %d of %d bytes.\n", bytes_sent, (int)strlen(response));
}


int server_app(struct Catalog* catalog) {
    WSADATA d;
    if(WSAStartup(MAKEWORD(2, 2), &d)) {
        fprintf(stderr, "Failed to initialize.\n");
        return 1;
    }

    printf("Configuring local address...\n");
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    struct addrinfo *bind_address;
    getaddrinfo(0, "8080", &hints, &bind_address);

    printf("Creating a socket...\n");
    SOCKET socket_listen;
    socket_listen = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol);

    if(socket_listen == INVALID_SOCKET) {
        fprintf(stderr, "socket() failed. (%d)\n", WSAGetLastError());
        return 1;
    }

    printf("Binding socket to local address...\n");
    if(bind(socket_listen, bind_address->ai_addr, bind_address->ai_addrlen)) {
        fprintf(stderr, "bind() failed. (%d)\n", WSAGetLastError());
        return 1;
    }
    freeaddrinfo(bind_address);

    printf("Listening...\n");
    if(listen(socket_listen, 10) < 0) {
        fprintf(stderr, "listen() failed. (%d)\n", WSAGetLastError());
        return 1;
    }

    while(1) {
        printf("Waiting for connection...\n");
        struct sockaddr_storage client_address;
        socklen_t client_len = sizeof(client_address);
        SOCKET socket_client = accept(socket_listen, (struct sockaddr*) &client_address, &client_len);
        if(socket_client == INVALID_SOCKET) {
            fprintf(stderr, "accept() failed. (%d)\n", WSAGetLastError());
            return 1;
        }

        printf("Client is connected... ");
        char address_buffer[100];
        getnameinfo((struct sockaddr*) &client_address, client_len, address_buffer, sizeof(address_buffer), 0, 0, NI_NUMERICHOST);
        printf("%s\n", address_buffer);

        int exit = 0;
        while(exit != 1) {
            printf("Reading request...\n");
            char request[1024];
            int bytes_received = recv(socket_client, request, 1024, 0);
            printf("Received %d bytes.\n", bytes_received);
            printf("%.*s", bytes_received, request);

            printf("Sending response...\n");
            int i_request = atoi(request);
            printf("Selection: %d\n", i_request);
            switch(i_request) {
                case 0: ; {
                    send_cli_options(socket_client);
                    break;
                }
                case 1: ; {
                    struct Product* product = newProduct("", 0, "");
                    char product_name[20];
                    send_and_await(socket_client, "Enter the product's name: ", product_name);
                    strcpy(product->name, product_name);


                    char product_price[20];
                    send_and_await(socket_client, "Enter the product's price: ", product_price);
                    product->price = atoi(product_price);

                    char product_stock_string[20];
                    send_and_await(socket_client, "Enter the product's stock: ", product_stock_string);


                    struct CatalogItem* item = newCatalogItem(product, atoi(product_stock_string));
                    addToCatalog(item, catalog);

                    const char* response = "Product added to catalog successfully.\n";
                    int bytes_sent = send(socket_client, response, strlen(response), 0);
                    break;
                }
                case 2: {
                    char product_name[20];
                    send_and_await(socket_client, "Enter product's name: ", product_name);

                    struct Product* p = newProduct(product_name, 0, "");
                    struct CatalogItem* item = searchCatalog(*p, *catalog);
                    if(item->stock == 0) {
                        const char* response = "Product does not exist in the catalog \n";
                        int bytes_sent = send(socket_client, response, strlen(response), 0);
                        printf("Sent %d of %d bytes.\n", bytes_sent, sizeof(response));
                    }
                    else {
                        char *response;
                        int response_len = asprintf(&response, "Name: %s \t Price: %d \t Stock: %d \n", item->product->name, item->product->price, item->stock);
                        int bytes_sent = send(socket_client, response, response_len, 0);
                        printf("Sent %d of %d bytes.\n", bytes_sent, response_len);
                    }
                    break;
                }
                case 3: ; {
                    int bytes_sent;
                    for(int i = 0; i<catalog->catalogSize; i++) {
                        struct CatalogItem* item = catalog->items[i];

                        char *catalog_item_number;
                        int catalog_item_number_len = asprintf(&catalog_item_number, " Catalog Item: %d\n", i+1);
                        bytes_sent = send(socket_client, catalog_item_number, catalog_item_number_len, 0);
                        printf("Sent %d of %d bytes.\n", bytes_sent, catalog_item_number_len);

                        char *catalog_item_details;
                        int catalog_item_details_len = asprintf(&catalog_item_details," Item name: %s \n Item price: %d \n Stock: %d \n\n", item->product->name, item->product->price, item->stock);
                        bytes_sent = send(socket_client, catalog_item_details, catalog_item_details_len, 0);
                        printf("Sent %d of %d bytes.\n", bytes_sent, catalog_item_details_len);
                    }
                    break;
                }
                case 4: ; {
                    char *response;
                    int response_len = asprintf(&response, "Catalog size: %d \n", catalog->catalogSize);
                    int bytes_sent = send(socket_client, response, response_len, 0);
                    printf("Sent %d of %d bytes.\n", bytes_sent, response_len);
                    break;
                }
                case 5: ; {
                    struct Product* product = newProduct("", 0, "");

                    char product_name[20];
                    send_and_await(socket_client, "Enter the product's name: ", product_name);
                    strcpy(product->name, product_name);

                    char payment_amount_string[20];
                    send_and_await(socket_client, "Enter the payment amount: ", payment_amount_string);
                    struct Payment payment = *newPayment(atoi(payment_amount_string));

                    struct Payment change = purchaseItem(*product, payment, catalog);
                    char *response;
                    int response_len;
                    if(change.amount == payment.amount) {
                        response_len = asprintf(&response, "Could not complete purchase. Check that the item exists in the catalog AND that the payment amount is more than the product's price \n");
                    } else {
                        response_len = asprintf(&response, "Change amount: %d \n", change.amount);
                    }
                    int bytes_sent = send(socket_client, response, response_len, 0);
                    printf("Sent %d of %d bytes.\n", bytes_sent, response_len);

                    break;
                }
                case 6: ; {
                    printf("Closing connection...\n");
                    closesocket(socket_client);
                    exit = 1;
                    break;
                }
                default: ; {
                    char response[100];
                    sprintf(response, "Invalid selection.\n");
                    send(socket_client, response, sizeof(response), 0);
                    break;
                }
            }
            if(exit != 1) send_cli_options(socket_client);
        }
    }

    WSACleanup();
    printf("Finished.\n");
    return 0;
}

#endif // SERVER_APP_H_INCLUDED
