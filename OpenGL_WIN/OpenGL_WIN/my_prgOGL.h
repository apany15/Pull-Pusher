#include <windows.h>              // «аголовочные файлы дл€ Windows


#include <gl\glew.h>                // «аголовочные файлы дл€ библиотеки glew
#include"GL/wglew.h"


#define MYPRGOGLMAINWINDOWNAME TEXT("“олкало-т€нутель V 0.0.0.1")
#define MYPRGOGLMAINWINDOWWIDTH 1024		//ширина окна
#define MYPRGOGLMAINWINDOWHEIGHT 768	//высота окна

#define MYPRGOGLMAINWINDOWSTARTFULLSRC 1	//старт на полный экран
#define MYPRGOGLMAINWINDOWSTARTNFULLSRC 0	//старт в окне
#define MYPRGOGLMAINWINDOWSTARTFULLSRCASC 2	//спросить как стартовать у юзера
//#define MYPRGOGLMAINWINDOWSTART MYPRGOGLMAINWINDOWSTARTNFULLSRC	//тут нада выбрать сейчас способ старта
//#define MYPRGOGLMAINWINDOWSTART MYPRGOGLMAINWINDOWSTARTFULLSRCASC	//тут нада выбрать сейчас способ старта
#define MYPRGOGLMAINWINDOWSTART MYPRGOGLMAINWINDOWSTARTNFULLSRC	//тут нада выбрать сейчас способ старта

void my_initOGL(void);	//инициализаци€ OpenGL
void my_drawResizeWindowOGL(GLsizei width, GLsizei height);	//изменение размеров окна
void my_keyboardTest(const bool *keyStatus);	//вызываетс€ перед my_drawResizeWindowOGL и содержит состо€ние клавиш
void my_mouseTest(int x, int y, bool lButton, bool rButton, bool mButton); //вызываетс€ при перемещении мышки в окне
void my_drawOGL(void);	//рисователь (вызываетс€ непрерывно)
void my_beforeExit(void);	//вызываетс€ перед закрытием приложени€



#define WGL_DRAW_TO_WINDOW_ARB 0x2001
#define WGL_SUPPORT_OPENGL_ARB 0x2010
#define WGL_ACCELERATION_ARB	0x2003
#define WGL_FULL_ACCELERATION_ARB	0x2027
#define WGL_COLOR_BITS_ARB	0x2014
#define WGL_ALPHA_BITS_ARB	0x201B
#define WGL_DEPTH_BITS_ARB	0x2022
#define WGL_STENCIL_BITS_ARB	0x2023
#define WGL_DOUBLE_BUFFER_ARB	0x2011
#define WGL_SAMPLE_BUFFERS_ARB	0x2041
#define WGL_SAMPLES_ARB	0x2042

#define TEXTURES_ANISOTROPY_LEVEL 8
#define MULTISAMPLE_LEVEL 4