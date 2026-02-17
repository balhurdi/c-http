#include "server.h"
#include <netinet/in.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

const int32_t LISTEN_QUEUE_SIZE = 10;
_Atomic uint32_t running = 1;

typedef struct {
  int32_t listen_fd;
  on_client_connect_cb cb;
} server_runtime_args;

void *server_runtime(void *args) {

  server_runtime_args *config = (server_runtime_args *)args;

  while (running) {
    int32_t connfd = accept(config->listen_fd, NULL, NULL);
    (config->cb)();
    close(connfd);
  }

  return NULL;
}

typedef struct _server {
  int32_t listen_fd;
  server_runtime_args *runtime_args;
  pthread_t running_thread;
} *server_t;

server_t server_start(uint16_t port, on_client_connect_cb cb) {
  server_t server = (server_t)malloc(sizeof(struct _server));

  struct sockaddr_in serv_addr = {0};

  int32_t listen_fd = socket(AF_INET, SOCK_STREAM, 0);

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(port);

  int opt = 1;
  setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  int32_t res =
      bind(listen_fd, (const struct sockaddr *)&serv_addr, sizeof(serv_addr));

  if (res != 0) {
    return NULL;
  }

  res = listen(listen_fd, LISTEN_QUEUE_SIZE);

  if (res != 0) {
    return NULL;
  }

  pthread_t thread;
  server_runtime_args *runtime_args = malloc(sizeof(server_runtime_args));
  runtime_args->cb = cb;
  runtime_args->listen_fd = listen_fd;

  pthread_create(&thread, NULL, &server_runtime, (void *)runtime_args);

  server->listen_fd = listen_fd;
  server->runtime_args = runtime_args;
  server->running_thread = thread;

  return server;
}

void stop(server_t server) {
  running = 0;
  void *r;
  pthread_join(server->running_thread, &r);
  shutdown(server->listen_fd, SHUT_RDWR);
  close(server->listen_fd);
  free(server->runtime_args);
  free(server);
}
