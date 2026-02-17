#pragma once
#include <inttypes.h>
typedef struct _server *server_t;

typedef void (*on_client_connect_cb)();

server_t server_start(uint16_t port, on_client_connect_cb cb);
void stop(server_t server);
