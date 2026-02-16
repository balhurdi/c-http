#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

int main() {
  int32_t listen_fd = 0;
  int32_t connfd = 0;

  struct sockaddr_in serv_addr;

  uint8_t *buffer = "Hello World\n";

  listen_fd = socket(AF_INET, SOCK_STREAM, 0);
  memset(&serv_addr, 0, sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(8080);

  int32_t res =
      bind(listen_fd, (const struct sockaddr *)&serv_addr, sizeof(serv_addr));

  if (res != 0) {
    return res;
  }

  listen(listen_fd, 10);

  while (1) {
    connfd = accept(listen_fd, NULL, NULL);
    write(connfd, buffer, strlen(buffer));
    close(connfd);
  }

  close(listen_fd);

  return 0;
}
