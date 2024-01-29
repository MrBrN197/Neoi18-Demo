#include <stdio.h>
#include <wayland-server.h>

typedef struct wl_display wl_display;
typedef struct wl_protocol_logger wl_protocol_logger;
typedef struct wl_display wl_display;

typedef struct wl_display wl_display;

void logger_func(void *user_data, enum wl_protocol_logger_type direction,
                 const struct wl_protocol_logger_message *message) {}

void start_server() {
  wl_display *disp = wl_display_create();
  if (!disp)
    printf("wl_display_create => null \n");

  printf("wl_display_create => %p\n", disp);

  int s;
  s = wl_display_add_socket(disp, "/run/user/1000/wayland-server-0");
  printf("wl_display_add_socket => %d\n", s);
  if (s == 0)
    printf("wl_display_add_socket()\n");

  void *user_data = (void *)(0);
  wl_protocol_logger *logger = wl_display_add_protocol_logger(
      disp, (wl_protocol_logger_func_t)logger_func, user_data);

  if (logger == NULL) {
    printf("Failed: wl_display_add_protocol => NULL\n");
    return;
  }

  wl_display_destroy(disp);
  printf("wl_display_destroy()\n");
}

int main() {
  start_server();
  return 0;
}
