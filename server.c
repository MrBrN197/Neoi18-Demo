#include <stdio.h>
#include <wayland-server.h>

typedef struct wl_display wl_display;
typedef struct wl_protocol_logger wl_protocol_logger;
typedef struct wl_display wl_display;

typedef struct wl_display wl_display;
typedef struct wl_event_source wl_event_source;

void logger_func(void *user_data, enum wl_protocol_logger_type direction,
                 const struct wl_protocol_logger_message *message) {
  printf("[LOGGER]\n");
}

// int get_event(struct wl_event_loop *evloop) {
//   s = wl_event_loop_get_fd(evloop);
// }

int evsource_cb(void *data) {
  printf("event_source callback\n");
  return 0;
}

int fdsource_cb(int fd, uint32_t mask, void *data) {
  printf("fd event source callback\n");
  return 0;
}
//

int start_server() {
  printf("server: creating display \n");
  wl_display *disp = wl_display_create();
  if (!disp)
    printf("wl_display_create => null \n");

  int s;
  s = wl_display_add_socket(disp, "/run/user/1000/wl-server-0");
  if (s != 0) {
    printf("wl_display_add_socket => %d\n", s);
    return 1;
  }
  printf("server listenieng\n");

  void *user_data = (void *)(0);
  wl_protocol_logger *logger = wl_display_add_protocol_logger(
      disp, (wl_protocol_logger_func_t)logger_func, user_data);

  if (logger == NULL) {
    printf("wl_display_add_protocol => NULL\n");
    return 2;
  }

  struct wl_event_loop *ev_loop = wl_event_loop_create();
  if (!ev_loop) {
    printf("faile\n");
    return 3;
  }

  wl_event_source *evsource =
      wl_event_loop_add_timer(ev_loop, &evsource_cb, (void *)0);
  if (!evsource) {
    printf("event_loop_add_timer => null\n");
    return 8;
  }

  int timeout = 1000;
  for (int i = 0; i < 1000; i++) {
    s = wl_event_loop_dispatch(ev_loop, timeout);
    printf("polling ... event_loop_dispatch => %d\n", s);
  }

  printf("wl_event_loop_destroy()\n");
  wl_event_loop_destroy(ev_loop);

  wl_display_destroy(disp);
  printf("wl_display_destroy()\n");
  return 0;
}

int main() { return start_server(); }
