#include "server.h"
#include <stdio.h>
#include <unistd.h>

void on_connect() { printf("Connection from client\n"); }

int main() {
  server_t server = server_start(9177, on_connect);

  if (server == NULL) {
      printf("Failed to start server\n");
      return -1;
  }

  stop(server);
  return 0;
}
