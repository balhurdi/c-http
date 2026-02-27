#pragma once
#include "client.h"
#include <inttypes.h>

typedef void (*on_client_connect_cb)(client_t client);
typedef void (*on_client_msg_cb)(client_t client);
typedef void (*on_client_disconnect_cb)(client_t client);

void server_start(uint16_t port, on_client_connect_cb conn_cb,
                  on_client_msg_cb msg_cb, on_client_disconnect_cb dc_cb);
