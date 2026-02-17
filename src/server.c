#include "server.h"
#include <netinet/in.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

const int32_t LISTEN_QUEUE_SIZE = 10;

typedef struct _server {
  int32_t listen_fd;
} *server_t;

server_t server_start(uint16_t port, on_client_connect_cb cb) {
  server_t server = (server_t)malloc(sizeof(struct _server));

  struct sockaddr_in serv_addr = {0};
  int32_t listen_fd = socket(AF_INET, SOCK_STREAM, 0);

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(port);

  int32_t res =
      bind(listen_fd, (const struct sockaddr *)&serv_addr, sizeof(serv_addr));

  if (res != 0) {
    return NULL;
  }

  res = listen(listen_fd, LISTEN_QUEUE_SIZE);

  if (res != 0) {
    return NULL;
  }

  while (1) {
    int32_t connfd = accept(listen_fd, NULL, NULL);
    (cb)();
    close(connfd);
  }

  return server;
}

void stop(server_t server) {
  close(server->listen_fd);
  free(server);
}
