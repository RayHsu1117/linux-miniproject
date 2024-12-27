#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in server_address;
    char buffer[BUFFER_SIZE];
    FILE *output_file;

    // 打開輸出文件
    output_file = fopen("out", "w");
    if (output_file == NULL) {
        perror("Failed to open output file");
        return -1;
    }
    // 創建 socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return -1;
    }

    // 設定 Server 地址
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);

    // 將 Server IP 地址轉換為二進制格式
    if (inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr) <= 0) {
        perror("Invalid address / Address not supported");
        return -1;
    }

    // 連接到 Server
    if (connect(sock, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Connection failed");
        return -1;
    }

    //printf("Connected to server at %s:%d\n", SERVER_IP, SERVER_PORT);

    // 主循環：讀取輸入，發送指令並接收回應
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);

        // 從 stdin 讀取輸入
        // printf("Enter command: ");
        if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
            break;
        }

        // 發送輸入給 Server
        if (send(sock, buffer, strlen(buffer), 0) < 0) {
            perror("Send failed");
            break;
        }

        // 處理 "kill" 指令，結束程序
        if (strncmp(buffer, "kill", 4) == 0) {
            // printf("Server terminated, exiting client.\n");
            break;
        }

        // 接收來自 Server 的回應
        memset(buffer, 0, BUFFER_SIZE);
        if (recv(sock, buffer, BUFFER_SIZE, 0) < 0) {
            perror("Receive failed");
            break;
        }

        // printf("Server response: %s", buffer);
        //printf("%s",buffer);
	fprintf(output_file, "%s", buffer);
    }

    // 關閉 socket
    close(sock);
    return 0;
}
