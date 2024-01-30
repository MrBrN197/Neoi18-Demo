#include <config.h>
#include <stdio.h>
#include <threads.h>
#include <wayland-server-core.h>
#include <wayland-server.h>

typedef struct timespec timespec;
typedef struct wl_client wl_client;
typedef struct wl_resouce wl_resouce;
typedef struct wl_message wl_message;
typedef struct wl_resource wl_resource;
typedef struct wl_listener wl_listener;
typedef struct wl_display wl_display;
typedef struct wl_protocol_logger wl_protocol_logger;
typedef struct wl_display wl_display;

typedef struct wl_display wl_display;
typedef struct wl_event_source wl_event_source;
typedef struct wl_list wl_list;
typedef struct wl_event_loop wl_event_loop;

// void format_resource(wl_resouce *resource) {}

void logger_func(void *user_data, enum wl_protocol_logger_type direction,
                 const struct wl_protocol_logger_message *message) {
#ifdef DEBUG
  wl_message msg = *message->message;

  if (direction == WL_PROTOCOL_LOGGER_REQUEST) {
    printf("\t\e[38;5;1m[req] <== [Msg]: %s\n[m", msg.name);
  } else if (direction == WL_PROTOCOL_LOGGER_EVENT) {
    printf("\t\e[38;5;10m[ev] ==> [Msg]: %s\n[m", msg.name);
  }
#endif
}

void notif_cb_f(struct wl_listener *listener, void *data) {
  wl_client *new_client = (wl_client *)data;
  timespec ts = {.tv_sec = 1000};
  thrd_sleep(&ts, NULL);
  wl_client_destroy(new_client);
  printf("[Client]\n");
}

int fdsource_cb(int fd, uint32_t mask, void *data) {
  printf("fd event source callback\n");
  return 0;
}
//

int start_server() {
  setvbuf(stdout, NULL, _IONBF, 0);

  int s;
  printf("server: creating display \n");
  wl_display *disp = wl_display_create();
  if (!disp) {
    printf("wl_display_create => null \n");
    return 1;
  }
  wl_listener lstner = {.notify = notif_cb_f};
  // lstner.link;
  wl_display_add_client_created_listener(disp, &lstner);

  void *user_data = (void *)(0);
  wl_protocol_logger *logger = wl_display_add_protocol_logger(
      disp, (wl_protocol_logger_func_t)logger_func, user_data);

  if (logger == NULL) {
    printf("wl_display_add_protocol => NULL\n");
    return 2;
  }

  s = wl_display_add_socket(disp, "/run/user/1000/wl-server-0");
  if (s != 0) {
    printf("wl_display_add_socket => %d\n", s);
    return 1;
  }
  printf("server listenieng\n");

  wl_event_loop *ev_loop = wl_display_get_event_loop(disp);
  if (!ev_loop) {
    printf("failed\n");
    return 3;
  }

  while (true) {
    printf("polling ... => %d\n", s);
    s = wl_event_loop_dispatch(ev_loop, -1);
    if (s != 0) {
      printf("Failed\n");
      return 1;
    }
  };

  wl_list *prev = wl_display_get_client_list(disp);
  wl_list *list = prev->next;
  printf("list => %p\n", prev);
  while (list != prev) {
    printf(" => %p\n", list);
    list = list->next;
  }

  printf("wl_event_loop_destroy()\n");
  wl_event_loop_destroy(ev_loop);

  wl_display_destroy(disp);
  printf("wl_display_destroy()\n");
  return 0;
}

int main() { return start_server(); }
