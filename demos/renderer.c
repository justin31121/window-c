
#define STB_TRUETYPE_IMPLEMENTATION 
#include "../thirdparty/stb_truetype.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../thirdparty/stb_image.h"

#define WINDOW_IMPLEMENTATION
#define WINDOW_STB_IMAGE
#define WINDOW_STB_TRUETYPE
#include "../src/window.h"

#include <stdint.h>

// Logo.jpg (https://de.freepik.com/vektoren-kostenlos/vogel-bunter-logo-gradientenvektor_28267842.htm#query=logo&position=2&from_view=keyword&track=sph)

int main() {
  
  Window window;
  if(!window_init(&window, 700, 400, "Renderer", 0)) {
    return 1;
  }

  unsigned int tex;
  int logo_width, logo_height;
  if(!push_image("rsc\\logo.jpg", &logo_width, &logo_height, &tex)) {
    return 1;
  }

  /*
  #include "../rsc/logo.h"
  if(!push_texture(logo_width, logo_height, logo_data, false, &tex)) {
      return 1;
  }
  */

  if(!push_font("C:\\Windows\\Fonts\\arial.ttf", 64.0)) {
      return 1;
  }

  int64_t time = 0.0f;

  int mouse_x = 0;
  int mouse_y = 0;
  Window_Event event;
  while(window.running) {
    while(window_peek(&window, &event)) {
      if(event.type == WINDOW_EVENT_KEYPRESS) {
	if(event.as.key == 'q') {
	  window.running = false;	  
	} else if(event.as.key == 'k') {
	  window_toggle_fullscreen(&window);
	}
      }
    }
    float widthf  = (float) window.width;
    float heightf = (float) window.height;

    if(window_get_mouse_position(&window, &mouse_x, &mouse_y)) {
      if(mouse_x > widthf) mouse_x = widthf;
      if(mouse_x < 0) mouse_x = 0;
      if(mouse_y > heightf) mouse_y = heightf;
      if(mouse_y < 0) mouse_y = 0;
      mouse_y = heightf - mouse_y;
    }
    
    Vec2f uv = vec2f(-1, -1);
    draw_triangle(vec2f(0, 0), vec2f(widthf, 0), vec2f(widthf, heightf),
		  RED, GREEN, BLUE,
		  uv, uv, uv);

    draw_solid_triangle(vec2f(0, heightf), vec2f(0, heightf/2), vec2f(widthf/2, heightf),
			vec4f(1, 1, 0, .2) );

    draw_solid_rect( vec2f(0, 0), vec2f(100, 100), vec4f(0, 0, 1, 1) );

    draw_texture_colored( tex,
			  vec2f(100, 100),
			  vec2f(100, 100),
			  vec2f(0, 0),
			  vec2f(1, 1),
			  vec4f(1, 1, 1, .5));

    Vec2f size;
    float factor = .5f;
    
    const char *text = "Text is only possible with stb_truetype.h";
    measure_text(text, factor, &size);
    draw_text_colored(text,
		      vec2f(widthf/2 - size.x/2,
			    heightf/2 - size.y/2),
		      factor, vec4f(1, 1, 1, .9) );

    text = "Press k to toggle fullscreen!";
    measure_text(text, factor, &size);
    draw_text_colored( text,
		       vec2f(widthf/2 - size.x/2, heightf/2 - size.y/2 - 4 * size.y),
		       factor, vec4f(1, 1, 1, .9) );

    char buf[256];
    snprintf(buf, sizeof(buf), "%d x %d", window.width, window.height);
    draw_text(buf, vec2f(0, heightf - 64.0f * .5f), .5f );

    snprintf(buf, sizeof(buf), "%.2f ms", window.dt);
    draw_text(buf, vec2f(0, heightf - 64.0f * .5f * 2), .5f );

    draw_solid_circle(vec2f((float) mouse_x, (float) mouse_y),
		      0,
		      2 * PI,
		      10.0f,
		      20,
		      vec4f(1, 0, 0, 1) );

    draw_solid_rect_angle( vec2f(widthf/2 - 50, heightf/2 - 50), vec2f(100, 100), time * 6 / 4 * PI / 1000 , vec4f(1, 0, 0, .4) );
	    
    window_swap_buffers(&window);

    time += 16;
  }

  window_free(&window);
}
