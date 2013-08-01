//#include <windows.h>              // ������������ ����� ��� Windows
//#include <gl\gl.h>                // ������������ ����� ��� ���������� OpenGL32
//#include <gl\glu.h>              // ������������ ����� ��� ���������� GLu32
//#include <gl\glaux.h>              // ������������ ����� ��� ���������� GLaux

#include"my_prgOGL.h"

#define DEF_WINDOWCLASSNAME TEXT("�������-��������")


HGLRC  hRC=NULL;              // ���������� �������� ����������
HDC  hDC=NULL;              // ��������� �������� ���������� GDI
HWND  hWnd=NULL;              // ����� ����� �������� ���������� ����
HINSTANCE  hInstance;              // ����� ����� �������� ���������� ����������

bool  keys[256];                // ������, ������������ ��� �������� � �����������
bool  active=true;                // ���� ���������� ����, ������������� � true �� ���������
bool  fullscreen=true;              // ���� ������ ����, ������������� � ������������� �� ���������

LRESULT  CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);        // �������� ������� WndProc

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)        // �������� ������ � ���������������� ���� GL
{
	my_drawResizeWindowOGL(width,height);
	return;
	//if (height==0)              // �������������� ������� �� ����
	//{
	//	height=1;
	//}
	//glViewport(0,0,width,height);          // ����� ������� ������� ������ 
	//glMatrixMode(GL_PROJECTION);            // ����� ������� ��������
	//glLoadIdentity();              // ����� ������� ��������
	//// ���������� ����������� �������������� �������� ��� ����
	//gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
	//glMatrixMode(GL_MODELVIEW);            // ����� ������� ���� ������
	//glLoadIdentity();              // ����� ������� ���� ������
}
int InitGL(GLvoid)                // ��� ��������� ������� OpenGL ���������� �����
{
	my_initOGL();
	return true;                // ������������� ������ �������

	//glShadeModel(GL_SMOOTH);            // ��������� ������� �������� �����������
	//glClearColor(0.0f,0.0f,0.0f,0.0f);          // ������� ������ � ������ ����
	//glClearDepth(1.0f);              // ��������� ������� ������ �������
	//glEnable(GL_DEPTH_TEST);            // ��������� ���� �������
	//glDepthFunc(GL_LEQUAL);            // ��� ����� �������
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);      // ��������� � ���������� �����������
	//return true;                // ������������� ������ �������
}
int DrawGLScene(GLvoid)                // ����� ����� ����������� ��� ����������
{
	my_drawOGL();
	return true;                // ���������� ������ �������

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);      // �������� ����� � ����� �������
	//glLoadIdentity();              // �������� ������� �������
	//return true;                // ���������� ������ �������
}
GLvoid KillGLWindow(GLvoid)              // ���������� ���������� ����
{
	if(fullscreen)              // �� � ������������� ������?
	{
		ChangeDisplaySettings(NULL,0);        // ���� ��, �� ������������� ������� � ������� �����
		ShowCursor(true);            // �������� ������ �����
	}
	if(hRC)                // ���������� �� �������� ����������?
	{
		if(!wglMakeCurrent(NULL,NULL))        // �������� �� ���������� RC � DC?
		{
			MessageBox(NULL,TEXT("Release Of DC And RC Failed."),TEXT("SHUTDOWN ERROR"),MB_OK | MB_ICONINFORMATION);
		}
		if(!wglDeleteContext(hRC))        // �������� �� ������� RC?
		{
          MessageBox(NULL,TEXT("Release Rendering Context Failed."),TEXT("SHUTDOWN ERROR"),MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;              // ���������� RC � NULL
	}
	if(hDC && !ReleaseDC(hWnd,hDC))          // �������� �� ���������� DC?
	{
		MessageBox(NULL,TEXT("Release Device Context Failed."),TEXT("SHUTDOWN ERROR"),MB_OK | MB_ICONINFORMATION);
		hDC=NULL;                // ���������� DC � NULL
	}
	if(hWnd && !DestroyWindow(hWnd))            // �������� �� ���������� ����?
	{
		MessageBox(NULL,TEXT("Could Not Release hWnd."),TEXT("SHUTDOWN ERROR"),MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;                // ���������� hWnd � NULL
	}
	if(!UnregisterClass(DEF_WINDOWCLASSNAME,hInstance))        // �������� �� ����������������� �����
	{
		MessageBox(NULL,TEXT("Could Not Unregister Class."),TEXT("SHUTDOWN ERROR"),MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;                // ���������� hInstance � NULL
	}
}

//����������� �������, ������� �������� ��������� ��������� ������� ���������������
int selectMaxMultisampleLevel(HDC hDC,GLint *iAttributes,int *multi_level)
{
	iAttributes[19] = *multi_level;
	float fAttributes[] = {0,0};
	int pixelFormat_;
	UINT numFormats;

	bool valid=wglChoosePixelFormatARB(hDC,iAttributes,fAttributes,1,&pixelFormat_,&numFormats);
	if (valid && numFormats>=1) 
		return pixelFormat_;
	if (*multi_level==1) 
		return -1;
	else
	{
		(*multi_level)/=2;
		return selectMaxMultisampleLevel(hDC,iAttributes,multi_level);
	}
}

BOOL CreateGLWindow(LPCSTR title,int width,int height,int bits,bool fullscreenflag)
{
	static bool firstStartFunc=true;
	
	static GLuint    PixelFormat=0;              // ������ ��������� ����� ������

	WNDCLASS  wc;                // ��������� ������ ����
	DWORD    dwExStyle;              // ����������� ����� ����
	DWORD    dwStyle;              // ������� ����� ����
	RECT WindowRect;                // Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;              // ���������� ����� ������������ � 0
	WindowRect.right=(long)width;              // ���������� ������ ������������ � Width
	WindowRect.top=(long)0;                // ���������� ������� ������������ � 0
	WindowRect.bottom=(long)height;              // ���������� ������ ������������ � Height
	fullscreen=fullscreenflag;              // ������������� �������� ���������� ���������� fullscreen

	hInstance=GetModuleHandle(NULL);        // ������� ���������� ������ ����������
	wc.style=CS_HREDRAW | CS_VREDRAW | CS_OWNDC;      // ���������� ��� ����������� � ������ ������� DC
	wc.lpfnWndProc=(WNDPROC)WndProc;          // ��������� ��������� ���������
	wc.cbClsExtra=0;              // ��� �������������� ���������� ��� ����
	wc.cbWndExtra=0;              // ��� �������������� ���������� ��� ����
	wc.hInstance=hInstance;            // ������������� ����������
	wc.hIcon=LoadIcon(NULL,IDI_WINLOGO);        // ��������� ������ �� ���������
	wc.hCursor=LoadCursor(NULL,IDC_ARROW);        // ��������� ��������� �����
	wc.hbrBackground=NULL;              // ��� �� ��������� ��� GL
	wc.lpszMenuName=NULL;              // ���� � ���� �� �����
	wc.lpszClassName=DEF_WINDOWCLASSNAME;            // ������������� ��� ������

	if(!RegisterClass(&wc))              // �������� ���������������� ����� ����
	{
		MessageBox(NULL,TEXT("Failed To Register The Window Class."),TEXT("ERROR"),MB_OK | MB_ICONEXCLAMATION);
		return false;                // ����� � ����������� �������� �������� false
	}

	if (fullscreen)                // ������������� �����?
	{
		DEVMODE dmScreenSettings;            // ����� ����������
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));    // ������� ��� �������� ���������
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);      // ������ ��������� Devmode
		dmScreenSettings.dmPelsWidth=width;        // ������ ������
		dmScreenSettings.dmPelsHeight=height;        // ������ ������
		dmScreenSettings.dmBitsPerPel=bits;        // ������� �����
		dmScreenSettings.dmFields=DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;// ����� �������
		// �������� ���������� ��������� ����� � �������� ���������.  ����������: CDS_FULLSCREEN ������� ������ ����������.
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			// ���� ������������ � ������������� ����� ����������, ����� ���������� ��� ��������: ������� ����� ��� �����.
			if (MessageBox(NULL,TEXT("The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?"),TEXT("NeHe GL"),MB_YESNO | MB_ICONEXCLAMATION)==IDYES )
			{
				fullscreen = false;          // ����� �������� ������ (fullscreen = false)
			}
			else
			{
				// ������������� ����, ���������� ������������ � �������� ����.
				MessageBox(NULL,TEXT("Program Will Now Close."),TEXT("ERROR"), MB_OK | MB_ICONSTOP );
				return false;            // ����� � ����������� �������� false
			}
		}
	}
	if(fullscreen)                  // �� �������� � ������������� ������?
	{
		dwExStyle=WS_EX_APPWINDOW;          // ����������� ����� ����
		dwStyle=WS_POPUP;            // ������� ����� ����
		ShowCursor(false);              // ������ ��������� �����
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;      // ����������� ����� ����
		dwStyle=WS_OVERLAPPEDWINDOW;        // ������� ����� ����
	}
	AdjustWindowRectEx(&WindowRect,dwStyle,false,dwExStyle);      // ��������� ���� ���������� �������
	if(!(hWnd=CreateWindowEx(dwExStyle,          // ����������� ����� ��� ����
		DEF_WINDOWCLASSNAME,          // ��� ������
		title,            // ��������� ����
		WS_CLIPSIBLINGS |        // ��������� ����� ��� ����
		WS_CLIPCHILDREN |        // ��������� ����� ��� ����
		dwStyle,          // ���������� ����� ��� ����
		0,0,            // ������� ����
		WindowRect.right-WindowRect.left,    // ���������� ���������� ������
		WindowRect.bottom-WindowRect.top,    // ���������� ���������� ������
		NULL,            // ��� �������������
		NULL,            // ��� ����
		hInstance,          // ���������� ����������
		NULL)))          // �� ������� ������ �� WM_CREATE (???)
	{
		KillGLWindow();                // ������������ �����
		MessageBox(NULL,TEXT("Window Creation Error."),TEXT("ERROR"),MB_OK | MB_ICONEXCLAMATION);
		return false;                // ������� false
	}
	if(!(hDC=GetDC(hWnd)))              // ����� �� �� �������� �������� ����������?
	{
		KillGLWindow();                // ������������ �����
		MessageBox(NULL,TEXT("Can't Create A GL Device Context."),TEXT("ERROR"),MB_OK | MB_ICONEXCLAMATION);
		return false;                // ������� false
	}

	static  PIXELFORMATDESCRIPTOR pfd=            // pfd �������� Windows ����� ����� ����� �� ����� ������� �������
		{
			sizeof(PIXELFORMATDESCRIPTOR),            // ������ ����������� ������� ������� ��������
			1,                  // ����� ������
			PFD_DRAW_TO_WINDOW |              // ������ ��� ����
			PFD_SUPPORT_OPENGL |              // ������ ��� OpenGL	(****************************************************************************************************************************)
			PFD_DOUBLEBUFFER,              // ������ ��� �������� ������
			PFD_TYPE_RGBA,                // ��������� RGBA ������
			bits,                  // ���������� ��� ������� �����
			0, 0, 0, 0, 0, 0,              // ������������� �������� �����
			0,                  // ��� ������ ������������
			0,                  // ��������� ��� ������������
			0,                  // ��� ������ ����������
			0, 0, 0, 0,                // ���� ���������� ������������
			32,                  // 32 ������ Z-����� (����� �������)
			1,                  // ���!!!����!!! ������ ���������				(***************************************************************************************************************************)
			0,                  // ��� ��������������� �������
			PFD_MAIN_PLANE,                // ������� ���� ���������
			0,                  // ���������������
			0, 0, 0                  // ����� ���� ������������
		};


	if (firstStartFunc)
	{
		if(!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))        // ������ �� ���������� ������ �������?
		{
			KillGLWindow();                // ������������ �����
			MessageBox(NULL,TEXT("Can't Find A Suitable PixelFormat."),TEXT("ERROR"),MB_OK | MB_ICONEXCLAMATION);
			return false;                // ������� false
		}
	}
	
	
	if(!SetPixelFormat(hDC,PixelFormat,&pfd))          // �������� �� ���������� ������ �������?
	{
		KillGLWindow();                // ������������ �����
		MessageBox(NULL,TEXT("Can't Set The PixelFormat."),TEXT("ERROR"),MB_OK | MB_ICONEXCLAMATION);
		return false;                // ������� false
	}

	//DescribePixelFormat(hDC,PixelFormat,sizeof(tek_pf),&tek_pf);

	

	if(!(hRC=wglCreateContext(hDC)))          // �������� �� ���������� �������� ����������?
	{
		KillGLWindow();                // ������������ �����
		MessageBox(NULL,TEXT("Can't Create A GL Rendering Context."),TEXT("ERROR"),MB_OK | MB_ICONEXCLAMATION);
		return false;                // ������� false
	}
	
	if(!wglMakeCurrent(hDC,hRC))            // ����������� ������������ �������� ����������
	{
		KillGLWindow();                // ������������ �����
		MessageBox(NULL,TEXT("Can't Activate The GL Rendering Context."),TEXT("ERROR"),MB_OK | MB_ICONEXCLAMATION);
		return false;                // ������� false
	}


	if (firstStartFunc)
	{
		//��������� ���������������
		//�������� ��������� ����������
		if (glewInit()==GLEW_OK)
		{
			if (glewIsSupported("GL_ARB_multisample"))
			{
				// ��� �������� � ����, ������� �� ����� �������������� � ����� ����
				// ��� �������� ����������, ������ ���� �� ��� �� �����
				// ������������� ��������������� - ��� SAMPLE BUFFERS ARB � WGL SAMPLES
				// ��� �������� ������� �������� �� �������: ���� ��� ���
				// � ��� ��������� ������������� �������
				int iAttributes[] = {
					WGL_DRAW_TO_WINDOW_ARB,GL_TRUE, // �������, ���� ������ ������� ����� ���� ����������� � ����
					WGL_SUPPORT_OPENGL_ARB,GL_TRUE, // �������, ���� �������������� OpenGL
					WGL_ACCELERATION_ARB,WGL_FULL_ACCELERATION_ARB, // ������ ���������� ���������
					WGL_COLOR_BITS_ARB,24,          // ���������
					WGL_ALPHA_BITS_ARB,8,           // ����������� �����-������
					WGL_DEPTH_BITS_ARB,16,          // ������� ������ �������
					WGL_STENCIL_BITS_ARB,0,         // ������� ������ �������
					WGL_DOUBLE_BUFFER_ARB,GL_TRUE,  // ������, ���� ������������ ������� �����������
					WGL_SAMPLE_BUFFERS_ARB,GL_TRUE, // ��� �� � �����
					WGL_SAMPLES_ARB, MULTISAMPLE_LEVEL ,            // �������� �� 4x ���
					0,0};
				int pixelFormat_;
				int level=MULTISAMPLE_LEVEL;
				pixelFormat_=selectMaxMultisampleLevel(hDC,iAttributes,&level);
				if (pixelFormat_!=-1)
				{
					PixelFormat=pixelFormat_;
					KillGLWindow();
					firstStartFunc=false;
					return CreateGLWindow(title,width,height,bits,fullscreenflag);
				}
			}
		}
	}
	else
		firstStartFunc=true;






	ShowWindow(hWnd,SW_SHOW);              // �������� ����
	SetForegroundWindow(hWnd);              // ������ ������� ���������
	SetFocus(hWnd);                // ���������� ����� ���������� �� ���� ����
	ReSizeGLScene(width,height);              // �������� ����������� ��� ������ OpenGL ������.
	if(!InitGL())                  // ������������� ������ ��� ���������� ����
	{
		KillGLWindow();                // ������������ �����
		MessageBox(NULL,TEXT("Initialization Failed."),TEXT("ERROR"),MB_OK | MB_ICONEXCLAMATION);
		return false;                // ������� false
	}



	



	return true;                  // �� � �������!
}

LRESULT CALLBACK WndProc(  HWND  hWnd,            // ���������� ������� ����
	UINT  uMsg,            // ��������� ��� ����� ����
	WPARAM  wParam,            // �������������� ����������
	LPARAM  lParam)            // �������������� ����������
{
	switch (uMsg)                // �������� ��������� ��� ����
	{
		case WM_ACTIVATE:            // �������� ��������� ���������� ����
		{
			if(!HIWORD(wParam))          // ��������� ��������� �����������
			{
				active=true;          // ��������� �������
			}
			else
			{
				active=false;          // ��������� ������ �� �������
			}
			return 0;            // ������������ � ���� ��������� ���������
		}
		case WM_SYSCOMMAND:            // ������������� ��������� �������
		{
			switch (wParam)            // ������������� ��������� �����
			{
				case SC_SCREENSAVE:        // �������� �� ���������� �����������?
				case SC_MONITORPOWER:        // �������� �� ������� ������� � ����� ���������� �������?
				return 0;          // ������������� ���
			}
			break;              // �����

		}
		case WM_CLOSE:              // �� �������� ��������� � ��������?
		{
			PostQuitMessage(0);          // ��������� ��������� � ������
			return 0;            // ��������� �����
		}
		case WM_KEYDOWN:            // ���� �� ������ ������?
		{
			keys[wParam]=true;          // ���� ���, �� ����������� ���� ������ true
			return 0;            // ������������
		}
		case WM_KEYUP:              // ���� �� �������� �������?
		{
			keys[wParam]=false;          //  ���� ���, �� ����������� ���� ������ false
			return 0;            // ������������
		}
		case WM_SIZE:              // �������� ������� OpenGL ����
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  // ������� �����=Width, ������� �����=Height
			return 0;            // ������������
		}
		case WM_MOUSEMOVE:
		{	
			RECT rt;
			POINT pt={0,0};
			GetClientRect(hWnd,&rt);
			ClientToScreen(hWnd,&pt);
			OffsetRect(&rt,pt.x,pt.y);
			GetCursorPos(&pt);
			if (PtInRect(&rt,pt))
				my_mouseTest((int)pt.x,(int)pt.y,(GetKeyState(VK_LBUTTON) & 0x8000)>0,(GetKeyState(VK_RBUTTON) & 0x8000)>0,(GetKeyState(VK_MBUTTON) & 0x8000)>0);
			return 0;
		}
	}
	// ���������� ��� �������������� ��������� DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}
BOOL  gHalt=false;            // ���������� ���������� ��� ������ �� �����

int WINAPI WinMain(HINSTANCE  hInstance,        // ���������� ����������
	HINSTANCE  hPrevInstance,        // ���������� ������������� ����������
	LPSTR    lpCmdLine,        // ��������� ��������� ������
	int    nCmdShow)        // ��������� ����������� ����
{
	MSG  msg;              // ��������� ��� �������� ��������� Windows
	//BOOL  done=false;            // ���������� ���������� ��� ������ �� �����

	if (MYPRGOGLMAINWINDOWSTART==MYPRGOGLMAINWINDOWSTARTFULLSRCASC)
	{
		// ���������� ������������, ����� ����� ������ �� ������������
		if (MessageBox(NULL,TEXT("������ �� �� ��������� ���������� � ������������� ������?"),TEXT("��������� � ������������� ������?"),MB_YESNO | MB_ICONQUESTION)==IDNO)
		{
			fullscreen = false;          // ������� �����
		}
	}
	else
	{
		fullscreen=MYPRGOGLMAINWINDOWSTART;
	}



	// ������� ���� OpenGL ����
	if(!CreateGLWindow(MYPRGOGLMAINWINDOWNAME,MYPRGOGLMAINWINDOWWIDTH,MYPRGOGLMAINWINDOWHEIGHT,32,fullscreen))
	{
		return 0;              // �����, ���� ���� �� ����� ���� �������
	}

	while(!gHalt)                // ���� ������������, ���� done �� ����� true
	{
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))    // ���� �� � ������� �����-������ ���������?
		{
			if( msg.message==WM_QUIT )        // �� ������� ��������� � ������?
			{
				gHalt=true;          // ���� ���, done=true
			}
			else              // ���� ���, ������������ ���������
			{
				TranslateMessage(&msg);        // ��������� ���������
				DispatchMessage(&msg);        // �������� ���������
			}
		}
		else                // ���� ��� ���������
		{
			// ������������� �����.
			if(active)          // ������� �� ���������?
			{
				if(keys[VK_ESCAPE])        // ���� �� ������ ������� ESC?
				{
					gHalt=true;      // ESC ������� �� �������� ���������� ���������
				}
				else            // �� ����� ��� ������, ������� �����.
				{
					my_keyboardTest(keys);
					DrawGLScene();        // ������ �����
					SwapBuffers(hDC);    // ������ ����� (������� �����������)
					
				}
			}
			if(keys[VK_F1])          // ���� �� ������ F1?
			{
				keys[VK_F1]=false;        // ���� ���, ������ �������� ������ ������� �� false
				KillGLWindow();          // ��������� ������� ����
				fullscreen=!fullscreen;      // ����������� �����
				// ���������� ���� OpenGL ����
				if(!CreateGLWindow(MYPRGOGLMAINWINDOWNAME,MYPRGOGLMAINWINDOWWIDTH,MYPRGOGLMAINWINDOWHEIGHT,32,fullscreen))
				{
					gHalt=true;       // �������, ���� ��� ����������
				}
			}
		}
	}
	// Shutdown
	my_beforeExit();
	KillGLWindow();                // ��������� ����
	return((int)msg.wParam);              // ������� �� ���������
}