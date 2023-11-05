#include <stdio.h>

#define FRAME_IMPLEMENTATION
#include "../src/frame.h"

int main() {

  Frame frame;
  if(!frame_init(&frame, 400, 400, "Frame", FRAME_DRAG_N_DROP)) {
    return 1;
  }
    
  Frame_Event event;
  while(frame.running) {
    while(frame_peek(&frame, &event)) {
      if(event.type == FRAME_EVENT_KEYPRESS) {
	if(event.as.key == 'q') {
	  frame.running = false;	  
	} else if(event.as.key == 'k') {	  
	  frame_toggle_fullscreen(&frame);
	}
      }
      else if(event.type == FRAME_EVENT_FILEDROP) {
	Frame_Dragged_Files files;
	if(frame_dragged_files_init(&files, &event)) {

	  char *path;
	  while(frame_dragged_files_next(&files, &path)) {
	    printf("%s\n", path);
	  }
	  fflush(stdout);
		
	  frame_dragged_files_free(&files);
	}
      }
    }

    glViewport(0, 0, frame.width, frame.height);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(1, 0, 0, 1);

    frame_swap_buffers(&frame);
  }

  frame_free(&frame);
  
  return 0;
}
