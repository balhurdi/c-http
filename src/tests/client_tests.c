#include "../client.h"
#include <assert.h>

void set_get() {
  client_pool_t client_pool = client_pool_new();
  client_pool_add_client(client_pool, 1);
  client_pool_add_client(client_pool, 2);
  client_pool_add_client(client_pool, 3);

  client_t cli1 = client_pool_get_client(client_pool, 1);
  assert(client_fd(cli1) == 1);
  client_t cli2 = client_pool_get_client(client_pool, 2);
  assert(client_fd(cli2) == 2);
  client_t cli3 = client_pool_get_client(client_pool, 3);
  assert(client_fd(cli3) == 3);

  client_t cli4 = client_pool_get_client(client_pool, 4);
  assert(cli4 == NULL);

  client_pool_drop(client_pool);
}

void arena_expansion() {
  client_pool_t client_pool = client_pool_new();
  for (int i = 0; i < 15; i++) {
    client_pool_add_client(client_pool, i);
    client_t cli = client_pool_get_client(client_pool, i);
    assert(client_fd(cli) == i);
  }
  client_pool_drop(client_pool);
}

void remove_cli() {
  client_pool_t client_pool = client_pool_new();

  client_pool_add_client(client_pool, 1);
  client_pool_add_client(client_pool, 2);
  client_pool_add_client(client_pool, 3);

  client_pool_remove_client(client_pool, 2);
  client_t cli = client_pool_get_client(client_pool, 2);
  assert(cli == NULL);

  client_pool_drop(client_pool);
}

int main() {
  set_get();
  arena_expansion();
  remove_cli();
}
