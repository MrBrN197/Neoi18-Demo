#include <stdio.h>
#include <threads.h>
#include <wayland-client.h>

typedef struct wl_display_listener wl_display_listener;
typedef struct wl_proxy wl_proxy;
typedef struct wl_display wl_display;
typedef struct wl_interface wl_interface;
typedef struct wl_proxy wl_proxy;
typedef struct wl_registry wl_registry;
typedef struct timespec timespec;
typedef struct wl_callback wl_callback;
typedef struct wl_event_loop wl_event_loop;

#define status(X)                                                              \
  if ((X) != 0) {                                                              \
    printf("failed\n");                                                        \
    return 1;                                                                  \
  };

#define check(X)                                                               \
  if (!(X)) {                                                                  \
    printf("failed\n");                                                        \
    return 1;                                                                  \
  };

int main() {
  setvbuf(stdout, NULL, _IONBF, 0);

  wl_display *server;
  int init = 0;
  int s;
  timespec ts = {.tv_sec = 1};

  while (!init) {
    printf("connecting to server...\n");
    server = wl_display_connect("/run/user/1000/wl-server-0");
    // int fd = wl_display_get_fd(server);
    if (server) {
      printf("==> display_connected\n");
      init = 1;
      break;
    }
    thrd_sleep(&ts, NULL);
  }

  // wl_display_listener *listener;
  // s = wl_display_add_listener(server, listener, NULL);

  // wl_proxy *p;
  // uint32_t op;
  // wl_proxy_marshal(p, op /*, ...*/);
  wl_registry *registry = wl_display_get_registry(server);
  if (!registry) {
    return 3;
    printf("failed\n");
  }

  s = wl_display_get_error(server);
  status(s);

  wl_callback *cb = wl_display_sync(server);
  status(cb == 0 ? 1 : 0);
  s = wl_display_flush(server);
  check(s > 0);

  // s = wl_display_roundtrip(server);
  // status(s);
  // printf("roundtrip\n");

  s = wl_display_dispatch(server);
  if (s != 0) {
    printf("failed to read from queue\n");
    return 1;
  }
  // printf("read bytes\n");

  // wl_display_get_fd
  return 0;
}
