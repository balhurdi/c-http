#pragma once
#include <stddef.h>
#include <stdint.h>

typedef struct _client *client_t;
typedef struct _client_pool *client_pool_t;

client_pool_t client_pool_new();
client_t client_pool_add_client(client_pool_t pool, int32_t client_fd);
void client_pool_remove_client(client_pool_t pool, int32_t client_fd);
client_t client_pool_get_client(client_pool_t pool, int32_t client_fd);
void client_pool_drop(client_pool_t client_pool);

int32_t client_fd(client_t client);
const char *client_recvd_data(client_t client, size_t *size);
void client_send_data(client_t client, const char *data, size_t data_len);
