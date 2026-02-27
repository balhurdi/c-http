#include "server.h"
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

void on_connect(client_t client) {
  int32_t client_id = client_fd(client);
  printf("Connection from client %d\n", client_id);
}

int main() {
  server_start(9177, on_connect);
  return 0;
}
