#define FRAME_IMPLEMENTATION
#define FRAME_VERBOSE
#define FRAME_NO_RENDERER
#include "../src/frame.h"

#include <stdio.h>
#include <assert.h>

static const char* vertex_shader_source =
    "#version 330 core\n"
    "\n"
    "layout(location = 0) in vec2 position;\n"
    "layout(location = 1) in vec4 color;\n"
    "\n"
    "uniform float resolution_x;\n"
    "uniform float resolution_y;\n"
    "\n"
    "out vec4 out_color;\n"
    "\n"
    "vec2 resolution_project(vec2 point) {\n"
    "      return 2 * point / vec2(resolution_x, resolution_y) - 1;\n"
    "}\n"
    "\n"
    "void main() {\n"
    "  gl_Position = vec4(resolution_project(position), 1, 1);\n"
    "  out_color = color;\n"
    "}";

static const char *fragment_shader_source=
    "#version 330 core\n"
    "\n"
    "in vec4 out_color;\n"
    "\n"
    "void main() {\n"
    "  gl_FragColor = out_color;\n"
    "}\n";

typedef struct{
    float x, y;
}Vec2f;

Vec2f vec2f(float x, float y) { return (Vec2f) {x, y}; }

typedef struct{
    float x, y, z, w;
}Vec4f;

Vec4f vec4f(float x, float y, float z, float w) { return (Vec4f) {x, y, z, w}; }

typedef struct{
    Vec2f position; // VERTEX_ATTR_POSITION :: 0
    Vec4f color;    // VERTEX_ATTR_COLOR    :: 1
}Vertex;

// enumarte every attribute of 'Vertex'
#define VERTEX_ATTR_POSITION 0
#define VERTEX_ATTR_COLOR 1

// static of pool of verticies that can be rendered
#define VERTEX_CAP 1024
static Vertex verticies[VERTEX_CAP];
int verticies_count = 0;

void push_vertex(Vec2f p, Vec4f c) {
    assert(verticies_count < VERTEX_CAP);
    Vertex *last = &verticies[verticies_count];
    last->position = p;
    last->color = c;
    verticies_count++;
}

void push_triangle(Vec2f p1, Vec2f p2, Vec2f p3,
		   Vec4f c1, Vec4f c2, Vec4f c3) {
    push_vertex(p1, c1);
    push_vertex(p2, c2);
    push_vertex(p3, c3);
}

int main() {

    Frame frame;
    if(!frame_init(&frame, 800, 800, "Frame", 0)) {
	return 1;
    }

    GLuint vao, vbo;
  
    // introduce 'verticies' to opengl
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_DYNAMIC_DRAW);

    // introduce 'Vertex' to opengl
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glVertexAttribPointer(VERTEX_ATTR_POSITION,
			  sizeof(Vec2f)/sizeof(float),
			  GL_FLOAT,
			  GL_FALSE,
			  sizeof(Vertex),
			  (GLvoid *) offsetof(Vertex, position));

    glEnableVertexAttribArray(VERTEX_ATTR_COLOR);
    glVertexAttribPointer(VERTEX_ATTR_COLOR,
			  sizeof(Vec4f)/sizeof(float),
			  GL_FLOAT,
			  GL_FALSE,
			  sizeof(Vertex),
			  (GLvoid *) offsetof(Vertex, color));
  
    // compile shaders
    GLuint vertex_shader;
    if(!frame_compile_shader(&vertex_shader, GL_VERTEX_SHADER, vertex_shader_source)) {
	return 1;
    }

    GLuint fragment_shader;
    if(!frame_compile_shader(&fragment_shader, GL_FRAGMENT_SHADER, fragment_shader_source)) {
	return 1;
    }

    // link program
    GLuint program;
    if(!frame_link_program(&program, vertex_shader, fragment_shader)) {
	return 1;
    }
    glUseProgram(program);

    int width, height;
    float mouse_x = frame.width;
    float mouse_y = frame.height;
    bool dragging = false;
    
    Frame_Event event;
    while(frame.running) {
	while(frame_peek(&frame, &event)) {
	    if(event.type == FRAME_EVENT_KEYPRESS && event.as.key == 'Q') frame.running = false;
	    else if(event.type == FRAME_EVENT_MOUSEPRESS) dragging = true;
	    else if(event.type == FRAME_EVENT_MOUSERELEASE) dragging = false;
	}

	width = frame.width;
        height = frame.height;
	if(dragging) {
	    if(!frame_get_mouse_position(&frame, &mouse_x, &mouse_y)) return 1;
	    if(mouse_x > width) mouse_x = width;
	    if(mouse_x < 0) mouse_x = 0;
	    if(mouse_y > height) mouse_y = height;
	    if(mouse_y < 0) mouse_y = 0;
	}

	glEnable( GL_BLEND );
	//glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	glEnable( GL_POINT_SMOOTH );
	glEnable( GL_LINE_SMOOTH );
    
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0, 0, 0, 1);

	float widthf = (float) width;
	float heightf = (float) height;

	// tell vertex shader what is the resolution is
	glUniform1fv(glGetUniformLocation(program, "resolution_x"), 1, &widthf);
	glUniform1fv(glGetUniformLocation(program, "resolution_y"), 1, &heightf);

	// render triangles:
	push_triangle(vec2f(0, heightf/2), vec2f(widthf/2, heightf), vec2f((float) mouse_x, (float) mouse_y),
		      vec4f(1, 0, 0, 1),
		      vec4f(0, 1, 0, 1),
		      vec4f(0, 0, 1, 1));

	// tell opengl to render 'verticies_count' verticies, as triangles
	glBufferSubData(GL_ARRAY_BUFFER, 0, verticies_count * sizeof(Vertex), verticies);
	glDrawArrays(GL_TRIANGLES, 0, verticies_count);
	verticies_count = 0;

	frame_swap_buffers(&frame);
    }

    frame_free(&frame);
  
    return 0;
}
