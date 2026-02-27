#pragma once
#include <inttypes.h>
#include "client.h"

typedef void (*on_client_connect_cb)(client_t client);
typedef void (*on_client_msg_cb)();
typedef void (*on_client_disconnect_cb());

void server_start(uint16_t port, on_client_connect_cb cb);
