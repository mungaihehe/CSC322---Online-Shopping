# Online shopping application - CSC 322 - Version 3 (Separate client/server)

Programmed in the C programming language

## Provides the following services

1. Display catalog
2. Search for an item in the catalog
3. Purchase an item
4. Payment to complete a purchase

## Design Diagram

![UML diagram](design.jpg)

## How to build

1. Clone the repository
2. Build and run main.c - the code does not require any external dependencies (server-side code)
   `gcc main.c -o main.exe -lws2_32`
3. Build and run client.c (client-side code)
   `gcc client.c -o client.exe -lws_32`

## Usage

### Client

`client.exe [hostname] [port]`

#### Example

`client.exe 127.0.0.1 8080`

### Server

Runs on port 8080

`main.exe`
