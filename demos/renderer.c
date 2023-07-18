#include <assert.h>

#define WINDOW_IMPLEMENTATION
#define WINDOW_VERBOSE
#include "../src/window.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "../thirdparty/stb_truetype.h"

#include <stdint.h>
#include <stdio.h>

//https://de.freepik.com/vektoren-kostenlos/vogel-bunter-logo-gradientenvektor_28267842.htm#query=logo&position=2&from_view=keyword&track=sph
#include "../rsc/logo.h"

unsigned char ttf_buffer[1<<20];
unsigned char temp_bitmap[1024*1024];
stbtt_bakedchar cdata[96]; // ASCII 32..126 is 95 glyphs

unsigned int font_tex;

void print(float x, float y, Vec4f color, float factor, const char *cstr) {
    size_t len = strlen(cstr);

    for(size_t i=0;i<len;i++) {
	char c = cstr[i];
	if (c < 32 && c >= 128) {
	    continue;
	}

	float _y = y;

	stbtt_aligned_quad q;
	stbtt_GetBakedQuad(cdata, 1024,1024, c-32, &x,&y,&q,1);//1=opengl & d3d10+,0=d3d9
	
	draw_texture_colored( font_tex,
			      vec2f(q.x0 * factor, _y + factor * (y - q.y1) ),
			      vec2f((q.x1 - q.x0) * factor, (q.y1 - q.y0) * factor),
			      vec2f(1 + q.s0, 1 + 1 - q.t1),
			      vec2f(q.s1 - q.s0, q.t1 - q.t0),
			      color);
    }
}

int main() {
  
  Window window;
  if(!window_init(&window, 800, 400, "Renderer")) {
    return 1;
  }

  fread(ttf_buffer, 1, 1<<20, fopen("c:/windows/fonts/segoeui.ttf", "rb"));
  stbtt_BakeFontBitmap(ttf_buffer,0, 64.0, temp_bitmap,1024,1024, 32,96, cdata);

  if(!push_texture(1024, 1024, temp_bitmap, true, &font_tex)) {
    return 1;
  }

  unsigned int tex;
  if(!push_texture(logo_width, logo_height, logo_data, false, &tex)) {
      return 1;
  }

  printf("tex: %u, font_tex: %u\n", tex, font_tex);

  int64_t time = 0.0f;

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

    Vec2f uv = vec2f(-1, -1);
    draw_triangle(vec2f(0, 0), vec2f(widthf, 0), vec2f(widthf, heightf),
		  vec4f(1, 0, 0, 1),
		  vec4f(0, 1, 0, 1),
		  vec4f(0, 0, 1, 1),
		  uv, uv, uv);

    draw_solid_triangle(vec2f(0, heightf), vec2f(0, heightf/2), vec2f(widthf/2, heightf),
			vec4f(1, 1, 0, .2) );

    draw_solid_rect( vec2f(0, 0), vec2f(100, 100), vec4f(0, 0, 1, 1) );
  
    draw_texture_colored( tex,
			  vec2f(100, 100),
			  vec2f(200, 200),
			  vec2f(0, 0),
			  vec2f(1, 1),
			  vec4f(1, 1, 1, 1) );
    print(0, heightf/2 , vec4f(1, 1, 1, 1), 1.f, "Thanks to stb_truetype!");

    draw_solid_circle(vec2f((float) mouse_x, (float) mouse_y),
		      10.0f,
		      20,
		      vec4f(1, 0, 0, 1) );

    draw_solid_rect_angle( vec2f(width/2 - 50, height/2 - 50), vec2f(100, 100), time * 6 / 4 * PI / 1000 , vec4f(1, 0, 0, .4) );
	
    window_renderer_end(&window_renderer);
    window_swap_buffers(&window);

    time += 16;
  }

  window_free(&window);
}
