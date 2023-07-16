#include <assert.h>

#define WINDOW_IMPLEMENTATION
#define WINDOW_VERBOSE
#include "../src/window.h"

int main() {
    Window window;
    if(!window_init(&window, 400, 400, "Renderer")) {
	return 1;
    }

    int width, height;
    Window_Event event;
    while(window.running) {
	while(window_peek(&window, &event)) {
	    if(event.type == WINDOW_EVENT_KEYPRESS &&
	       event.as.key == 'Q') {
		window.running = false;
	    }
	}
	if(!window_get_window_size(&window, &width, &height)) {
	    return 1;
	}
	
	float widthf  = (float) width;
	float heightf = (float) height;
	window_renderer_begin(&window_renderer, width, height);

	draw_triangle(vec2f(0, 0), vec2f(widthf, 0), vec2f(widthf, heightf),
		      vec4f(1, 0, 0, 1),
		      vec4f(0, 1, 0, 1),
		      vec4f(0, 0, 1, 1));	

	window_renderer_end(&window_renderer);
	window_swap_buffers(&window);
    }

    window_free(&window);
}
