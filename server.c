#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_read = read(client_socket, buffer, BUFFER_SIZE - 1);
        if (bytes_read <= 0) {
            break;
        }

        printf("Received: %s", buffer);
        ssize_t bytes_written = 0;
        // 處理指令
        if (strncmp(buffer, "add", 3) == 0) {
            int a, b;
            sscanf(buffer + 4, "%d %d", &a, &b);
            int result = a + b;
            snprintf(buffer, BUFFER_SIZE, "%d\n", result);
            bytes_written = write(client_socket, buffer, strlen(buffer));
        } else if (strncmp(buffer, "kill", 4) == 0) {
            snprintf(buffer, BUFFER_SIZE, "Server shutting down\n");
            bytes_written = write(client_socket, buffer, strlen(buffer));
            break;
        } 
        else if(strncmp(buffer, "abs", 3) == 0) {
            int a;
            sscanf(buffer + 4, "%d", &a);   
            int result = abs(a);
            snprintf(buffer, BUFFER_SIZE, "%d\n", result);
            bytes_written = write(client_socket, buffer, strlen(buffer));
        } 
        else if(strncmp(buffer, "mul", 3) == 0) {
            int a, b;
            sscanf(buffer + 4, "%d %d", &a, &b);   
            int result = a * b;
            snprintf(buffer, BUFFER_SIZE, "%d\n", result);
            bytes_written = write(client_socket, buffer, strlen(buffer));
        } 
        else {
            snprintf(buffer, BUFFER_SIZE, "Hello\n");
            bytes_written = write(client_socket, buffer, strlen(buffer));
        }
    }

    close(client_socket);
    printf("Client disconnected\n");
}

int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // 創建 socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // 設置 socket 選項
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 綁定地址和端口
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 開始監聽
    if (listen(server_fd, 3) < 0) {
        perror("Listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    // 接受客戶端連線
    while ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) >= 0) {
        printf("Client connected\n");
        handle_client(client_socket);
    }

    if (client_socket < 0) {
        perror("Accept");
    }

    close(server_fd);
    return 0;
}
