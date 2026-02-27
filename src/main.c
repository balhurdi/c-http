#include "server.h"
#include <stdio.h>
#include <unistd.h>

void on_connect() { printf("Connection from client\n"); }

int main() {
  server_start(9177, on_connect);
  return 0;
}
