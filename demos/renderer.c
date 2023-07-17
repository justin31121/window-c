#include <assert.h>

#define WINDOW_IMPLEMENTATION
#include "../src/window.h"

//https://de.freepik.com/vektoren-kostenlos/vogel-bunter-logo-gradientenvektor_28267842.htm#query=logo&position=2&from_view=keyword&track=sph
#include "../rsc/logo.h"

int main() {
  
  Window window;
  if(!window_init(&window, 400, 400, "Renderer")) {
    return 1;
  }
    
  unsigned int tex;
  if(!push_texture(logo_width, logo_height, logo_data, false, &tex)) {
    return 1;
  }

  int width, height;
  int mouse_x = 0;
  int mouse_y = 0;
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

    if(window_get_mouse_position(&window, &mouse_x, &mouse_y)) {
      if(mouse_x > width) mouse_x = width;
      if(mouse_x < 0) mouse_x = 0;
      if(mouse_y > height) mouse_y = height;
      if(mouse_y < 0) mouse_y = 0;
      mouse_y = height - mouse_y;

    }
    
    window_renderer_begin(&window_renderer, width, height);

    Vec2f uv = vec2f(0, 0);

    draw_triangle(vec2f(0, 0), vec2f(widthf, 0), vec2f(widthf, heightf),
		  vec4f(1, 0, 0, 1),
		  vec4f(0, 1, 0, 1),
		  vec4f(0, 0, 1, 1),
		  uv, uv, uv);

    draw_solid_triangle(vec2f(0, heightf), vec2f(0, heightf/2), vec2f(widthf/2, heightf),
			vec4f(1, 1, 0, 1) );

    draw_solid_rect( vec2f(0, 0), vec2f(100, 100), vec4f(0, 0, 1, 1) );    

    draw_texture(vec2f(100, 100),
		 vec2f(200, 200),
		 vec2f(0, 0),
		 vec2f(1, 1)
		 );

    draw_solid_circle(vec2f((float) mouse_x, (float) mouse_y),
		      10.0f,
		      20,
		      vec4f(1, 0, 0, 1) );
	
    window_renderer_end(&window_renderer);
    window_swap_buffers(&window);
  }

  window_free(&window);
}
