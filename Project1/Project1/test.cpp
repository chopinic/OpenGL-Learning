//#include <GLFW\glfw3.h>
//#include <glad/glad.h> 
//#include <GLFW/glfw3.h>
#define _CRT_SECURE_NO_DEPRECATE
#include <stdlib.h>
#include <stdio.h>			// Header File For Standard Input/Output
#include <windows.h>		// Windows��ͷ�ļ�
#include <iostream>
#include <gl.h>			// Header File For The OpenGL32 Library
#include <glu.h>			// Header File For The GLu32 Library
#include <glaux.h>		// Header File For The Glaux Library
#include "NeHeGL.h"
#pragma comment(lib, "glaux.lib")
#pragma comment(lib, "glu.lib")
#pragma comment(lib, "legacy_stdio_definitions.lib")

HGLRC           hRC = NULL;							// ������ɫ��������
HDC             hDC = NULL;							// OpenGL��Ⱦ��������
HWND            hWnd = NULL;							// �������ǵĴ��ھ��
HINSTANCE       hInstance;
RECT	rect;
GLuint	texture[1];			// Storage For 1 Texture
bool	keys[256];								// ������̰���������
bool	active = TRUE;								// ���ڵĻ��־��ȱʡΪTRUE
bool	fullscreen = TRUE;							// ȫ����־ȱʡ��ȱʡ�趨��ȫ��ģʽ
bool w_press=false, s_press = false, a_press = false, d_press = false;
GLfloat	xrote=0, yrote=0, zrote=0;
GLfloat speed = 0.001f,xspeed = 0.0f,yspeed=0.0f,zspeed = 0.0f;
//GLfloat		rtri=5;						// ���������εĽǶ�
GLfloat		rquad=9;						// �����ı��εĽǶ�
GLuint	box;						// ������ӵ���ʾ�б�
LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);				// WndProc�Ķ���
GLvoid ReSizeGLScene(GLsizei width, GLsizei height)				// ����OpenGL���ڴ�С
{
	if (height == 0)								// ��ֹ�����
	{
		height = 1;							// ��Height��Ϊ1
	}
	rect.right = width;
	rect.top = height;
	glViewport(0, 0, width, height);					// ���õ�ǰ���ӿ�
	
}
AUX_RGBImageRec* LoadBMP(const char* Filename)				// Loads A Bitmap Image
{
	FILE* File = NULL;									// File Handle

	if (!Filename)										// Make Sure A Filename Was Given
	{
		return NULL;									// If Not Return NULL
	}

	File = fopen(Filename, "r");							// Check To See If The File Exists

	if (File)											// Does The File Exist?
	{
		fclose(File);									// Close The Handle
		return auxDIBImageLoad(Filename);				// Load The Bitmap And Return A Pointer
	}

	return NULL;										// If Load Failed Return NULL
}

GLvoid BuildLists()
{
	box = glGenLists(1);				// ����������ʾ�б������
	glNewList(box, GL_COMPILE);
	glBegin(GL_QUADS);							// ��ʼ�����ı���
			// ����
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	// ǰ��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	// ����
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	// ����
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	// ����
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glEnd();								// �ı��λ��ƽ���
	glEndList();									

}
int LoadGLTextures()									// Load Bitmaps And Convert To Textures
{
	int Status = FALSE;									// Status Indicator

	AUX_RGBImageRec* TextureImage[1];					// Create Storage Space For The Texture

	memset(TextureImage, 0, sizeof(void*) * 1);           	// Set The Pointer To NULL

	// Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit
	if (TextureImage[0] = LoadBMP("Cube.bmp"))
	{
		Status = TRUE;									// Set The Status To TRUE

		glGenTextures(1, &texture[0]);					// Create The Texture

		// Typical Texture Generation Using Data From The Bitmap
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	if (TextureImage[0])									// If Texture Exists
	{
		if (TextureImage[0]->data)							// If Texture Image Exists
		{
			free(TextureImage[0]->data);					// Free The Texture Image Memory
		}

		free(TextureImage[0]);								// Free The Image Structure
	}

	return Status;										// Return The Status
}
int InitGL(GLvoid)								// �˴���ʼ��OpenGL������������
{
	rect.bottom = 0;
	rect.top = 480;
	rect.left = 0;
	rect.right = 640;
	if (!LoadGLTextures())								// Jump To Texture Loading Routine
	{
		return FALSE;									// If Texture Didn't Load Return FALSE
	}
	BuildLists();										// Jump To The Code That Creates Our Display Lists
	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
	glShadeModel(GL_SMOOTH);						// ������Ӱƽ��
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);					// ��ɫ����
	glClearDepth(1.0f);							// ������Ȼ���
	glEnable(GL_DEPTH_TEST);						// ������Ȳ���
	glDepthFunc(GL_LEQUAL);							// ������Ȳ��Ե�����
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			// ����ϵͳ��͸�ӽ�������
	return TRUE;								// ��ʼ�� OK
}
int DrawGLScene(GLvoid)								// �����￪ʼ�������еĻ���
{

	int window_width = rect.right - rect.left;								// Calculate The Width (Right Side-Left Side)
	int window_height = rect.bottom - rect.top;								// Calculate The Height (Bottom-Top)
	glBindTexture(GL_TEXTURE_2D, texture[0]);		// ѡ������
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// �����Ļ����Ȼ���
	for (int i = 0; i < 2; i++)
	{
		if (i == 0)
		{
			glViewport(0, 0, window_width / 2, window_height);
			glMatrixMode(GL_PROJECTION);								// Select The Projection Matrix
			glLoadIdentity();
			gluPerspective(45.0, 1, 0.1f, 500.0);

			glMatrixMode(GL_MODELVIEW);									// Select The Modelview Matrix
			glLoadIdentity();												// Reset The Modelview Matrix
			glClear(GL_DEPTH_BUFFER_BIT);									// Clear Depth Buffer
			glTranslatef(-1.5f, 0.0f, -6.0f);				// ���� 1.5 ��λ����������Ļ 6.0
			glColor3f(1.0f, 1.0f, 1.0f);
			glRotatef(yrote, 0.0f, 1.0f, 0.0f);				// ��Y����ת������
			glRotatef(xrote, 1.0f, 0.0f, 0.0f);
			glRotatef(zrote, 0.0f, 0.0f, 1.0f);
			//gluOrtho2D(0, window_width / 2, window_height, 0);
			glCallList(box);			// ���ƺ���
		}
		if (i == 1)
		{
			glViewport(window_width / 2, 0, window_width / 2, window_height);
			glMatrixMode(GL_PROJECTION);								// Select The Projection Matrix
			glLoadIdentity();
			gluPerspective(45.0, 1, 0.1f, 500.0);
			glMatrixMode(GL_MODELVIEW);									// Select The Modelview Matrix
			glLoadIdentity();
			glBegin(GL_TRIANGLES);							// ����������
			glColor3f(1.0, 0.0, 0.0);
			glVertex3f(0.0f, 1.0f, 0.0f);					// �϶���
			glColor3f(0.0, 1.0, 0.0);
			glVertex3f(-0.5f, 0.0f, 0.0f);					// ����
			glColor3f(0.0, 0.0, 1.0);
			glVertex3f(0.5f, 0.0f, 0.0f);					// ����
			glEnd();
		}
	}


	
	xrote += xspeed;
	yrote += yspeed;
	zrote += zspeed;
	//rtri += 0.2f;						// ���������ε���ת����

	//rquad -= 0.15f;						// �����ı��ε���ת����
	return TRUE;						// ��������
}
GLvoid KillGLWindow(GLvoid)							// �������ٴ���
{
	if (fullscreen)								// ���Ǵ���ȫ��ģʽ��?
	{
		ChangeDisplaySettings(NULL, 0);					// �ǵĻ����л�������
		ShowCursor(TRUE);						// ��ʾ���ָ��
	}
	if (hRC)								// ����ӵ��OpenGL��Ⱦ��������?
	{
		if (!wglMakeCurrent(NULL, NULL))					// �����ܷ��ͷ�DC��RC������?
		{
			MessageBox(NULL, "�ͷ�DC��RCʧ�ܡ�", "�رմ���", MB_OK | MB_ICONINFORMATION);
		}
		if (!wglDeleteContext(hRC))					// �����ܷ�ɾ��RC?
		{
			MessageBox(NULL, "�ͷ�RCʧ�ܡ�", "�رմ���", MB_OK | MB_ICONINFORMATION);
		}
		hRC = NULL;							// ��RC��Ϊ NULL
	}
	if (hDC && !ReleaseDC(hWnd, hDC))					// �����ܷ��ͷ� DC?
	{
		MessageBox(NULL, "�ͷ�DCʧ�ܡ�", "�رմ���", MB_OK | MB_ICONINFORMATION);
		hDC = NULL;							// �� DC ��Ϊ NULL
	}
	if (hWnd && !DestroyWindow(hWnd))					// �ܷ����ٴ���?
	{
		MessageBox(NULL, "�ͷŴ��ھ��ʧ�ܡ�", "�رմ���", MB_OK | MB_ICONINFORMATION);
		hWnd = NULL;							// �� hWnd ��Ϊ NULL
	}
	if (!UnregisterClass("OpenG", hInstance))				// �ܷ�ע����?
	{
		MessageBox(NULL, "����ע�������ࡣ", "�رմ���", MB_OK | MB_ICONINFORMATION);
		hInstance = NULL;							// �� hInstance ��Ϊ NULL
	}
}
BOOL CreateGLWindow(const char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;						// �������ƥ��Ľ��
	WNDCLASS	wc;							// ������ṹ
	DWORD		dwExStyle;						// ��չ���ڷ��
	DWORD		dwStyle;						// ���ڷ��
	RECT WindowRect;							// ȡ�þ��ε����ϽǺ����½ǵ�����ֵ
	WindowRect.left = (long)0;						// ��Left   ��Ϊ 0
	WindowRect.right = (long)width;						// ��Right  ��ΪҪ��Ŀ��
	WindowRect.top = (long)0;							// ��Top    ��Ϊ 0
	WindowRect.bottom = (long)height;						// ��Bottom ��ΪҪ��ĸ߶�
	fullscreen = fullscreenflag;						// ����ȫ��ȫ����־
	hInstance = GetModuleHandle(NULL);			// ȡ�����Ǵ��ڵ�ʵ��
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;		// �ƶ�ʱ�ػ�����Ϊ����ȡ��DC
	wc.lpfnWndProc = (WNDPROC)WndProc;				// WndProc������Ϣ
	wc.cbClsExtra = 0;						// �޶��ⴰ������
	wc.cbWndExtra = 0;						// �޶��ⴰ������
	wc.hInstance = hInstance;					// ����ʵ��
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);			// װ��ȱʡͼ��
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);			// װ�����ָ��
	wc.hbrBackground = NULL;						// GL����Ҫ����
	wc.lpszMenuName = NULL;						// ����Ҫ�˵�
	wc.lpszClassName = "OpenG";					// �趨������
	if (!RegisterClass(&wc))						// ����ע�ᴰ����
	{
		MessageBox(NULL, "ע�ᴰ��ʧ��", "����", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;							// �˳�������FALSE
	}
	if (fullscreen)								// Ҫ����ȫ��ģʽ��?
	{
		DEVMODE dmScreenSettings;						// �豸ģʽ
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));			// ȷ���ڴ����Ϊ��
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);			// Devmode �ṹ�Ĵ�С
		dmScreenSettings.dmPelsWidth = width;				// ��ѡ��Ļ���
		dmScreenSettings.dmPelsHeight = height;				// ��ѡ��Ļ�߶�
		dmScreenSettings.dmBitsPerPel = bits;					// ÿ������ѡ��ɫ�����
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		// ����������ʾģʽ�����ؽ����ע: CDS_FULLSCREEN ��ȥ��״̬����
		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			// ��ģʽʧ�ܣ��ṩ����ѡ��˳����ڴ��������С�
			if (MessageBox(NULL, "ȫ��ģʽ�ڵ�ǰ�Կ�������ʧ�ܣ�\nʹ�ô���ģʽ��", "NeHe G", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
			{
				fullscreen = FALSE;				// ѡ�񴰿�ģʽ(Fullscreen=FALSE)
			}
			else
			{
				// ����һ���Ի��򣬸����û��������
				MessageBox(NULL, "���򽫱��ر�", "����", MB_OK | MB_ICONSTOP);
				return FALSE;					//  �˳������� FALSE
			}
		}
	}
	if (fullscreen)								// �Դ���ȫ��ģʽ��?
	{
		dwExStyle = WS_EX_APPWINDOW;					// ��չ������
		dwStyle = WS_POPUP;						// ������
		ShowCursor(FALSE);						// �������ָ��
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// ��չ������
		dwStyle = WS_OVERLAPPEDWINDOW;					//  ������
	}
	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// �������ڴﵽ����Ҫ��Ĵ�С
	if (!(hWnd = CreateWindowEx(dwExStyle,				// ��չ������
		"OpenG",				// ������
		title,					// ���ڱ���
		WS_CLIPSIBLINGS |			// ����Ĵ���������
		WS_CLIPCHILDREN |			// ����Ĵ���������
		dwStyle,				// ѡ��Ĵ�������
		0, 0,					// ����λ��
		WindowRect.right - WindowRect.left,	// ��������õĴ��ڿ��
		WindowRect.bottom - WindowRect.top,	// ��������õĴ��ڸ߶�
		NULL,					// �޸�����
		NULL,					// �޲˵�
		hInstance,				// ʵ��
		NULL)))					// ����WM_CREATE�����κζ���
	{
		KillGLWindow();							// ������ʾ��
		MessageBox(NULL, "���ܴ���һ�������豸������", "����", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;							// ���� FALSE
	}
	static	 PIXELFORMATDESCRIPTOR pfd =					// /pfd ���ߴ���������ϣ���Ķ�����������ʹ�õ����ظ�ʽ
	{
		sizeof(PIXELFORMATDESCRIPTOR),					// ������ʽ�������Ĵ�С
		1,								// �汾��
		PFD_DRAW_TO_WINDOW |						// ��ʽ֧�ִ���
		PFD_SUPPORT_OPENGL |						// ��ʽ����֧��OpenGL
		PFD_DOUBLEBUFFER,						// ����֧��˫����
		PFD_TYPE_RGBA,							// ���� RGBA ��ʽ
		bits,								// ѡ��ɫ�����
		0, 0, 0, 0, 0, 0,						// ���Ե�ɫ��λ
		0,								// ��Alpha����
		0,								// ����Shift Bit
		0,								// ���ۼӻ���
		0, 0, 0, 0,							// ���Ծۼ�λ
		16,								// 16λ Z-���� (��Ȼ���)
		0,								// ���ɰ建��
		0,								// �޸�������
		PFD_MAIN_PLANE,							// ����ͼ��
		0,								// Reserved
		0, 0, 0								// ���Բ�����
	};
	if (!(hDC = GetDC(hWnd)))							// ȡ���豸��������ô?
	{
		KillGLWindow();							// ������ʾ��
		MessageBox(NULL, "���ܴ���һ����ƥ������ظ�ʽ", "����", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;							// ���� FALSE
	}
	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd)))				// Windows �ҵ���Ӧ�����ظ�ʽ����?
	{
		KillGLWindow();							// ������ʾ��
		MessageBox(NULL, "�����������ظ�ʽ", "����", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;							// ���� FALSE
	}
	if (!SetPixelFormat(hDC, PixelFormat, &pfd))				// �ܹ��������ظ�ʽô?
	{
		KillGLWindow();							// ������ʾ��
		MessageBox(NULL, "�����������ظ�ʽ", "����", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;							// ���� FALSE
	}
	if (!(hRC = wglCreateContext(hDC)))					// �ܷ�ȡ����ɫ������?
	{
		KillGLWindow();							// ������ʾ��
		MessageBox(NULL, "���ܴ���OpenGL��Ⱦ������", "����", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;							// ���� FALSE
	}
	if (!wglMakeCurrent(hDC, hRC))						// ���Լ�����ɫ������
	{
		KillGLWindow();							// ������ʾ��
		MessageBox(NULL, "���ܼ��ǰ��OpenGL��Ȼ������", "����", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;							// ���� FALSE
	}
	ShowWindow(hWnd, SW_SHOW);						// ��ʾ����
	SetForegroundWindow(hWnd);						// ����������ȼ�
	SetFocus(hWnd);								// ���ü��̵Ľ������˴���
	ReSizeGLScene(width, height);						// ����͸�� GL ��Ļ
	if (!InitGL())								// ��ʼ���½���GL����
	{
		KillGLWindow();							// ������ʾ��
		MessageBox(NULL, "Initialization Failed.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;							// ���� FALSE
	}
	return TRUE;								// �ɹ�
}
LRESULT CALLBACK WndProc(HWND	hWnd, UINT	uMsg,	 WPARAM	wParam, LPARAM	lParam)					// ���ӵ���Ϣ����
{
	switch (uMsg)								// ���Windows��Ϣ
	{
		case WM_ACTIVATE:						// ���Ӵ��ڼ�����Ϣ
		{
			if (!HIWORD(wParam))					// �����С��״̬
			{
				active = TRUE;					// �����ڼ���״̬
			}
			else
			{
				active = FALSE;					// �����ټ���
			}

			return 0;						// ������Ϣѭ��
		}
		case WM_SYSCOMMAND:						// ϵͳ�ж�����
		{
			switch (wParam)						// ���ϵͳ����
			{
			case SC_SCREENSAVE:				// ����Ҫ����?
			case SC_MONITORPOWER:				// ��ʾ��Ҫ����ڵ�ģʽ?
				return 0;					// ��ֹ����
			}
			break;							// �˳�
		}
		case WM_CLOSE:							// �յ�Close��Ϣ?
		{
			PostQuitMessage(0);					// �����˳���Ϣ
			return 0;						// ����
		}
		case WM_KEYDOWN:						// �м�����ô?
		{
			keys[wParam] = TRUE;					// ����ǣ���ΪTRUE
			return 0;						// ����
		}
		case WM_KEYUP:							// �м��ſ�ô?
		{
			keys[wParam] = FALSE;					// ����ǣ���ΪFALSE
			return 0;						// ����
		}
		case WM_SIZE:							// ����OpenGL���ڴ�С
		{
			ReSizeGLScene(LOWORD(lParam), HIWORD(lParam));		// LoWord=Width,HiWord=Height
			return 0;						// ����
		}
	}
	// �� DefWindowProc��������δ�������Ϣ��
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
int WINAPI WinMain(HINSTANCE	hInstance,	 HINSTANCE	hPrevInstance,	 LPSTR		lpCmdLine,	 int		nCmdShow)				// ������ʾ״̬
{
	MSG	msg;								// Windowsx��Ϣ�ṹ
	BOOL	done = FALSE;							// �����˳�ѭ����Bool ����
// ��ʾ�û�ѡ������ģʽ
	if (MessageBox(NULL, "������ȫ��ģʽ������ô��", "����ȫ��ģʽ", MB_YESNO | MB_ICONQUESTION) == IDNO)
	{
		fullscreen = FALSE;						// FALSEΪ����ģʽ
	}
	// ����OpenGL����
	if (!CreateGLWindow("NeHe's OpenGL������", 640, 480, 16, fullscreen))
	{
		return 0;							// ʧ���˳�
	}
	while (!done)								// ����ѭ��ֱ�� done=TRUE
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))			// ����Ϣ�ڵȴ���?
		{
			if (msg.message == WM_QUIT)				// �յ��˳���Ϣ?
			{
				done = TRUE;					// �ǣ���done=TRUE
			}
			else							// ���ǣ���������Ϣ
			{
				TranslateMessage(&msg);				// ������Ϣ
				DispatchMessage(&msg);				// ������Ϣ
			}
		}
		else								// ���û����Ϣ
		{
			// ���Ƴ���������ESC��������DrawGLScene()���˳���Ϣ
			if (active)						// ���򼤻��ô?
			{
				if (keys[VK_ESCAPE])				// ESC ������ô?
				{
					done = TRUE;				// ESC �����˳��ź�
				}
				else						// �����˳���ʱ��ˢ����Ļ
				{
					DrawGLScene();				// ���Ƴ���
					SwapBuffers(hDC);			// �������� (˫����)
					if (keys['W'])
					{
						w_press = true;
						yspeed+= speed;
					}
					

					if (keys['S'] )
					{
						s_press = true;
						yspeed -= speed;
					}
					
					if (keys['A'] )
					{
						a_press = true;
						xspeed -= speed;
					}
					
					if (keys['D'])
					{
						d_press = true;
						xspeed += speed;
					}
					if (keys['Q'])
						zspeed -= speed;
					if (keys['E'])
						zspeed += speed;

				}
			}
			if (keys[VK_F1])					// F1��������ô?
			{
				keys[VK_F1] = FALSE;				// ���ǣ�ʹ��Ӧ��Key�����е�ֵΪ FALSE
				KillGLWindow();					// ���ٵ�ǰ�Ĵ���
				fullscreen = !fullscreen;				// �л� ȫ�� / ���� ģʽ
				// �ؽ� OpenGL ����
				if (!CreateGLWindow("NeHe's OpenGL ��һ��ͼ�γ���", 640, 480, 16, fullscreen))
				{
					return 0;				// �������δ�ܴ����������˳�
				}
			}

		}
	}
	// �رճ���
	KillGLWindow();								// ���ٴ���
	return (msg.wParam);							// �˳�����
}
int main()
{
	InitGL();
	WinMain(hInstance,NULL,0,0);


}

