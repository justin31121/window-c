#define WINDOW_IMPLEMENTATION
#define WINDOW_VERBOSE // log errors
#include "../src/window.h"

void draw_solid_rounded_rect(Vec2f pos, Vec2f size, float radius, int parts, Vec4f color) {

    draw_solid_rect(
	vec2f(pos.x, pos.y + radius),
	vec2f(size.x, size.y - 2 * radius),
	color);

    draw_solid_rect(
	vec2f(pos.x + radius, pos.y),
	vec2f(size.x - 2 * radius, size.y),
	color);

    pos = vec2f(pos.x + radius, pos.y + radius);
    size = vec2f(size.x - 2 * radius, size.y - 2 * radius);

    //bottom left
    draw_solid_circle(
	pos,
	radius,
	parts,
	color);

    //bottom right
    draw_solid_circle(
	vec2f(pos.x + size.x, pos.y),
	radius,
	parts,
	color);

    //top left
    draw_solid_circle(
	vec2f(pos.x, pos.y + size.y),
	radius,
	parts,
	color);

    //top right
    draw_solid_circle(
	vec2f(pos.x + size.x, pos.y + size.y),
	radius,
	parts,
	color);
}

int main() {
  
    Window window;
    if(!window_init(&window, 400, 400, "Advanced", 0)) {
	return 1;
    }

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
      
	int r = 20;
	int parts = 40;
	Vec4f c = vec4f(0, 0, 1, 1);

#define N 8

	float w = (float) width / N;
	float h = (float) height / N;

	for(int i=0;i<N;i++) {
	    for(int j=0;j<N;j++) {
		if((j+i) % 2 == 0) {
		    c = vec4f(1, 0, 0, 1);
		} else {
		    c = vec4f(0, 0, 1, 1);
		}
		
		draw_solid_rounded_rect(
		    vec2f(w*i, h*j),
		    vec2f(w, h),
		    (float) r,
		    parts,
		    c);
	    }
	}
      


	window_renderer_end(width, height);
	window_swap_buffers(&window);
    }

    window_free(&window);
}

