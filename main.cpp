//-----------------------------------------------------------------------------
//           Name: ogl_initialization.cpp
//         Author: Kevin Harris
//  Last Modified: 02/01/05
//    Description: This sample demonstrates how to initialize OpenGL.
//-----------------------------------------------------------------------------

#define STRICT
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "resource.h"
#include <math.h>

#include <fstream.h>
		
        	int nWidth; 
			int nHeight;
//-----------------------------------------------------------------------------
// GLOBALS
//-----------------------------------------------------------------------------
HWND  g_hWnd = NULL;
HDC	  g_hDC  = NULL;
HGLRC g_hRC  = NULL;

//-----------------------------------------------------------------------------
// PROTOTYPES
//-----------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void init(void);
void shutDown(void);
void render(void);

//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------
int WINAPI WinMain( HINSTANCE hInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR     lpCmdLine,
                    int       nCmdShow )
{
    WNDCLASSEX winClass; 
	MSG        uMsg;

    memset(&uMsg,0,sizeof(uMsg));
    
	winClass.lpszClassName = "MY_WINDOWS_CLASS";
	winClass.cbSize        = sizeof(WNDCLASSEX);
	winClass.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	winClass.lpfnWndProc   = WindowProc;
	winClass.hInstance     = hInstance;
    winClass.hIcon	       = LoadIcon(hInstance, (LPCTSTR)IDI_OPENGL_ICON);
    winClass.hIconSm	   = LoadIcon(hInstance, (LPCTSTR)IDI_OPENGL_ICON);
	winClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	winClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	winClass.lpszMenuName  = NULL;
	winClass.cbClsExtra    = 0;
	winClass.cbWndExtra    = 0;

	if( !RegisterClassEx(&winClass) )
		return E_FAIL;

	g_hWnd = CreateWindowEx( 0, "MY_WINDOWS_CLASS", 
                             "OpenGL - Initialization",
						     WS_OVERLAPPEDWINDOW | WS_VISIBLE,
					         0, 0, 1024, 768, 0, 0, hInstance, 0 );

	if( g_hWnd == NULL )
		return E_FAIL;

    ShowWindow( g_hWnd, nCmdShow );
    UpdateWindow( g_hWnd );

	init();

	while( uMsg.message != WM_QUIT )
	{
		if( PeekMessage( &uMsg, NULL, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &uMsg );
			DispatchMessage( &uMsg );
		}
        else
			render();
	}

	shutDown();

    UnregisterClass( "MY_WINDOWS_CLASS", winClass.hInstance );

	return uMsg.wParam;
}

//-----------------------------------------------------------------------------
// Name: WindowProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT CALLBACK WindowProc( HWND   hWnd, 
							 UINT   msg, 
							 WPARAM wParam, 
							 LPARAM lParam )
{
    switch( msg )
	{
        case WM_KEYDOWN:
		{
			switch( wParam )
			{
				case VK_ESCAPE:
					PostQuitMessage(0);
					break;
			}
		}
        break;

		case WM_SIZE:
		{
			nWidth  = LOWORD(lParam); 
			nHeight = HIWORD(lParam);
			
            glViewport(0, 0, nWidth, nHeight);
            glMatrixMode( GL_PROJECTION );
			glLoadIdentity();

            
            //gluPerspective( 45.0, (GLdouble)nWidth / (GLdouble)nHeight, 0.1, 100.0);
		}
		break;

		case WM_CLOSE:
		{
			PostQuitMessage(0);	
		}

        case WM_DESTROY:
		{
            PostQuitMessage(0);
		}
        break;
		
		default:
		{
			return DefWindowProc( hWnd, msg, wParam, lParam );
		}
		break;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// Name: init()
// Desc: 
//-----------------------------------------------------------------------------
void init( void )
{
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

    pfd.nSize      = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion   = 1;
    pfd.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
	pfd.cDepthBits = 32;

	g_hDC = GetDC( g_hWnd );
	GLuint iPixelFormat = ChoosePixelFormat( g_hDC, &pfd );

	if( iPixelFormat != 0 )
	{
		PIXELFORMATDESCRIPTOR bestMatch_pfd;
		DescribePixelFormat( g_hDC, iPixelFormat, sizeof(pfd), &bestMatch_pfd );

		// TO DO: Double-check  the closet match pfd for anything unacceptable...

		if( bestMatch_pfd.cDepthBits < pfd.cDepthBits )
		{
			// POTENTIAL PROBLEM: We need at least a 16-bit z-buffer!
			return;
		}

		if( SetPixelFormat( g_hDC, iPixelFormat, &pfd) == FALSE )
		{
			DWORD dwErrorCode = GetLastError();
			// TO DO: Report cause of failure here...
			return;
		}
	}
	else
	{
		DWORD dwErrorCode = GetLastError();
		// TO DO: Report cause of failure here...
		return;
	}

	g_hRC = wglCreateContext( g_hDC );
	wglMakeCurrent( g_hDC, g_hRC );

	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
}

//-----------------------------------------------------------------------------
// Name: shutDown()
// Desc: 
//-----------------------------------------------------------------------------
void shutDown( void )	
{
	if( g_hRC != NULL )
	{
		wglMakeCurrent( NULL, NULL );
		wglDeleteContext( g_hRC );
		g_hRC = NULL;
	}

	if( g_hDC != NULL )
	{
		ReleaseDC( g_hWnd, g_hDC );
		g_hDC = NULL;
	}
}
//============================================================================================================
//------------------------------------------------------------------------------------------------------------
//============================================================================================================
GLfloat ship_01_POSITION[] = {2.0, 0.0, 0.0};
GLfloat ship_01_ROTATION[] = {0.0, 0.0, 0.0, 1.0};

GLfloat ship_02_POSITION[] = {-2.0, 0.0, 0.0};
GLfloat ship_02_ROTATION[] = {0.0, 0.0, 0.0, 1.0};

GLfloat blendedRotation = 0.0;
GLfloat startRotation   = 0.0;
GLfloat targetRotation  = 180.0;
GLfloat sliderTime      = 0.0;
GLfloat incrementTime   = 0.0;

GLfloat distanceBetweenModels[] = {0.0, 0.0, 0.0};

GLfloat angleAdjust = 0.0;

GLfloat up			    = 0.0;
GLfloat up_two	        = 0.0;
//============================================================================================================
//------------------------------------------------------------------------------------------------------------
//============================================================================================================
void render( void )	
{
	glDisable(GL_CULL_FACE);

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

			glViewport(0, 0, nWidth, nHeight);
            glMatrixMode( GL_PROJECTION );
			glLoadIdentity();


//========================================================================================

//========================================================================================

incrementTime += 0.00005;
//---------------------------

distanceBetweenModels[0] = ship_02_POSITION[0] - ship_01_POSITION[0];
distanceBetweenModels[1] = ship_02_POSITION[1] - ship_01_POSITION[1];

targetRotation = atan2(distanceBetweenModels[0], distanceBetweenModels[1])* 180 / 3.14159;

blendedRotation = startRotation * (1.0 - sliderTime) + targetRotation * sliderTime;

if(sliderTime <= 1.0) // This will prevent the rotation from over-shooting
{
        sliderTime += incrementTime;
}

glPushMatrix();
	glScalef(.1, .1, .1);

	
glPushMatrix();
	glTranslatef(ship_01_POSITION[0],ship_01_POSITION[1],ship_01_POSITION[2]);
    glRotatef(-blendedRotation ,ship_01_ROTATION[1],ship_01_ROTATION[2],ship_01_ROTATION[3]);
	glTranslatef(1.5f,0.0f,0.0f);
    glColor3f(0.0f,1.0f,0.0f);
	glBegin(GL_LINE_LOOP);
		glVertex3f(-1.5f,-0.25f,0.0f);
		glVertex3f(-1.75f,-0.5f,0.0f);
		glVertex3f(-2.0f,-0.5f,0.0f);
		glVertex3f(-1.5f,0.5f,0.0f);
		glVertex3f(-1.0f,-0.5f,0.0f);
		glVertex3f(-1.25f,-0.5f,0.0f);
		glVertex3f(-1.5f,-0.25f,0.0f);
	glEnd();
glPopMatrix();

glPushMatrix();
	glTranslatef(ship_02_POSITION[0],ship_02_POSITION[1], ship_02_POSITION[2]);
    glRotatef(ship_02_ROTATION[0], ship_02_ROTATION[1], ship_02_ROTATION[2], ship_02_ROTATION[3]);
	glTranslatef(-1.5f,0.0f,0.0f);
	glColor3f(1.0f,0.0f,0.0f);
	glBegin(GL_LINE_LOOP);
		glVertex3f(1.5f,-0.25f,0.0f);
		glVertex3f(1.25f,-0.5f,0.0f);
		glVertex3f(1.0f,-0.5f,0.0f);
		glVertex3f(1.5f,0.5f,0.0f);
		glVertex3f(2.0f,-0.5f,0.0f);
		glVertex3f(1.75f,-0.5f,0.0f);
		glVertex3f(1.5f,-0.25f,0.0f);
	glEnd();
glPopMatrix();
	
glPushMatrix();
	glColor3f(1.0f,0.0f,0.0f);
	glPointSize(2.0f);
	glBegin(GL_POINTS);
		glVertex3f(1.5f,up_two++,0.0f);
	glEnd();
glPopMatrix();

glPushMatrix();
	glColor3f(0.0f,1.0f,0.0f);
	glPointSize(2.0f);
	glBegin(GL_POINTS);
		glVertex3f(-1.5f,up++,0.0f);
	glEnd();
glPopMatrix();


glPopMatrix();

	SwapBuffers( g_hDC );
}
