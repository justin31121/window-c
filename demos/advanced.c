
#define STB_TRUETYPE_IMPLEMENTATION
#include "../thirdparty/stb_truetype.h"

#define WINDOW_IMPLEMENTATION
#define WINDOW_STB_TRUETYPE
#define WINDOW_VERBOSE // log errors
#include "../src/window.h"

int main() {
  
    Window window;
    if(!window_init(&window, 400, 400, "Advanced", 0)) {
	return 1;
    }

#define BLINK_MS 500
#define BLINK_DURATION_MS 5000

#define BACKSPACE_DT_MS 50

#define FONT_SIZE 32

    if(!push_font("c:\\windows\\fonts\\segoeui.ttf", FONT_SIZE)) {
	return 1;
    }

#define TINT 0.09411764705882353

    window_renderer_set_color(vec4f(TINT, TINT, TINT, 1));

    char buf[1024];
    int buf_size = 0;

    float DT = 1000.0f / 60.0f;

    bool backspace_pressed = false;
    float t_backspace = 0.0f;

    bool cursor_visible = true;
    float t_cursor = BLINK_MS;
    float t_cursor_total = BLINK_DURATION_MS;
  
    Window_Event event;
    while(window.running) {
	while(window_peek(&window, &event)) {
	    switch(event.type) {
	    case WINDOW_EVENT_KEYPRESS: {

		//printf("key: '%c' (%u)\n", event.as.key, event.as.key); fflush(stdout);
	  
		switch(event.as.key) {
		case WINDOW_ESCAPE: {
		    window.running = false;
		} break;
		case WINDOW_BACKSPACE: {
		    backspace_pressed = true;
		    t_backspace = 0.0f;
		} break;
		default: {
		    if( isprint(event.as.key) ) {

		      cursor_visible = true;
		      t_cursor = BLINK_MS;
		      t_cursor_total = BLINK_DURATION_MS;
	    
		      buf[buf_size++] = event.as.key;
		      if(buf_size >= sizeof(buf)) buf_size = sizeof(buf) - 1;
		      cursor_visible = true;
		      t_cursor = BLINK_MS;		
		    }
		    printf("%c %d\n", event.as.key, event.as.key); fflush(stdout);
		} break;
		}
	    } break;
	    case WINDOW_EVENT_KEYRELEASE: {
		switch(event.as.key) {
		case WINDOW_BACKSPACE: {
		    backspace_pressed = false;
		} break;
		default: {
		} break;
		}
	    } break;
	    default: {	      
	    } break;
	    }
	}

	if(backspace_pressed) {
      
	    cursor_visible = true;
	    t_cursor = BLINK_MS;
	    t_cursor_total = BLINK_DURATION_MS;

	    if(t_backspace == -1.0f) {
		buf_size--;
		if(buf_size < 0) buf_size = 0;
		cursor_visible = true;
		t_backspace = BACKSPACE_DT_MS;
		t_cursor = BLINK_MS;
		t_backspace = 0.0f;
	    } else {
		t_backspace += DT;
		if(t_backspace >= BACKSPACE_DT_MS) {
		    t_backspace = -1.0f;
		}
	    }


	}

	int width = window.width;
	int height = window.height;
            
	window_renderer_begin(width, height);

	//TEXT
	Vec2f size;
	float f = 1.f;

	//TEXTFIELD
	float padding = f * FONT_SIZE / 4;
	float w = width * 2 / 3 + 2 * padding;
	float h = f * FONT_SIZE + 2 * padding;

	Vec2f pos = vec2f(width/2 - w/2, height - 3 * f * FONT_SIZE);
	Vec2f text_pos = vec2f(pos.x + padding, pos.y + 2 * padding);

	draw_solid_rounded_rect(pos,
				vec2f(w, h),
				10.f,
				20,
				vec4f(1, 1, 1, 1));    

	measure_text_len(buf, buf_size, f, &size);
	draw_text_len_colored(buf, buf_size, text_pos, f, vec4f(0, 0, 0, 1));

	if(cursor_visible) {
	    draw_solid_rect(vec2f(text_pos.x + size.x, pos.y + padding),
			    vec2f(2, FONT_SIZE * f), vec4f(0, 0, 0, 1));          
	}
    
	window_renderer_end(width, height);
	window_swap_buffers(&window);

	t_cursor -= DT;
	t_cursor_total -= DT;
	if(t_cursor < 0.0f) {
	    if(t_cursor_total > 0.0f) {
		t_cursor = BLINK_MS;
		cursor_visible = !cursor_visible;	
	    }
	}
	if(t_cursor_total < 0.0f) {
	    t_cursor = 0.0f;
	    cursor_visible = true;
	    t_cursor_total = 0.0f;
	}
    }

    window_free(&window);
}

