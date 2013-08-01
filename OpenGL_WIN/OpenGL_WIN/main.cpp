//#include <windows.h>              // Заголовочные файлы для Windows
//#include <gl\gl.h>                // Заголовочные файлы для библиотеки OpenGL32
//#include <gl\glu.h>              // Заголовочные файлы для библиотеки GLu32
//#include <gl\glaux.h>              // Заголовочные файлы для библиотеки GLaux

#include"my_prgOGL.h"

#define DEF_WINDOWCLASSNAME TEXT("Толкало-тянутель")


HGLRC  hRC=NULL;              // Постоянный контекст рендеринга
HDC  hDC=NULL;              // Приватный контекст устройства GDI
HWND  hWnd=NULL;              // Здесь будет хранится дескриптор окна
HINSTANCE  hInstance;              // Здесь будет хранится дескриптор приложения

bool  keys[256];                // Массив, используемый для операций с клавиатурой
bool  active=true;                // Флаг активности окна, установленный в true по умолчанию
bool  fullscreen=true;              // Флаг режима окна, установленный в полноэкранный по умолчанию

LRESULT  CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);        // Прототип функции WndProc

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)        // Изменить размер и инициализировать окно GL
{
	my_drawResizeWindowOGL(width,height);
	return;
	//if (height==0)              // Предотвращение деления на ноль
	//{
	//	height=1;
	//}
	//glViewport(0,0,width,height);          // Сброс текущей области вывода 
	//glMatrixMode(GL_PROJECTION);            // Выбор матрицы проекций
	//glLoadIdentity();              // Сброс матрицы проекции
	//// Вычисление соотношения геометрических размеров для окна
	//gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
	//glMatrixMode(GL_MODELVIEW);            // Выбор матрицы вида модели
	//glLoadIdentity();              // Сброс матрицы вида модели
}
int InitGL(GLvoid)                // Все установки касаемо OpenGL происходят здесь
{
	my_initOGL();
	return true;                // Инициализация прошла успешно

	//glShadeModel(GL_SMOOTH);            // Разрешить плавное цветовое сглаживание
	//glClearColor(0.0f,0.0f,0.0f,0.0f);          // Очистка экрана в черный цвет
	//glClearDepth(1.0f);              // Разрешить очистку буфера глубины
	//glEnable(GL_DEPTH_TEST);            // Разрешить тест глубины
	//glDepthFunc(GL_LEQUAL);            // Тип теста глубины
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);      // Улучшение в вычислении перспективы
	//return true;                // Инициализация прошла успешно
}
int DrawGLScene(GLvoid)                // Здесь будет происходить вся прорисовка
{
	my_drawOGL();
	return true;                // Прорисовка прошла успешно

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);      // Очистить экран и буфер глубины
	//glLoadIdentity();              // Сбросить текущую матрицу
	//return true;                // Прорисовка прошла успешно
}
GLvoid KillGLWindow(GLvoid)              // Корректное разрушение окна
{
	if(fullscreen)              // Мы в полноэкранном режиме?
	{
		ChangeDisplaySettings(NULL,0);        // Если да, то переключаемся обратно в оконный режим
		ShowCursor(true);            // Показать курсор мышки
	}
	if(hRC)                // Существует ли Контекст Рендеринга?
	{
		if(!wglMakeCurrent(NULL,NULL))        // Возможно ли освободить RC и DC?
		{
			MessageBox(NULL,TEXT("Release Of DC And RC Failed."),TEXT("SHUTDOWN ERROR"),MB_OK | MB_ICONINFORMATION);
		}
		if(!wglDeleteContext(hRC))        // Возможно ли удалить RC?
		{
          MessageBox(NULL,TEXT("Release Rendering Context Failed."),TEXT("SHUTDOWN ERROR"),MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;              // Установить RC в NULL
	}
	if(hDC && !ReleaseDC(hWnd,hDC))          // Возможно ли уничтожить DC?
	{
		MessageBox(NULL,TEXT("Release Device Context Failed."),TEXT("SHUTDOWN ERROR"),MB_OK | MB_ICONINFORMATION);
		hDC=NULL;                // Установить DC в NULL
	}
	if(hWnd && !DestroyWindow(hWnd))            // Возможно ли уничтожить окно?
	{
		MessageBox(NULL,TEXT("Could Not Release hWnd."),TEXT("SHUTDOWN ERROR"),MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;                // Установить hWnd в NULL
	}
	if(!UnregisterClass(DEF_WINDOWCLASSNAME,hInstance))        // Возможно ли разрегистрировать класс
	{
		MessageBox(NULL,TEXT("Could Not Unregister Class."),TEXT("SHUTDOWN ERROR"),MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;                // Установить hInstance в NULL
	}
}

//рекурсивная функция, которая выбирает близайший возможный уровень мультисемплинга
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
	
	static GLuint    PixelFormat=0;              // Хранит результат после поиска

	WNDCLASS  wc;                // Структура класса окна
	DWORD    dwExStyle;              // Расширенный стиль окна
	DWORD    dwStyle;              // Обычный стиль окна
	RECT WindowRect;                // Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;              // Установить левую составляющую в 0
	WindowRect.right=(long)width;              // Установить правую составляющую в Width
	WindowRect.top=(long)0;                // Установить верхнюю составляющую в 0
	WindowRect.bottom=(long)height;              // Установить нижнюю составляющую в Height
	fullscreen=fullscreenflag;              // Устанавливаем значение глобальной переменной fullscreen

	hInstance=GetModuleHandle(NULL);        // Считаем дескриптор нашего приложения
	wc.style=CS_HREDRAW | CS_VREDRAW | CS_OWNDC;      // Перерисуем при перемещении и создаём скрытый DC
	wc.lpfnWndProc=(WNDPROC)WndProc;          // Процедура обработки сообщений
	wc.cbClsExtra=0;              // Нет дополнительной информации для окна
	wc.cbWndExtra=0;              // Нет дополнительной информации для окна
	wc.hInstance=hInstance;            // Устанавливаем дескриптор
	wc.hIcon=LoadIcon(NULL,IDI_WINLOGO);        // Загружаем иконку по умолчанию
	wc.hCursor=LoadCursor(NULL,IDC_ARROW);        // Загружаем указатель мышки
	wc.hbrBackground=NULL;              // Фон не требуется для GL
	wc.lpszMenuName=NULL;              // Меню в окне не будет
	wc.lpszClassName=DEF_WINDOWCLASSNAME;            // Устанавливаем имя классу

	if(!RegisterClass(&wc))              // Пытаемся зарегистрировать класс окна
	{
		MessageBox(NULL,TEXT("Failed To Register The Window Class."),TEXT("ERROR"),MB_OK | MB_ICONEXCLAMATION);
		return false;                // Выход и возвращение функцией значения false
	}

	if (fullscreen)                // Полноэкранный режим?
	{
		DEVMODE dmScreenSettings;            // Режим устройства
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));    // Очистка для хранения установок
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);      // Размер структуры Devmode
		dmScreenSettings.dmPelsWidth=width;        // Ширина экрана
		dmScreenSettings.dmPelsHeight=height;        // Высота экрана
		dmScreenSettings.dmBitsPerPel=bits;        // Глубина цвета
		dmScreenSettings.dmFields=DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;// Режим Пикселя
		// Пытаемся установить выбранный режим и получить результат.  Примечание: CDS_FULLSCREEN убирает панель управления.
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			// Если переключение в полноэкранный режим невозможно, будет предложено два варианта: оконный режим или выход.
			if (MessageBox(NULL,TEXT("The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?"),TEXT("NeHe GL"),MB_YESNO | MB_ICONEXCLAMATION)==IDYES )
			{
				fullscreen = false;          // Выбор оконного режима (fullscreen = false)
			}
			else
			{
				// Выскакивающее окно, сообщающее пользователю о закрытие окна.
				MessageBox(NULL,TEXT("Program Will Now Close."),TEXT("ERROR"), MB_OK | MB_ICONSTOP );
				return false;            // Выход и возвращение функцией false
			}
		}
	}
	if(fullscreen)                  // Мы остались в полноэкранном режиме?
	{
		dwExStyle=WS_EX_APPWINDOW;          // Расширенный стиль окна
		dwStyle=WS_POPUP;            // Обычный стиль окна
		ShowCursor(false);              // Скрыть указатель мышки
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;      // Расширенный стиль окна
		dwStyle=WS_OVERLAPPEDWINDOW;        // Обычный стиль окна
	}
	AdjustWindowRectEx(&WindowRect,dwStyle,false,dwExStyle);      // Подбирает окну подходящие размеры
	if(!(hWnd=CreateWindowEx(dwExStyle,          // Расширенный стиль для окна
		DEF_WINDOWCLASSNAME,          // Имя класса
		title,            // Заголовок окна
		WS_CLIPSIBLINGS |        // Требуемый стиль для окна
		WS_CLIPCHILDREN |        // Требуемый стиль для окна
		dwStyle,          // Выбираемые стили для окна
		0,0,            // Позиция окна
		WindowRect.right-WindowRect.left,    // Вычисление подходящей ширины
		WindowRect.bottom-WindowRect.top,    // Вычисление подходящей высоты
		NULL,            // Нет родительского
		NULL,            // Нет меню
		hInstance,          // Дескриптор приложения
		NULL)))          // Не передаём ничего до WM_CREATE (???)
	{
		KillGLWindow();                // Восстановить экран
		MessageBox(NULL,TEXT("Window Creation Error."),TEXT("ERROR"),MB_OK | MB_ICONEXCLAMATION);
		return false;                // Вернуть false
	}
	if(!(hDC=GetDC(hWnd)))              // Можем ли мы получить Контекст Устройства?
	{
		KillGLWindow();                // Восстановить экран
		MessageBox(NULL,TEXT("Can't Create A GL Device Context."),TEXT("ERROR"),MB_OK | MB_ICONEXCLAMATION);
		return false;                // Вернуть false
	}

	static  PIXELFORMATDESCRIPTOR pfd=            // pfd сообщает Windows каким будет вывод на экран каждого пикселя
		{
			sizeof(PIXELFORMATDESCRIPTOR),            // Размер дескриптора данного формата пикселей
			1,                  // Номер версии
			PFD_DRAW_TO_WINDOW |              // Формат для Окна
			PFD_SUPPORT_OPENGL |              // Формат для OpenGL	(****************************************************************************************************************************)
			PFD_DOUBLEBUFFER,              // Формат для двойного буфера
			PFD_TYPE_RGBA,                // Требуется RGBA формат
			bits,                  // Выбирается бит глубины цвета
			0, 0, 0, 0, 0, 0,              // Игнорирование цветовых битов
			0,                  // Нет буфера прозрачности
			0,                  // Сдвиговый бит игнорируется
			0,                  // Нет буфера накопления
			0, 0, 0, 0,                // Биты накопления игнорируются
			32,                  // 32 битный Z-буфер (буфер глубины)
			1,                  // Нет!!!ЕСТЬ!!! буфера трафарета				(***************************************************************************************************************************)
			0,                  // Нет вспомогательных буферов
			PFD_MAIN_PLANE,                // Главный слой рисования
			0,                  // Зарезервировано
			0, 0, 0                  // Маски слоя игнорируются
		};


	if (firstStartFunc)
	{
		if(!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))        // Найден ли подходящий формат пикселя?
		{
			KillGLWindow();                // Восстановить экран
			MessageBox(NULL,TEXT("Can't Find A Suitable PixelFormat."),TEXT("ERROR"),MB_OK | MB_ICONEXCLAMATION);
			return false;                // Вернуть false
		}
	}
	
	
	if(!SetPixelFormat(hDC,PixelFormat,&pfd))          // Возможно ли установить Формат Пикселя?
	{
		KillGLWindow();                // Восстановить экран
		MessageBox(NULL,TEXT("Can't Set The PixelFormat."),TEXT("ERROR"),MB_OK | MB_ICONEXCLAMATION);
		return false;                // Вернуть false
	}

	//DescribePixelFormat(hDC,PixelFormat,sizeof(tek_pf),&tek_pf);

	

	if(!(hRC=wglCreateContext(hDC)))          // Возможно ли установить Контекст Рендеринга?
	{
		KillGLWindow();                // Восстановить экран
		MessageBox(NULL,TEXT("Can't Create A GL Rendering Context."),TEXT("ERROR"),MB_OK | MB_ICONEXCLAMATION);
		return false;                // Вернуть false
	}
	
	if(!wglMakeCurrent(hDC,hRC))            // Попробовать активировать Контекст Рендеринга
	{
		KillGLWindow();                // Восстановить экран
		MessageBox(NULL,TEXT("Can't Activate The GL Rendering Context."),TEXT("ERROR"),MB_OK | MB_ICONEXCLAMATION);
		return false;                // Вернуть false
	}


	if (firstStartFunc)
	{
		//включение мультисемплинга
		//проверка поддержки расширения
		if (glewInit()==GLEW_OK)
		{
			if (glewIsSupported("GL_ARB_multisample"))
			{
				// Эти атрибуты – биты, которые мы хотим протестировать в нашем типе
				// Все довольно стандартно, только одно на чем мы хотим
				// действительно сфокусироваться - это SAMPLE BUFFERS ARB и WGL SAMPLES
				// Они выполнят главную проверку на предмет: есть или нет
				// у нас поддержка множественной выборки
				int iAttributes[] = {
					WGL_DRAW_TO_WINDOW_ARB,GL_TRUE, // Истинна, если формат пикселя может быть использован в окне
					WGL_SUPPORT_OPENGL_ARB,GL_TRUE, // Истинна, если поддерживается OpenGL
					WGL_ACCELERATION_ARB,WGL_FULL_ACCELERATION_ARB, // Полная аппаратная поддержка
					WGL_COLOR_BITS_ARB,24,          // Цветность
					WGL_ALPHA_BITS_ARB,8,           // Размерность альфа-канала
					WGL_DEPTH_BITS_ARB,16,          // Глубина буфера глубины
					WGL_STENCIL_BITS_ARB,0,         // Глубина буфера шаблона
					WGL_DOUBLE_BUFFER_ARB,GL_TRUE,  // Истина, если используется двойная буферизация
					WGL_SAMPLE_BUFFERS_ARB,GL_TRUE, // Что мы и хотим
					WGL_SAMPLES_ARB, MULTISAMPLE_LEVEL ,            // проверка на 4x тип
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






	ShowWindow(hWnd,SW_SHOW);              // Показать окно
	SetForegroundWindow(hWnd);              // Слегка повысим приоритет
	SetFocus(hWnd);                // Установить фокус клавиатуры на наше окно
	ReSizeGLScene(width,height);              // Настроим перспективу для нашего OpenGL экрана.
	if(!InitGL())                  // Инициализация только что созданного окна
	{
		KillGLWindow();                // Восстановить экран
		MessageBox(NULL,TEXT("Initialization Failed."),TEXT("ERROR"),MB_OK | MB_ICONEXCLAMATION);
		return false;                // Вернуть false
	}



	



	return true;                  // Всё в порядке!
}

LRESULT CALLBACK WndProc(  HWND  hWnd,            // Дескриптор нужного окна
	UINT  uMsg,            // Сообщение для этого окна
	WPARAM  wParam,            // Дополнительная информация
	LPARAM  lParam)            // Дополнительная информация
{
	switch (uMsg)                // Проверка сообщения для окна
	{
		case WM_ACTIVATE:            // Проверка сообщения активности окна
		{
			if(!HIWORD(wParam))          // Проверить состояние минимизации
			{
				active=true;          // Программа активна
			}
			else
			{
				active=false;          // Программа теперь не активна
			}
			return 0;            // Возвращаемся в цикл обработки сообщений
		}
		case WM_SYSCOMMAND:            // Перехватываем системную команду
		{
			switch (wParam)            // Останавливаем системный вызов
			{
				case SC_SCREENSAVE:        // Пытается ли запустится скринсейвер?
				case SC_MONITORPOWER:        // Пытается ли монитор перейти в режим сбережения энергии?
				return 0;          // Предотвращаем это
			}
			break;              // Выход

		}
		case WM_CLOSE:              // Мы получили сообщение о закрытие?
		{
			PostQuitMessage(0);          // Отправить сообщение о выходе
			return 0;            // Вернуться назад
		}
		case WM_KEYDOWN:            // Была ли нажата кнопка?
		{
			keys[wParam]=true;          // Если так, мы присваиваем этой ячейке true
			return 0;            // Возвращаемся
		}
		case WM_KEYUP:              // Была ли отпущена клавиша?
		{
			keys[wParam]=false;          //  Если так, мы присваиваем этой ячейке false
			return 0;            // Возвращаемся
		}
		case WM_SIZE:              // Изменены размеры OpenGL окна
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  // Младшее слово=Width, старшее слово=Height
			return 0;            // Возвращаемся
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
	// пересылаем все необработанные сообщения DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}
BOOL  gHalt=false;            // Логическая переменная для выхода из цикла

int WINAPI WinMain(HINSTANCE  hInstance,        // Дескриптор приложения
	HINSTANCE  hPrevInstance,        // Дескриптор родительского приложения
	LPSTR    lpCmdLine,        // Параметры командной строки
	int    nCmdShow)        // Состояние отображения окна
{
	MSG  msg;              // Структура для хранения сообщения Windows
	//BOOL  done=false;            // Логическая переменная для выхода из цикла

	if (MYPRGOGLMAINWINDOWSTART==MYPRGOGLMAINWINDOWSTARTFULLSRCASC)
	{
		// Спрашивает пользователя, какой режим экрана он предпочитает
		if (MessageBox(NULL,TEXT("Хотите ли Вы запустить приложение в полноэкранном режиме?"),TEXT("Запустить в полноэкранном режиме?"),MB_YESNO | MB_ICONQUESTION)==IDNO)
		{
			fullscreen = false;          // Оконный режим
		}
	}
	else
	{
		fullscreen=MYPRGOGLMAINWINDOWSTART;
	}



	// Создать наше OpenGL окно
	if(!CreateGLWindow(MYPRGOGLMAINWINDOWNAME,MYPRGOGLMAINWINDOWWIDTH,MYPRGOGLMAINWINDOWHEIGHT,32,fullscreen))
	{
		return 0;              // Выйти, если окно не может быть создано
	}

	while(!gHalt)                // Цикл продолжается, пока done не равно true
	{
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))    // Есть ли в очереди какое-нибудь сообщение?
		{
			if( msg.message==WM_QUIT )        // Мы поучили сообщение о выходе?
			{
				gHalt=true;          // Если так, done=true
			}
			else              // Если нет, обрабатывает сообщения
			{
				TranslateMessage(&msg);        // Переводим сообщение
				DispatchMessage(&msg);        // Отсылаем сообщение
			}
		}
		else                // Если нет сообщений
		{
			// Прорисовываем сцену.
			if(active)          // Активна ли программа?
			{
				if(keys[VK_ESCAPE])        // Было ли нажата клавиша ESC?
				{
					gHalt=true;      // ESC говорит об останове выполнения программы
				}
				else            // Не время для выхода, обновим экран.
				{
					my_keyboardTest(keys);
					DrawGLScene();        // Рисуем сцену
					SwapBuffers(hDC);    // Меняем буфер (двойная буферизация)
					
				}
			}
			if(keys[VK_F1])          // Была ли нажата F1?
			{
				keys[VK_F1]=false;        // Если так, меняем значение ячейки массива на false
				KillGLWindow();          // Разрушаем текущее окно
				fullscreen=!fullscreen;      // Переключаем режим
				// Пересоздаём наше OpenGL окно
				if(!CreateGLWindow(MYPRGOGLMAINWINDOWNAME,MYPRGOGLMAINWINDOWWIDTH,MYPRGOGLMAINWINDOWHEIGHT,32,fullscreen))
				{
					gHalt=true;       // Выходим, если это невозможно
				}
			}
		}
	}
	// Shutdown
	my_beforeExit();
	KillGLWindow();                // Разрушаем окно
	return((int)msg.wParam);              // Выходим из программы
}