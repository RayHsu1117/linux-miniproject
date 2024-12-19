# 編譯器與選項
CC = gcc
CFLAGS = -Wall -Wextra -O2

# 可執行文件
TARGETS = client server

all: $(TARGETS)

client: client.c
	$(CC) $(CFLAGS) -o client client.c

server: server.c
	$(CC) $(CFLAGS) -o server server.c

clean:
	rm -f $(TARGETS) out

test: all
	./server &
	sleep 1 # 延遲確保 server 啟動
	./client < in > out
	killall server
	diff -s out ans
