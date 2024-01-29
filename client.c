#include <stdio.h>
#include <threads.h>
#include <wayland-client.h>

typedef struct wl_display wl_display;
typedef struct wl_interface wl_interface;
typedef struct wl_proxy wl_proxy;
typedef struct wl_registry wl_registry;
typedef struct timespec timespec;

#define status(x)                                                              \
  if (x != 0) {                                                                \
    printf("failed\n");                                                        \
    return 1;                                                                  \
  };

int main() {
  wl_display *server;
  int init = 0;
  timespec ts = {.tv_sec = 1};

  while (!init) {
    printf("connecting to server...\n");
    server = wl_display_connect("/run/user/1000/wl-server-0");
    if (server) {
      printf("==> display_connected\n");
      init = 1;
      break;
    }
    thrd_sleep(&ts, NULL);
  }

  int s = wl_display_dispatch(server);
  if (s != 0) {
    printf("failed to read from queue\n");
    return 1;
  }

  s = wl_display_get_error(server);
  status(s);

  // wl_registry *registry = wl_display_get_registry(server);
  // printf("registry:%p\n", registry);
  //
  // if (!registry) {
  //   return 3;
  //   printf("failed\n");
  // }

  // wl_display_get_fd
  return 0;
}
