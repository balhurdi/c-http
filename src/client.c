#include "client.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define MIN_CLIENT_POOL_LEN 10

typedef struct _client {
  int32_t conn_fd;
  char *data;
  bool is_alive;
} *client_t;

typedef struct _client_pool {
  size_t client_count;
  size_t max_size;
  client_t arena;
} *client_pool_t;

client_pool_t client_pool_new() {
  client_pool_t client_pool = malloc(sizeof(struct _client_pool));

  client_pool->client_count = 0;
  client_pool->max_size = MIN_CLIENT_POOL_LEN;
  client_pool->arena = malloc(sizeof(struct _client) * MIN_CLIENT_POOL_LEN);

  for (int i = 0; i < MIN_CLIENT_POOL_LEN; i++) {
    client_pool->arena[i] = (struct _client){
        .conn_fd = 0,
        .is_alive = false,
    };
  }

  return client_pool;
}

client_t client_pool_add_client(client_pool_t pool, int32_t client_fd) {

  assert(pool != NULL);

  struct _client new_client = (struct _client){
      .conn_fd = client_fd,
      .data = NULL,
      .is_alive = true,
  };

  for (size_t i = 0; i < pool->max_size; i++) {
    if (!pool->arena[i].is_alive) {
      pool->arena[i] = new_client;
      return &pool->arena[i];
    }
  }

  size_t new_max_size = pool->max_size * 2;
  pool->arena =
      realloc(pool->arena, sizeof(struct _client) * pool->max_size * 2);
  for (size_t i = pool->max_size; i < new_max_size; i++) {
    pool->arena[i].conn_fd = 0;
    pool->arena[i].data = NULL;
    pool->arena[i].is_alive = false;
  }

  pool->arena[pool->max_size] = new_client;
  pool->max_size = new_max_size;

  return &pool->arena[pool->max_size];
}

void client_pool_remove_client(client_pool_t pool, int32_t client_fd) {

  assert(pool != NULL);

  for (size_t i = 0; i < pool->max_size; i++) {
    struct _client client = pool->arena[i];
    if (client.conn_fd == client_fd && client.is_alive == true) {
      pool->arena[i].conn_fd = 0;
      pool->arena[i].data = NULL;
      pool->arena[i].is_alive = false;
      break;
    }
  }
}

client_t client_pool_get_client(client_pool_t pool, int32_t client_fd) {
  assert(pool != NULL);

  for (size_t i = 0; i < pool->max_size; i++) {
    if (pool->arena[i].conn_fd == client_fd) {
      return &pool->arena[i];
    }
  }

  return NULL;
}

void client_pool_drop(client_pool_t client_pool) {
  assert(client_pool != NULL);

  free(client_pool->arena);
  free(client_pool);
}

void client_recvd_data(client_t client, char *data, size_t *size) {
  (void)client;
  (void)data;
  (void)size;
}
void client_send_data(client_t client, const char *data, size_t data_len) {
  (void)client;
  (void)data;
  (void)data_len;
}

int32_t client_fd(client_t client) { return client->conn_fd; }
