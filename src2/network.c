#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include "network.h"

void init_network(ijvm *m) {
  for (int i = 0; i < m->MAX_CONNECTIONS; i++) {
    m->conn[i].sock = -1;
    m->conn[i].used = false;
  }
}

int find_free_connection(ijvm *m) {
  for (int i = 0; i < m->MAX_CONNECTIONS; i++) {
    if (!m->conn[i].used) {
      m->conn[i].used = true;
      return i;
    }
  }
  return -1;
}

void close_connection(ijvm *m, int index) {
  if (m->conn[index].used) {
    close(m->conn[index].sock);
    m->conn[index].used = false;
  }
}
