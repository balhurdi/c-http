#include "server.h"
#include <fcntl.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_EPOLL_SIZE 10

const int32_t LISTEN_QUEUE_SIZE = 10;

static int setnonblocking(int sockfd) {
  if (fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFL, 0) | O_NONBLOCK) == -1) {
    return -1;
  }
  return 0;
}

static void epoll_ctl_add(int epfd, int fd, uint32_t events) {
  struct epoll_event ev;
  ev.events = events;
  ev.data.fd = fd;
  if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1) {
    perror("epoll_ctl()\n");
    exit(1);
  }
}

void server_start(uint16_t port, on_client_connect_cb cb) {

  struct sockaddr_in serv_addr = {0};

  int32_t listen_fd = socket(AF_INET, SOCK_STREAM, 0);

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(port);

  int opt = 1;
  setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  int32_t res =
      bind(listen_fd, (const struct sockaddr *)&serv_addr, sizeof(serv_addr));

  if (res != 0) {
    return;
  }

  res = listen(listen_fd, LISTEN_QUEUE_SIZE);
  setnonblocking(listen_fd);

  if (res != 0) {
    return;
  }

  struct epoll_event events[MAX_EPOLL_SIZE];
  int32_t epoll_fd = epoll_create(1);

  epoll_ctl_add(epoll_fd, listen_fd, EPOLLIN | EPOLLOUT | EPOLLET);

  while (1) {

    int32_t event_count = epoll_wait(epoll_fd, events, MAX_EPOLL_SIZE, -1);

    for (int i = 0; i < event_count; i++) {
      if (events[i].data.fd == listen_fd) {
        int32_t connfd = accept(listen_fd, NULL, NULL);
        (cb)();
        setnonblocking(connfd);
        epoll_ctl_add(epoll_fd, connfd,
                      EPOLLIN | EPOLLET | EPOLLRDHUP | EPOLLHUP);
      }

      if (events[i].events & EPOLLIN && events[i].data.fd == listen_fd) {
        printf("Received data from client\n");
      }

      if (events[i].events & (EPOLLRDHUP | EPOLLHUP)) {
        printf("Client disconnected\n");
        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
        close(events[i].data.fd);
        continue;
      }
    }
  }
}
