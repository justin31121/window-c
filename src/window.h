#ifndef WINDOW_H
#define WINDOW_H

// win32
//   msvc : user32.lib gdi32.lib opengl32.lib
//   mingw:

#ifdef WINDOW_VERBOSE
#  include <stdio.h>
#  define WINDOW_LOG(...) fprintf(stderr, __VA_ARGS__);
#else
#  define WINDOW_LOG(...)
#endif //WINDOW_VERBOSE

#include <stdbool.h>
#include <math.h>

#include <windows.h>
#include <GL/GL.h>

#ifndef WINDOW_DEF
#  define WINDOW_DEF static inline
#endif //WINDOW_DEF

typedef enum{
    WINDOW_EVENT_NONE = 0,
    WINDOW_EVENT_KEYPRESS,
    WINDOW_EVENT_KEYRELEASE,
    WINDOW_EVENT_MOUSEPRESS,
    WINDOW_EVENT_MOUSERELEASE,
}Window_Event_Type;

typedef struct{
    MSG msg;
    Window_Event_Type type;
    union{
	char key;
    }as;
}Window_Event;

typedef struct{
    HWND hwnd;
    HDC dc;
    RECT rect;
    POINT point;
    bool running;
}Window;

WINDOW_DEF bool window_init(Window *w, int width, int height, const char *title);
WINDOW_DEF bool window_set_vsync(Window *w, bool use_vsync);
WINDOW_DEF bool window_peek(Window *w, Window_Event *event);
WINDOW_DEF bool window_get_window_size(Window *w, int *width, int *height);
WINDOW_DEF bool window_get_mouse_position(Window *w, int *width, int *height);
WINDOW_DEF void window_swap_buffers(Window *w);
WINDOW_DEF void window_free(Window *w);

WINDOW_DEF bool window_compile_shader(GLuint *shader, GLenum shader_type, const char *shader_source);
WINDOW_DEF bool window_link_program(GLuint *program, GLuint vertex_shader, GLuint fragment_shader);

#ifndef WINDOW_NO_RENDERER

typedef struct{
    float x, y;
}Window_Renderer_Vec2f;

WINDOW_DEF Window_Renderer_Vec2f window_renderer_vec2f(float x, float y);

typedef struct{
    float x, y, z, w;
}Window_Renderer_Vec4f;

WINDOW_DEF Window_Renderer_Vec4f window_renderer_vec4f(float x, float y, float z, float w);

typedef struct{
    Window_Renderer_Vec2f position;
    Window_Renderer_Vec4f color;
    Window_Renderer_Vec2f uv;
}Window_Renderer_Vertex;

#define WINDOW_RENDERER_VERTEX_ATTR_POSITION 0
#define WINDOW_RENDERER_VERTEX_ATTR_COLOR 1
#define WINDOW_RENDERER_VERTEX_ATTR_UV 2

#define WINDOW_RENDERER_CAP 1024

typedef struct{
    GLuint vao, vbo;
    GLuint vertex_shader, fragment_shader;
    GLuint program;
  
    GLuint textures;
    unsigned int images_count;

#ifdef __STB_INCLUDE_STB_TRUETYPE_H__
    stbtt_bakedchar font_cdata[96]; // ASCII 32..126 is 95 glyphs
#endif //__STB_INCLUDE_STB_TRUETYPE_H__
    
    int font_index;    
    int tex_index;

    Window_Renderer_Vertex verticies[WINDOW_RENDERER_CAP];
    int verticies_count;
}Window_Renderer;

static Window_Renderer_Vec4f WHITE = {1, 1, 1, 1};
static Window_Renderer_Vec4f RED   = {1, 0, 0, 1};
static Window_Renderer_Vec4f BLUE  = {0, 0, 1, 1};
static Window_Renderer_Vec4f GREEN = {0, 1, 0, 1};
static Window_Renderer_Vec4f BLACK = {0, 0, 0, 1};

#define Vec2f Window_Renderer_Vec2f
#define vec2f(x, y) window_renderer_vec2f((x), (y))
#define Vec4f Window_Renderer_Vec4f
#define vec4f(x, y, z, w) window_renderer_vec4f((x), (y), (z), (w))
#define draw_triangle(p1, p2, p3, c1, c2, c3, uv1, uv2, uv3) window_renderer_triangle(&(window_renderer), (p1), (p2), (p3), (c1), (c2), (c3), (uv1), (uv2), (uv3))
#define draw_solid_triangle(p1, p2, p3, c1) window_renderer_solid_triangle(&(window_renderer), (p1), (p2), (p3), c1)
#define draw_solid_rect(pos, size, color) window_renderer_solid_rect(&(window_renderer), (pos), (size), (color))
#define draw_solid_rect_angle(pos, size, angle, color) window_renderer_solid_rect_angle(&(window_renderer), (pos), (size), (angle), (color))
#define push_texture(w, h, d, g, index) window_renderer_push_texture(&(window_renderer), (w), (h), (d), (g), (index))
#define draw_texture(t, p, s, uvp, uvs) window_renderer_texture_colored(&(window_renderer), (t), (p), (s), (uvp), (uvs), (WHITE))
#define draw_texture_colored(t, p, s, uvp, uvs, c) window_renderer_texture_colored(&(window_renderer), (t), (p), (s), (uvp), (uvs), (c))
#define draw_solid_circle(p, r, parts, c) window_renderer_solid_circle(&(window_renderer), (p), (r), (parts), (c))

#ifdef __STB_INCLUDE_STB_TRUETYPE_H__
#  define push_font(filepath, h) window_renderer_push_font(&(window_renderer), (filepath), (h))
#  define draw_text(cstr, pos, factor) window_renderer_text(&(window_renderer), (cstr), strlen((cstr)), (pos), (factor), (WHITE))
#  define draw_text_colored(cstr, pos, factor, color) window_renderer_text(&(window_renderer), (cstr), strlen((cstr)), (pos), (factor), (color))
#  define draw_text_len(cstr, cstr_len, pos, factor) window_renderer_text(&(window_renderer), (cstr), (cstr_len), (pos), (factor), (WHITE))
#  define draw_text_len_colored(cstr, cstr_len, pos, factor, color) window_renderer_text(&(window_renderer), (cstr), (cstr_len), (pos), (factor), (color))
#endif //__STB_INCLUDE_STB_TRUETYPE_H__

WINDOW_DEF bool window_renderer_init(Window_Renderer *r);
WINDOW_DEF void window_renderer_begin(Window_Renderer *r, int width, int height);
WINDOW_DEF void window_renderer_vertex(Window_Renderer *r, Window_Renderer_Vec2f p, Window_Renderer_Vec4f c, Window_Renderer_Vec2f uv);
WINDOW_DEF void window_renderer_triangle(Window_Renderer *r, Window_Renderer_Vec2f p1, Window_Renderer_Vec2f p2, Window_Renderer_Vec2f p3, Window_Renderer_Vec4f c1, Window_Renderer_Vec4f c2, Window_Renderer_Vec4f c3, Window_Renderer_Vec2f uv1, Window_Renderer_Vec2f uv2, Window_Renderer_Vec2f uv3);
WINDOW_DEF void window_renderer_solid_triangle(Window_Renderer *r, Window_Renderer_Vec2f p1, Window_Renderer_Vec2f p2, Window_Renderer_Vec2f p3, Window_Renderer_Vec4f c);
WINDOW_DEF void window_renderer_quad(Window_Renderer *r, Window_Renderer_Vec2f p1, Window_Renderer_Vec2f p2, Window_Renderer_Vec2f p3, Window_Renderer_Vec2f p4,Window_Renderer_Vec4f c1,Window_Renderer_Vec4f c2,Window_Renderer_Vec4f c3,Window_Renderer_Vec4f c4, Window_Renderer_Vec2f uv1, Window_Renderer_Vec2f uv2, Window_Renderer_Vec2f uv3, Window_Renderer_Vec2f uv4);
WINDOW_DEF void window_renderer_solid_rect(Window_Renderer *r, Window_Renderer_Vec2f pos, Window_Renderer_Vec2f size,Window_Renderer_Vec4f color);
WINDOW_DEF void window_renderer_solid_rect_angle(Window_Renderer *r, Window_Renderer_Vec2f pos, Window_Renderer_Vec2f size, float angle, Window_Renderer_Vec4f color);
WINDOW_DEF bool window_renderer_push_texture(Window_Renderer *r, int width, int height, const void *data, bool grey, unsigned int *index);
WINDOW_DEF void window_renderer_texture(Window_Renderer *r, unsigned int texture, Window_Renderer_Vec2f p, Window_Renderer_Vec2f s, Window_Renderer_Vec2f uvp, Window_Renderer_Vec2f uvs);
WINDOW_DEF void window_renderer_texture_colored(Window_Renderer *r, unsigned int texture, Window_Renderer_Vec2f p, Window_Renderer_Vec2f s, Window_Renderer_Vec2f uvp, Window_Renderer_Vec2f uvs, Window_Renderer_Vec4f c);
WINDOW_DEF void window_renderer_solid_circle(Window_Renderer *r, Window_Renderer_Vec2f pos, float radius, int parts, Window_Renderer_Vec4f color);
WINDOW_DEF void window_renderer_end(Window_Renderer *r);
WINDOW_DEF void window_renderer_free(Window_Renderer *r);

#ifdef __STB_INCLUDE_STB_TRUETYPE_H__
WINDOW_DEF bool window_renderer_push_font(Window_Renderer *r, const char *filepath, float pixel_height);
WINDOW_DEF void window_renderer_text(Window_Renderer *r, const char *cstr, size_t cstr_len, Window_Renderer_Vec2f pos, float scale, Window_Renderer_Vec4f color);
#endif //__STB_INCLUDE_STB_TRUETYPE_H__

#endif //WINDOW_NO_RENDERER


// opengl - functions / types / definitions
#define GL_TEXTURE0 0x84C0
#define GL_TEXTURE1 0x84C1
#define GL_TEXTURE2 0x84C2
#define GL_TEXTURE3 0x84C3
#define GL_TEXTURE4 0x84C4
#define GL_TEXTURE5 0x84C5

#define GL_ARRAY_BUFFER 0x8892
#define GL_DYNAMIC_DRAW 0x88E8
#define GL_ACTIVE_UNIFORMS 0x8B86

#define GL_FRAGMENT_SHADER 0x8B30
#define GL_VERTEX_SHADER 0x8B31
#define GL_COMPILE_STATUS 0x8B81
#define GL_LINK_STATUS 0x8B82
#define GL_CLAMP_TO_EDGE 0x812F
#define GL_RGB_INTEGER 0x8D98
#define GL_UNSIGNED_INT_8_8_8_8_REV 0x8367
#define GL_MULTISAMPLE  0x809D
#define GL_SAMPLES 0x80A9

#define GL_BGRA 0x80E1
#define GL_RGB 0x1907
#define GL_BGR 0x80E0

#define GL_MULTISAMPLE 0x809D
#define GL_MULTISAMPLE_ARB 0x809D
#define GL_MULTISAMPLE_BIT 0x20000000
#define GL_MULTISAMPLE_BIT_ARB 0x20000000
#define GL_MULTISAMPLE_FILTER_HINT_NV 0x8534
#define GL_SAMPLE_ALPHA_TO_COVERAGE 0x809E
#define GL_SAMPLE_BUFFERS 0x80A8
#define GL_SAMPLES 0x80A9

typedef ptrdiff_t GLsizeiptr;
typedef ptrdiff_t GLintptr;
typedef char GLchar;

void glActiveTexture(GLenum texture);
void glGenVertexArrays(GLsizei n, GLuint *arrays);
void glBindVertexArray(GLuint array);
void glGenBuffers(GLsizei n, GLuint *buffers);
void glBindBuffer(GLenum target, GLuint buffer);
void glBufferData(GLenum target, GLsizeiptr size, const void * data, GLenum usage);
void glEnableVertexAttribArray(GLuint index);
void glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void * pointer);
GLuint glCreateShader(GLenum shaderType);
void glShaderSource(GLuint shader, GLsizei count, const GLchar **_string, const GLint *length);
void glCompileShader(GLuint shader);
void glGetShaderiv(GLuint shader, GLenum pname, GLint *params);
void glGetShaderInfoLog(GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infoLog);
GLuint glCreateProgram(void);
void glAttachShader(GLuint program, GLuint shader);
void glLinkProgram(GLuint program);
void glGetProgramInfoLog(GLuint program, GLsizei maxLength, GLsizei *length, GLchar *infoLog);
void glGetProgramiv(GLuint program, GLenum pname, GLint *params);
void glUseProgram(GLuint program);
void glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const void * data);
void glUniform2f(GLint location, GLfloat v0, GLfloat v1);
void glUniform1f(GLint location, GLfloat v0);
void glUniform1fv(GLint location, GLsizei count, const GLfloat *value);
void glUniform2fv(GLint location, GLsizei count, const GLfloat *value);
void glUniform1i(GLint location, GLint v0);
GLint glGetUniformLocation(GLuint program, const GLchar *name);
void glGetActiveUniform(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
void glGetUniformfv(GLuint program, GLint location, GLfloat *params);
void glGetUniformiv(GLuint program, GLint location, GLsizei bufSize, GLint *params);
void glSampleCoverage(GLfloat value, GLboolean invert);
int wglSwapIntervalEXT(GLint interval);

#ifdef WINDOW_IMPLEMENTATION

#ifndef WINDOW_NO_RENDERER
static Window_Renderer window_renderer;
static bool window_renderer_inited = false;
#endif //WINDOW_NO_RENDERER

LRESULT CALLBACK Window_Implementation_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

    if(message == WM_CLOSE) {
	Window *w = (Window *) GetWindowLongPtr(hWnd, 0);
	if(w != NULL) {
	    w->running = false;
	}
	PostQuitMessage(0);
	return 0;
    } else if(message == WM_DESTROY) {
	Window *w = (Window *) GetWindowLongPtr(hWnd, 0);
	if(w != NULL) {
	    w->running = false;
	}
	PostQuitMessage(0);
	return 0;
    } else {
	return DefWindowProc(hWnd, message, wParam, lParam);
    }
  
}

WINDOW_DEF void window_win32_opengl_init();

WINDOW_DEF bool window_init(Window *w, int width, int height, const char *title) {

    STARTUPINFO startupInfo;
    GetStartupInfo(&startupInfo);
    HMODULE hInstance = GetModuleHandle(NULL);
    DWORD nCmdShow = startupInfo.wShowWindow;

    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = Window_Implementation_WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = title;
    wc.cbWndExtra = sizeof(LONG_PTR);
    wc.cbClsExtra = 0;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    if(!RegisterClassEx(&wc)) {
	return false;
    }

    int screen_width = GetSystemMetrics(SM_CXSCREEN);
    int screen_height = GetSystemMetrics(SM_CYSCREEN);

    // WS_THICKFRAME :: resizable
    // WS_MAXIMIZEBOX :: maximizable

    DWORD style = (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_THICKFRAME | WS_MAXIMIZEBOX);

    //add space for taskbar
    height += 39;

    w->hwnd = CreateWindowEx(0,
			     wc.lpszClassName,
			     wc.lpszClassName,
			     style,
			     screen_width / 2 - width/2,
			     screen_height / 2 - height/2,
			     width,
			     height,
			     NULL,
			     NULL,
			     hInstance,
			     NULL);
    if(!w->hwnd) {
	return false;
    }
    w->dc = GetDC(w->hwnd);

    //BEGIN opengl
    HDC w_dc = GetDC(w->hwnd);

    PIXELFORMATDESCRIPTOR desired_format = {0};
    desired_format.nSize = sizeof(desired_format);
    desired_format.nVersion = 1;
    desired_format.dwFlags = PFD_SUPPORT_OPENGL|PFD_DRAW_TO_WINDOW|PFD_DOUBLEBUFFER;
    desired_format.cColorBits = 32;
    desired_format.cAlphaBits = 8;

    int suggested_format_index = ChoosePixelFormat(w_dc, &desired_format);
    PIXELFORMATDESCRIPTOR suggested_format;
    DescribePixelFormat(w_dc, suggested_format_index, sizeof(suggested_format), &suggested_format);
    SetPixelFormat(w_dc, suggested_format_index, &suggested_format);
  
    HGLRC opengl_rc = wglCreateContext(w_dc);
    if(!wglMakeCurrent(w_dc, opengl_rc)) {
	return false;
    }
    ReleaseDC(w->hwnd, w_dc);
    //END opengl

    LONG_PTR lptr = {0};
    memcpy(&lptr, &w, sizeof(w));  
    SetWindowLongPtr(w->hwnd, 0, lptr);  

    ShowWindow(w->hwnd, nCmdShow);
    UpdateWindow(w->hwnd);

    w->running = true;

    // load non-default-opengl-functions
    window_win32_opengl_init();

#ifndef WINDOW_NO_RENDERER
    if(!window_renderer_inited) {
	if(!window_renderer_init(&window_renderer)) {
	    return false;	    
	}
	
	window_renderer_inited = true;
    }
#endif //WINDOW_NO_RENDERER

    // use vsync as default
    if(!window_set_vsync(w, true)) {
	return false;
    }

    return true;
}

WINDOW_DEF bool window_set_vsync(Window *w, bool use_vsync) {
    (void) w;
    return wglSwapIntervalEXT(use_vsync ? 1 : 0);
}

WINDOW_DEF bool window_peek(Window *w, Window_Event *e) {

    MSG *msg = &e->msg;

    while(true) {
	if(!PeekMessage(msg, w->hwnd, 0, 0, PM_REMOVE)) {
	    return false;
	}

	TranslateMessage(msg);
	DispatchMessage(msg);

	e->type = WINDOW_EVENT_NONE;

	switch(msg->message) {
	case WM_RBUTTONUP:  {
	    e->type = WINDOW_EVENT_MOUSERELEASE;
	    e->as.key = 'R';
	    ReleaseCapture();
	} break;
	case WM_RBUTTONDOWN: {
	    e->type = WINDOW_EVENT_MOUSEPRESS;
	    e->as.key = 'R';
	    SetCapture(w->hwnd);
	} break;
	case WM_LBUTTONUP: {
	    e->type = WINDOW_EVENT_MOUSERELEASE;
	    e->as.key = 'L';
	    ReleaseCapture();
	} break;
	case WM_LBUTTONDOWN: {
	    e->type = WINDOW_EVENT_MOUSEPRESS;
	    e->as.key = 'L';
	    SetCapture(w->hwnd);
	} break;
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	case WM_KEYDOWN:
	case WM_KEYUP: {
	    bool was_down = ((msg->lParam & (1 << 30)) != 0);
	    bool is_down = ((msg->lParam & (1 << 31)) == 0);

	    if(was_down != is_down) {
		e->as.key = (char) msg->wParam;
		if(was_down) {
		    e->type = WINDOW_EVENT_KEYRELEASE;
		} else {
		    e->type = WINDOW_EVENT_KEYPRESS;
		}
	    }
      
	} break;
	default: {
	} break;
	}

	if(e->type != WINDOW_EVENT_NONE) {
	    return true;
	}
    }

    return false;
}

WINDOW_DEF bool window_get_window_size(Window *w, int *width, int *height) {
    bool result = GetClientRect(w->hwnd, &w->rect);
    if(result) {
	*width = (w->rect.right - w->rect.left);
	*height = (w->rect.bottom - w->rect.top);
    }
    return result;
}

WINDOW_DEF bool window_get_mouse_position(Window *w, int *width, int *height) {
    if(GetCursorPos(&w->point) && ScreenToClient(w->hwnd, &w->point)) {
	*width = w->point.x;
	*height = w->point.y;
	return true;
    }

    return false;
}
  
WINDOW_DEF void window_swap_buffers(Window *w) {
    SwapBuffers(w->dc);
}

WINDOW_DEF void window_free(Window *w) {	
    ReleaseDC(w->hwnd, w->dc);
    DestroyWindow(w->hwnd);
}

WINDOW_DEF const char *window_shader_type_name(GLenum shader) {
    switch (shader) {
    case GL_VERTEX_SHADER:
	return "GL_VERTEX_SHADER";
    case GL_FRAGMENT_SHADER:
	return "GL_FRAGMENT_SHADER";
    default:
	return "(Unknown)";
    }
}

WINDOW_DEF bool window_compile_shader(GLuint *shader, GLenum shader_type, const char *shader_source) {
    *shader = glCreateShader(shader_type);
    glShaderSource(*shader, 1, &shader_source, NULL);
    glCompileShader(*shader);

    GLint compiled = 0;
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &compiled);

    if (!compiled) {
	GLchar message[1024];
	GLsizei message_size = 0;
	glGetShaderInfoLog(*shader, sizeof(message), &message_size, message);
	WINDOW_LOG("ERROR: could not compile %s\n", window_shader_type_name(shader_type));
	WINDOW_LOG("%.*s\n", message_size, message);
	return false;
    }

    return true;
}

WINDOW_DEF bool window_link_program(GLuint *program, GLuint vertex_shader, GLuint fragment_shader) {
    *program = glCreateProgram();
    glAttachShader(*program, vertex_shader);
    glAttachShader(*program, fragment_shader);

    glLinkProgram(*program);
  
    GLint linked = 0;
    glGetProgramiv(*program, GL_LINK_STATUS, &linked);
    if(!linked) {
	GLsizei message_size = 0;
	GLchar message[1024];

	glGetProgramInfoLog(*program, sizeof(message), &message_size, message);
	WINDOW_LOG("ERROR: Program Linking: %.*s\n", message_size, message);
	return false;
    }
  
    return true;
    
}

////////////////////////////////////////////////////////////////////////
// renderer - definitions
////////////////////////////////////////////////////////////////////////

#ifndef WINDOW_NO_RENDERER

static const char* window_renderer_vertex_shader_source =
    "#version 330 core\n"
    "\n"
    "layout(location = 0) in vec2 position;\n"
    "layout(location = 1) in vec4 color;\n"
    "layout(location = 2) in vec2 uv;\n"
    "\n"
    "uniform float resolution_x;\n"
    "uniform float resolution_y;\n"
    "\n"
    "out vec4 out_color;\n"
    "out vec2 out_uv;\n"
    "\n"
    "vec2 resolution_project(vec2 point) {\n"
    "    return 2 * point / vec2(resolution_x, resolution_y) - 1;\n"
    "}\n"
    "\n"
    "void main() {\n"
    "  out_color = color;\n"
    "  out_uv = uv;\n"
    "  gl_Position = vec4(resolution_project(position), 0, 1);\n"
    "}";

static const char *window_renderer_fragment_shader_source=
    "#version 330 core\n"
    "\n"
    "uniform sampler2D font_tex;\n"
    "uniform sampler2D texture;\n"
    "\n"
    "in vec4 out_color;\n"
    "in vec2 out_uv;\n"
    "\n"
    "out vec4 fragColor;\n"
    "\n"
    "void main() {\n"
    "    if(out_uv.x < 0 && out_uv.y < 0) {\n"
    "        fragColor = out_color;\n"
    "    } else if(out_color.w < 0) {\n"
    "        vec4 color = texture(font_tex, vec2(out_uv.x, 1-out_uv.y));\n"
    "        float a = color.w * -out_color.w;\n"
    "        color = (color + vec4(1, 1, 1, 0)) * out_color;\n"
    "        color.w = a;\n"
    "        fragColor = color;\n"
    "    } else {\n"
    "        vec4 color = texture(texture, vec2(out_uv.x, 1-out_uv.y));\n"
    "        color = color * out_color;\n"
    "        fragColor = color;\n"
    "    }\n"
    "}\n";

WINDOW_DEF Window_Renderer_Vec2f window_renderer_vec2f(float x, float y) {
    return (Window_Renderer_Vec2f) { x, y};
}

WINDOW_DEF Window_Renderer_Vec4f window_renderer_vec4f(float x, float y, float z, float w) {
    return (Window_Renderer_Vec4f) { x, y, z, w};
}

WINDOW_DEF bool window_renderer_init(Window_Renderer *r) {
    (void) WHITE;
    (void) RED;
    (void) BLUE;
    (void) GREEN;
    (void) BLACK;
    
    // introduce 'verticies' to opengl
    glGenVertexArrays(1, &r->vao);
    glBindVertexArray(r->vao);

    glGenBuffers(1, &r->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, r->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(r->verticies), r->verticies, GL_DYNAMIC_DRAW);

    // introduce 'Vertex' to opengl
    glEnableVertexAttribArray(WINDOW_RENDERER_VERTEX_ATTR_POSITION);
    glVertexAttribPointer(WINDOW_RENDERER_VERTEX_ATTR_POSITION,
			  sizeof(Window_Renderer_Vec2f)/sizeof(float),
			  GL_FLOAT,
			  GL_FALSE,
			  sizeof(Window_Renderer_Vertex),
			  (GLvoid *) offsetof(Window_Renderer_Vertex, position));

    glEnableVertexAttribArray(WINDOW_RENDERER_VERTEX_ATTR_COLOR);
    glVertexAttribPointer(WINDOW_RENDERER_VERTEX_ATTR_COLOR,
			  sizeof(Window_Renderer_Vec4f)/sizeof(float),
			  GL_FLOAT,
			  GL_FALSE,
			  sizeof(Window_Renderer_Vertex),
			  (GLvoid *) offsetof(Window_Renderer_Vertex, color));

    glEnableVertexAttribArray(WINDOW_RENDERER_VERTEX_ATTR_UV);
    glVertexAttribPointer(WINDOW_RENDERER_VERTEX_ATTR_UV,
			  sizeof(Window_Renderer_Vec2f)/sizeof(float),
			  GL_FLOAT,
			  GL_FALSE,
			  sizeof(Window_Renderer_Vertex),
			  (GLvoid *) offsetof(Window_Renderer_Vertex, uv));
  
    // compile shaders
    if(!window_compile_shader(&r->vertex_shader, GL_VERTEX_SHADER, window_renderer_vertex_shader_source)) {
	return false;
    }
    
    if(!window_compile_shader(&r->fragment_shader, GL_FRAGMENT_SHADER, window_renderer_fragment_shader_source)) {
	return false;
    }

    // link program
    if(!window_link_program(&r->program, r->vertex_shader, r->fragment_shader)) {
	return false;
    }
    glUseProgram(r->program);


    r->images_count = 0;
    r->verticies_count = 0;
    r->font_index = -1;
  
    return true;
}

WINDOW_DEF void window_renderer_begin(Window_Renderer *r, int width, int height) {

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0, 0, 0, 1);

    float widthf = (float) width;
    float heightf = (float) height;

    // tell vertex shader what is the resolution is
    glUniform1fv(glGetUniformLocation(r->program, "resolution_x"), 1, &widthf);
    glUniform1fv(glGetUniformLocation(r->program, "resolution_y"), 1, &heightf);

    if(r->font_index > 0) {
	GLint uniformLocation1 = glGetUniformLocation(r->program, "font_tex");
	glUniform1i(uniformLocation1, r->font_index);	
    }

    r->tex_index = -1;
}

WINDOW_DEF void window_renderer_vertex(Window_Renderer *r, Window_Renderer_Vec2f p, Window_Renderer_Vec4f c, Window_Renderer_Vec2f uv) {
    if(r->verticies_count < WINDOW_RENDERER_CAP) {
	Window_Renderer_Vertex *last = &r->verticies[r->verticies_count];
	last->position = p;
	last->color = c;
	last->uv = uv;
	r->verticies_count++;    	
    } else {
	// maybe flush ?
    }
}

WINDOW_DEF void window_renderer_triangle(Window_Renderer *r, Window_Renderer_Vec2f p1, Window_Renderer_Vec2f p2, Window_Renderer_Vec2f p3, Window_Renderer_Vec4f c1, Window_Renderer_Vec4f c2, Window_Renderer_Vec4f c3, Window_Renderer_Vec2f uv1, Window_Renderer_Vec2f uv2, Window_Renderer_Vec2f uv3) {
    window_renderer_vertex(r, p1, c1, uv1);
    window_renderer_vertex(r, p2, c2, uv2);
    window_renderer_vertex(r, p3, c3, uv3);    
}

WINDOW_DEF void window_renderer_solid_triangle(Window_Renderer *r, Window_Renderer_Vec2f p1, Window_Renderer_Vec2f p2, Window_Renderer_Vec2f p3, Window_Renderer_Vec4f c) {
    Window_Renderer_Vec2f uv = window_renderer_vec2f(-1, -1);
    window_renderer_vertex(r, p1, c, uv);
    window_renderer_vertex(r, p2, c, uv);
    window_renderer_vertex(r, p3, c, uv);
}

WINDOW_DEF void window_renderer_quad(Window_Renderer *r, Window_Renderer_Vec2f p1, Window_Renderer_Vec2f p2, Window_Renderer_Vec2f p3, Window_Renderer_Vec2f p4, Window_Renderer_Vec4f c1, Window_Renderer_Vec4f c2, Window_Renderer_Vec4f c3, Window_Renderer_Vec4f c4, Window_Renderer_Vec2f uv1, Window_Renderer_Vec2f uv2, Window_Renderer_Vec2f uv3, Window_Renderer_Vec2f uv4) {
    window_renderer_triangle(r, p1, p2, p4, c1, c2, c4, uv1, uv2, uv4);
    window_renderer_triangle(r, p1, p3, p4, c1, c3, c4, uv1, uv3, uv4);
}

WINDOW_DEF void window_renderer_solid_rect(Window_Renderer *r, Window_Renderer_Vec2f pos, Window_Renderer_Vec2f size, Window_Renderer_Vec4f color) {
    Vec2f uv = vec2f(-1, -1);
    window_renderer_quad(r, pos,
			 window_renderer_vec2f(pos.x + size.y, pos.y),
			 window_renderer_vec2f(pos.x, pos.y+size.y),
			 window_renderer_vec2f(pos.x + size.x, pos.y + size.y),
			 color, color, color, color, uv, uv, uv, uv);
}

WINDOW_DEF void window_renderer_solid_rect_angle(Window_Renderer *r, Window_Renderer_Vec2f pos, Window_Renderer_Vec2f size, float angle, Window_Renderer_Vec4f color) {
    Vec2f uv = vec2f(-1, -1);

    float s = sinf(angle);
    float c = cosf(angle);

    float dy1 = s * size.x;
    float dx1 = c * size.x;
  
    float dy2 = c * size.y;
    float dx2 = s * size.y;

    window_renderer_triangle(r,
			     pos,
			     window_renderer_vec2f(pos.x + dx1, pos.y + dy1),
			     window_renderer_vec2f(pos.x - dx2, pos.y + dy2),
			     color, color, color, uv, uv, uv);

    window_renderer_triangle(r,
			     window_renderer_vec2f(pos.x + dx1, pos.y + dy1),
			     window_renderer_vec2f(pos.x - dx2, pos.y + dy2),
			     window_renderer_vec2f(pos.x + dx1 - dx2, pos.y + dy1 + dy2),
			     color, color, color, uv, uv, uv);
}

WINDOW_DEF void window_renderer_texture(Window_Renderer *r, unsigned int texture, Window_Renderer_Vec2f p, Window_Renderer_Vec2f s, Window_Renderer_Vec2f uvp, Window_Renderer_Vec2f uvs)
{

    if(r->tex_index == -1) {
	r->tex_index = (int) texture;
	GLint uniformLocation1 = glGetUniformLocation(r->program, "texture");
	glUniform1i(uniformLocation1, r->tex_index);
    } else {
	//maybe flush ?
    }
    
    Vec4f c = vec4f(1, 1, 1, 1);
    window_renderer_quad(
	r,
	p,
	window_renderer_vec2f(p.x + s.x, p.y),
	window_renderer_vec2f(p.x, p.y + s.y),
	window_renderer_vec2f(p.x + s.x, p.y + s.y),
	c, c, c, c,
	uvp,
	window_renderer_vec2f(uvp.x + uvs.x, uvp.y),
	window_renderer_vec2f(uvp.x, uvp.y + uvs.y),
	window_renderer_vec2f(uvp.x + uvs.x, uvp.y + uvs.y));
}

WINDOW_DEF void window_renderer_texture_colored(Window_Renderer *r, unsigned int texture, Window_Renderer_Vec2f p, Window_Renderer_Vec2f s, Window_Renderer_Vec2f uvp, Window_Renderer_Vec2f uvs, Window_Renderer_Vec4f c)
{
    if(r->tex_index == -1) {
	r->tex_index = (int) texture;
	GLint uniformLocation1 = glGetUniformLocation(r->program, "texture");
	glUniform1i(uniformLocation1, r->tex_index);
    } else {
	//maybe flush ? 
    }
  
    window_renderer_quad(
	r,
	p,
	window_renderer_vec2f(p.x + s.x, p.y),
	window_renderer_vec2f(p.x, p.y + s.y),
	window_renderer_vec2f(p.x + s.x, p.y + s.y),
	c, c, c, c,
	uvp,
	window_renderer_vec2f(uvp.x + uvs.x, uvp.y),
	window_renderer_vec2f(uvp.x, uvp.y + uvs.y),
	window_renderer_vec2f(uvp.x + uvs.x, uvp.y + uvs.y));
}


WINDOW_DEF void window_renderer_solid_circle(Window_Renderer *r, Window_Renderer_Vec2f pos, float radius, int parts, Window_Renderer_Vec4f color) {
  
#define PI 3.141592653589793f

    Window_Renderer_Vec2f old = {radius, 0};
    for(int j=1;j<=parts;j++) {
	Window_Renderer_Vec2f new =
	    {radius * cosf(2 * PI * ((float) j ) / ((float) parts)),
	     radius * sinf(2 * PI * ((float) j ) / ((float) parts))};
	window_renderer_solid_triangle(r,
				       pos,
				       window_renderer_vec2f(pos.x + new.x, pos.y + new.y),
				       window_renderer_vec2f(pos.x + old.x, pos.y + old.y),
				       color );
	old = new;
    }
  
}

WINDOW_DEF bool window_renderer_push_texture(Window_Renderer *r, int width, int height, const void *data, bool grey, unsigned int *index) {

    GLenum current_texture;
    switch(r->images_count) {
    case 0:
	current_texture = GL_TEXTURE0;
	break;
    case 1:
	current_texture = GL_TEXTURE1;
	break;
    case 2:
	current_texture = GL_TEXTURE2;
	break;
    case 3:
	current_texture = GL_TEXTURE3;
	break;
    default:
	return false;
    }
  
    glActiveTexture(current_texture);
  
    glGenTextures(1, &r->textures);
    glBindTexture(GL_TEXTURE_2D, r->textures);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    if(grey) {
	glTexImage2D(GL_TEXTURE_2D,
		     0,
		     GL_ALPHA,
		     width,
		     height,
		     0,
		     GL_ALPHA,
		     GL_UNSIGNED_BYTE,
		     data);	
    } else {
	glTexImage2D(GL_TEXTURE_2D,
		     0,
		     GL_RGBA,
		     width,
		     height,
		     0,
		     GL_RGBA,
		     GL_UNSIGNED_INT_8_8_8_8_REV,
		     data);
    }

    *index = r->images_count++;

    return true;
}

WINDOW_DEF void window_renderer_end(Window_Renderer *r) {
    glBufferSubData(GL_ARRAY_BUFFER, 0, r->verticies_count * sizeof(Window_Renderer_Vertex), r->verticies);
    glDrawArrays(GL_TRIANGLES, 0, r->verticies_count);
    r->verticies_count = 0;
}

WINDOW_DEF void window_renderer_free(Window_Renderer *r) {
    (void) r;
}

#ifdef __STB_INCLUDE_STB_TRUETYPE_H__
#include <stdio.h>

#define WINDOW_RENDERER_STB_TEMP_BITMAP_SIZE 1024

WINDOW_DEF bool window_renderer_push_font(Window_Renderer *r, const char *filepath, float pixel_height) {
    FILE *f = fopen(filepath, "rb");
    if(!f) {
	WINDOW_LOG("ERROR: Can not open file: %s\n", filepath);
	return false;
    }

    if(fseek(f, 0, SEEK_END) < 0) {
	WINDOW_LOG("ERROR: Can not seek in file: %s\n", filepath);
	fclose(f);
	return false;
    }

    long m = ftell(f);
    if(m < 0) {
	WINDOW_LOG("ERROR: Can not read file(ftell): %s\n", filepath);
	fclose(f);
	return false;
    }  

    if(fseek(f, 0, SEEK_SET) < 0) {
	WINDOW_LOG("ERROR: Can not seek in file: %s\n", filepath);
	fclose(f);
	return false;
    }

    unsigned char *buffer = (unsigned char *) malloc((size_t) m);
    if(!buffer) {
	WINDOW_LOG("ERROR: Can not allocate enough memory\n");
	fclose(f);
	return false;
    }

    size_t _m = (size_t) m;
    size_t n = fread(buffer, 1, _m, f);
    if(n != _m) {
	WINDOW_LOG("ERROR: Failed to read file(fread): %s\n", filepath);
	free(buffer);
	fclose(f);
	return false;
    }
    
    fclose(f);

    unsigned char *temp_bitmap = malloc(WINDOW_RENDERER_STB_TEMP_BITMAP_SIZE *
	WINDOW_RENDERER_STB_TEMP_BITMAP_SIZE);
    stbtt_BakeFontBitmap(buffer,0, pixel_height,
			 temp_bitmap,
			 WINDOW_RENDERER_STB_TEMP_BITMAP_SIZE,
			 WINDOW_RENDERER_STB_TEMP_BITMAP_SIZE,
			 32, 96, r->font_cdata);

    unsigned int tex;
    bool result = push_texture(1024, 1024, temp_bitmap, true, &tex);
    r->font_index = (int) tex;

    free(buffer);
    free(temp_bitmap);
    
    return result;
}

WINDOW_DEF void window_renderer_text(Window_Renderer *r, const char *cstr, size_t cstr_len, Window_Renderer_Vec2f pos, float factor, Window_Renderer_Vec4f color) {

    float x = pos.x;
    float y = pos.y;
    color.w *= -1;
    
    for(size_t i=0;i<cstr_len;i++) {
	unsigned char c = cstr[i];
	if (c < 32 && c >= 128) {
	    continue;
	}

	float _y = y;

	stbtt_aligned_quad q;
	stbtt_GetBakedQuad(r->font_cdata,
			   WINDOW_RENDERER_STB_TEMP_BITMAP_SIZE,
			   WINDOW_RENDERER_STB_TEMP_BITMAP_SIZE, c-32, &x, &y, &q,1);
	//1=opengl & d3d10+,0=d3d9

	Window_Renderer_Vec2f p = vec2f(q.x0 * factor, _y + factor * (y - q.y1) );
	Window_Renderer_Vec2f s = vec2f((q.x1 - q.x0) * factor, (q.y1 - q.y0) * factor);
	Window_Renderer_Vec2f uvp = vec2f(q.s0, 1 - q.t1);
	Window_Renderer_Vec2f uvs = vec2f(q.s1 - q.s0, q.t1 - q.t0);

	
	window_renderer_quad(
	    r,
	    p,
	    window_renderer_vec2f(p.x + s.x, p.y),
	    window_renderer_vec2f(p.x, p.y + s.y),
	    window_renderer_vec2f(p.x + s.x, p.y + s.y),
	    color, color, color, color,
	    uvp,
	    window_renderer_vec2f(uvp.x + uvs.x, uvp.y),
	    window_renderer_vec2f(uvp.x, uvp.y + uvs.y),
	    window_renderer_vec2f(uvp.x + uvs.x, uvp.y + uvs.y));       
    }

}

#endif //__STB_INCLUDE_STB_TRUETYPE_H__

#endif //WINDOW_NO_RENDERER

////////////////////////////////////////////////////////////////////////
// opengl - definitions
////////////////////////////////////////////////////////////////////////

PROC _glActiveTexture = NULL;
void glActiveTexture(GLenum texture) { _glActiveTexture(texture); }

PROC _glGenVertexArrays = NULL;
void glGenVertexArrays(GLsizei n, GLuint *arrays) { _glGenVertexArrays(n, arrays); }

PROC _glBindVertexArray = NULL;
void glBindVertexArray(GLuint array) { _glBindVertexArray(array); }

PROC _glGenBuffers = NULL;
void glGenBuffers(GLsizei n, GLuint *buffers) { _glGenBuffers(n, buffers); }

PROC _glBindBuffer = NULL;
void glBindBuffer(GLenum target, GLuint buffer) { _glBindBuffer(target, buffer); }

PROC _glBufferData = NULL;
void glBufferData(GLenum target, GLsizeiptr size, const void * data, GLenum usage) { _glBufferData(target, size, data, usage); }

PROC _glEnableVertexAttribArray = NULL;
void glEnableVertexAttribArray(GLuint index) { _glEnableVertexAttribArray(index); }

PROC _glVertexAttribPointer = NULL;
void glVertexAttribPointer(GLuint index,
			   GLint size,
			   GLenum type,
			   GLboolean normalized,
			   GLsizei stride,
			   const void * pointer) {
    _glVertexAttribPointer(index, size, type, normalized, stride, pointer);
}

PROC _glCreateShader = NULL;
GLuint glCreateShader(GLenum shaderType) { return (GLuint) _glCreateShader(shaderType); }

PROC _glShaderSource = NULL;
void glShaderSource(GLuint shader,
		    GLsizei count,
		    const GLchar **_string,
		    const GLint *length) {
    _glShaderSource(shader, count, _string, length);
}

PROC _glCompileShader = NULL;
void glCompileShader(GLuint shader) { _glCompileShader(shader); }

PROC _glGetShaderiv = NULL;
void glGetShaderiv(GLuint shader, GLenum pname, GLint *params) {
    _glGetShaderiv(shader, pname, params);
}

PROC _glGetShaderInfoLog = NULL;
void glGetShaderInfoLog(GLuint shader,
			GLsizei maxLength,
			GLsizei *length,
			GLchar *infoLog) {
    _glGetShaderInfoLog(shader, maxLength, length, infoLog);
}

PROC _glCreateProgram = NULL;
GLuint glCreateProgram(void) { return (GLuint) _glCreateProgram(); }

PROC _glAttachShader = NULL;
void glAttachShader(GLuint program, GLuint shader) { _glAttachShader(program, shader); }

PROC _glLinkProgram = NULL;
void glLinkProgram(GLuint program) { _glLinkProgram(program); }

PROC _glGetProgramInfoLog = NULL;
void glGetProgramInfoLog(GLuint program,
			 GLsizei maxLength,
			 GLsizei *length,
			 GLchar *infoLog) {
    _glGetProgramInfoLog(program, maxLength, length, infoLog);
}

PROC _glGetProgramiv = NULL;
void glGetProgramiv(GLuint program,
		    GLenum pname,
		    GLint *params) {
    _glGetProgramiv(program, pname, params);
}

PROC _glUseProgram = NULL;
void glUseProgram(GLuint program) { _glUseProgram(program); }

PROC _glBufferSubData = NULL;
void glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const void * data) {
    _glBufferSubData(target, offset, size, data);
}

PROC _glUniform2f = NULL;
void glUniform2f(GLint location, GLfloat v0, GLfloat v1) {
    _glUniform2f(location, v0, v1);
}

PROC _glUniform1f = NULL;
void glUniform1f(GLint location, GLfloat v0) {
    _glUniform1f(location, v0);
}

PROC _glUniform1fv = NULL;
void glUniform1fv(GLint location, GLsizei count, const GLfloat *value) {
    _glUniform1fv(location, count, value);
}

PROC _glUniform2fv = NULL;
void glUniform2fv(GLint location, GLsizei count, const GLfloat *value) {
    _glUniform2fv(location, count, value);
}

PROC _glUniform1i = NULL;
void glUniform1i(GLint location, GLint v0) {
    _glUniform1i(location, v0);
}

PROC _glGetUniformLocation = NULL;
GLint glGetUniformLocation(GLuint program, const GLchar *name) { return (GLint) _glGetUniformLocation(program, name); }

PROC _glGetActiveUniform = NULL;
void glGetActiveUniform(GLuint program,
			GLuint index,
			GLsizei bufSize,
			GLsizei *length,
			GLint *size,
			GLenum *type,
			GLchar *name) {
    _glGetActiveUniform(program, index, bufSize, length, size, type, name);
}

PROC _glGetUniformfv = NULL;
void glGetUniformfv(GLuint program, GLint location, GLfloat *params) {
    _glGetUniformfv(program, location, params);
}

PROC _glGetUniformiv = NULL;
void glGetUniformiv(GLuint program, GLint location, GLsizei bufSize, GLint *params) {
    _glGetUniformiv(program, location, bufSize, params);
}

PROC _wglSwapIntervalEXT = NULL;
int wglSwapIntervalEXT(GLint interval) {
    return (int) _wglSwapIntervalEXT(interval);
}

PROC _glSampleCoverage = NULL;
void glSampleCoverage(GLfloat value, GLboolean invert) {
    _glSampleCoverage(value, invert);
}


WINDOW_DEF void window_win32_opengl_init() {
    if(_glActiveTexture != NULL) {
	return;
    }
  
    _glActiveTexture = wglGetProcAddress("glActiveTexture");
    _glGenVertexArrays = wglGetProcAddress("glGenVertexArrays");
    _glBindVertexArray= wglGetProcAddress("glBindVertexArray");
    _glGenBuffers= wglGetProcAddress("glGenBuffers");
    _glBindBuffer= wglGetProcAddress("glBindBuffer");
    _glBufferData= wglGetProcAddress("glBufferData");
    _glEnableVertexAttribArray= wglGetProcAddress("glEnableVertexAttribArray");
    _glVertexAttribPointer= wglGetProcAddress("glVertexAttribPointer");
    _glCreateShader= wglGetProcAddress("glCreateShader");
    _glShaderSource= wglGetProcAddress("glShaderSource");
    _glCompileShader= wglGetProcAddress("glCompileShader");
    _glGetShaderiv= wglGetProcAddress("glGetShaderiv");
    _glGetShaderInfoLog= wglGetProcAddress("glGetShaderInfoLog");
    _glCreateProgram= wglGetProcAddress("glCreateProgram");
    _glAttachShader= wglGetProcAddress("glAttachShader");
    _glLinkProgram= wglGetProcAddress("glLinkProgram");
    _glGetProgramInfoLog= wglGetProcAddress("glGetProgramInfoLog");
    _glGetProgramiv= wglGetProcAddress("glGetProgramiv");
    _glUseProgram= wglGetProcAddress("glUseProgram");
    _glBufferSubData= wglGetProcAddress("glBufferSubData");
    _glUniform2f= wglGetProcAddress("glUniform2f");
    _glUniform1f= wglGetProcAddress("glUniform1f");
    _glUniform1i= wglGetProcAddress("glUniform1i");
    _glGetUniformLocation= wglGetProcAddress("glGetUniformLocation");
    _glGetActiveUniform= wglGetProcAddress("glGetActiveUniform");
    _glGetUniformfv= wglGetProcAddress("glGetUniformfv");
    _glUniform1fv= wglGetProcAddress("glUniform1fv");
    _glUniform2fv= wglGetProcAddress("glUniform2fv");
    _glGetUniformiv= wglGetProcAddress("glGetUniformiv");
    _wglSwapIntervalEXT = wglGetProcAddress("wglSwapIntervalEXT");
}

#endif //WINDOW_IMPLEMENTATION

#endif //WINDOW_H
