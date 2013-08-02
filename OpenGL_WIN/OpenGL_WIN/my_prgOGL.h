#include <windows.h>              // ������������ ����� ��� Windows


#include <gl\glew.h>                // ������������ ����� ��� ���������� glew
#include"GL/wglew.h"


#define MYPRGOGLMAINWINDOWNAME TEXT("�������-�������� V 0.0.0.1")
#define MYPRGOGLMAINWINDOWWIDTH 1024		//������ ����
#define MYPRGOGLMAINWINDOWHEIGHT 768	//������ ����

#define MYPRGOGLMAINWINDOWSTARTFULLSRC 1	//����� �� ������ �����
#define MYPRGOGLMAINWINDOWSTARTNFULLSRC 0	//����� � ����
#define MYPRGOGLMAINWINDOWSTARTFULLSRCASC 2	//�������� ��� ���������� � �����
//#define MYPRGOGLMAINWINDOWSTART MYPRGOGLMAINWINDOWSTARTNFULLSRC	//��� ���� ������� ������ ������ ������
//#define MYPRGOGLMAINWINDOWSTART MYPRGOGLMAINWINDOWSTARTFULLSRCASC	//��� ���� ������� ������ ������ ������
#define MYPRGOGLMAINWINDOWSTART MYPRGOGLMAINWINDOWSTARTNFULLSRC	//��� ���� ������� ������ ������ ������

void my_initOGL(void);	//������������� OpenGL
void my_drawResizeWindowOGL(GLsizei width, GLsizei height);	//��������� �������� ����
void my_keyboardTest(const bool *keyStatus);	//���������� ����� my_drawResizeWindowOGL � �������� ��������� ������
void my_mouseTest(int x, int y, bool lButton, bool rButton, bool mButton); //���������� ��� ����������� ����� � ����
void my_drawOGL(void);	//���������� (���������� ����������)
void my_beforeExit(void);	//���������� ����� ��������� ����������



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