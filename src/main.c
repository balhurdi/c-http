#include "server.h"
#include <stdio.h>

void on_connect() { printf("Connection from client\n"); }

int main() {
  server_t server = server_start(8080, on_connect);
  return 0;
}
