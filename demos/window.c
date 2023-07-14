#include <stdio.h>

#define WINDOW_IMPLEMENTATION
#include "../src/window.h"

int main() {

  Window window;
  if(!window_init(&window, 400, 400, "Window")) {
    return 1;
  }

  Window_Event event;
  while(window.running) {
    while(window_peek(&window, &event)) ;
    
    glViewport(0, 0, 400, 400);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(1, 0, 0, 1);

    window_swap_buffers(&window);
  }

  window_free(&window);
  
  return 0;
}
