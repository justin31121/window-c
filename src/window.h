#ifndef WINDOW_H
#define WINDOW_H

// win32
//   msvc : user32.lib
//   mingw:

#include <stdbool.h>

#include <windows.h>
#include <GL/GL.h>

#ifndef WINDOW_DEF
#  define WINDOW_DEF static inline
#endif //WINDOW_DEF

typedef enum{
  WINDOW_EVENT_TYPE_NONE = 0,
}Window_Event_Type;

typedef struct{
  MSG msg;
  Window_Event_Type type;
}Window_Event;

typedef struct{
  HWND hwnd;
  HDC dc;
  bool running;
}Window;

WINDOW_DEF bool window_init(Window *w, int width, int height, const char *title);
WINDOW_DEF bool window_peek(Window *w, Window_Event *event);
WINDOW_DEF void window_swap_buffers(Window *w);
WINDOW_DEF void window_free(Window *w);

#ifdef WINDOW_IMPLEMENTATION

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
  if(!RegisterClassEx(&wc)) {
    return false;
  }

  int screen_width = GetSystemMetrics(SM_CXSCREEN);
  int screen_height = GetSystemMetrics(SM_CYSCREEN);

  height += 39;

  w->hwnd = CreateWindowEx(0,
			   wc.lpszClassName,
			   wc.lpszClassName,
			   WS_OVERLAPPEDWINDOW,
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

  return true;
}

WINDOW_DEF bool window_peek(Window *w, Window_Event *e) {

  MSG *msg = &e->msg;

  while(true) {
    if(!PeekMessage(msg, w->hwnd, 0, 0, PM_REMOVE)) {
      return false;
    }

    TranslateMessage(msg);
    DispatchMessage(msg);

    e->type = WINDOW_EVENT_TYPE_NONE;

    switch(msg->message) {
    default: {
    } break;
    }

    if(e->type != WINDOW_EVENT_TYPE_NONE) {
      return true;
    }
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

#endif //WINDOW_IMPLEMENTATION

#endif //WINDOW_H
