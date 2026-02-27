#pragma once
#include <inttypes.h>

typedef void (*on_client_connect_cb)();

void server_start(uint16_t port, on_client_connect_cb cb);
