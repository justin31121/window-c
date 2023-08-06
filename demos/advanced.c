#define WINDOW_IMPLEMENTATION
#define WINDOW_VERBOSE // log errors
#include "../src/window.h"

int main() {
  
  Window window;
  if(!window_init(&window, 800, 800, "Advanced", 0)) {
    return 1;
  }

#define TINT 0.09411764705882353

  window_renderer_set_color(vec4f(TINT, TINT, TINT, 1));
  
  Window_Event event;
  while(window.running) {
    while(window_peek(&window, &event)) {
      switch(event.type) {
      case WINDOW_EVENT_KEYPRESS: {
	switch(event.as.key) {
	case 'Q': {
	  window.running = false;
	} break;
	default: {
		  
	} break;
	}
      } break;
      default: {
	      
      } break;
      }
    }

    int width = window.width;
    int height = window.height;
            
    window_renderer_begin(width, height);
      
    float r = 40;
    int parts = 40;
    Vec4f c = vec4f(0, 0, 1, 1);

    int w = 400;
    int h = 400;
    
    draw_solid_rounded_shaded_rect(vec2f(width/2 - w/2, height/2 - h/2),
				   vec2f(w, h),
				   (float) r,
				   parts,
				   5,
				   c);


    window_renderer_end(width, height);
    window_swap_buffers(&window);
  }

  window_free(&window);
}

