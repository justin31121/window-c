#include <stdio.h>

#define WINDOW_IMPLEMENTATION
#include "../src/window.h"

int main() {

  Window window;
  if(!window_init(&window, 400, 400, "Window")) {
    return 1;
  }

  int width, height;
  Window_Event event;
  while(window.running) {
    while(window_peek(&window, &event)) {
      if(event.type == WINDOW_EVENT_KEYPRESS) window.running = false;
    }

    if(!window_get_window_size(&window, &width, &height)) return 1;
    
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(1, 0, 0, 1);

    window_swap_buffers(&window);
  }

  window_free(&window);
  
  return 0;
}
