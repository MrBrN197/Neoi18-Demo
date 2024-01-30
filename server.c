#include <stdio.h>
#include <wayland-server.h>

typedef struct wl_display wl_display;
typedef struct wl_protocol_logger wl_protocol_logger;
typedef struct wl_display wl_display;

typedef struct wl_display wl_display;
typedef struct wl_event_source wl_event_source;
typedef struct wl_list wl_list;
typedef struct wl_event_loop wl_event_loop;

void logger_func(void *user_data, enum wl_protocol_logger_type direction,
                 const struct wl_protocol_logger_message *message) {
  printf("[LOGGER]\n");
}

void idle_cb_f(void *data) { printf("[Idle]\n"); }

int evsource_cb_f(void *data) {
  printf("[Timer]\n");
  return 0;
}

int fdsource_cb(int fd, uint32_t mask, void *data) {
  printf("fd event source callback\n");
  return 0;
}
//

wl_event_source *create_timer(wl_event_loop *evloop, void *user_data, int ms) {
  wl_event_source *timer =
      wl_event_loop_add_timer(evloop, &evsource_cb_f, user_data);
  if (!timer) {
    printf("event_loop_add_timer => null\n");
    return NULL;
  }

  int status = wl_event_source_timer_update(timer, ms);
  if (status != 0) {
    return NULL;
  }

  return timer;
}

int start_server() {
  setvbuf(stdout, NULL, _IONBF, 0);
  int s;
  printf("server: creating display \n");
  wl_display *disp = wl_display_create();
  if (!disp) {
    printf("wl_display_create => null \n");
    return 1;
  }

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

  wl_event_loop *ev_loop = wl_event_loop_create();
  if (!ev_loop) {
    printf("failed\n");
    return 3;
  }

  wl_event_source *timer = create_timer(ev_loop, (void *)0, 1000);
  if (!timer) {
    return 8;
  }

  wl_event_source *idle_source =
      wl_event_loop_add_idle(ev_loop, &idle_cb_f, (void *)0);
  if (!idle_source) {
    printf("Runnig\n");
    return 8;
  }

  int timeout = 240;
  printf("... => %d\n", s);
  int i = 0;
  while (true) {
    i += 1;
    printf("\r     ");
    printf("\r%d", i);
    s = wl_event_loop_dispatch(ev_loop, timeout);
    if (s != 0) {
      printf("Failed\n");
      return 1;
    }
  };

  wl_list *prev = wl_display_get_client_list(disp);
  wl_list *list = prev->next;
  printf("list => %p\n", prev);
  while (list != prev) {
    printf("count %p\n", list);
    list = list->next;
  }

  printf("wl_event_loop_destroy()\n");
  wl_event_loop_destroy(ev_loop);

  wl_display_destroy(disp);
  printf("wl_display_destroy()\n");
  return 0;
}

int main() { return start_server(); }
