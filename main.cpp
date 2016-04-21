/****************************************************************************
			72 Hour Game Development Contest!

 All code (c) Tim Graupmann 2003, 2004.

 Feel free to use any of this code with only the following condition:

 1) The code is not supported in any way. I am not responsible for any
    harm caused to your machine (although I have made every attemp to check
    for any possibly damaging stuff)
 
 Enjoy! :)

 Credits:
   A lot of my code was based on tutorials by GT (www.gametutorials.com).   

****************************************************************************/

#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glaux.h>
#include <math.h>

// Include the basic libraries
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glaux.lib")
#pragma comment(lib, "winmm.lib")

// Our custom classes
#include "c3dobjects.h"
#include "fonts.h"
#include "midi.h"
#include "resource.h"

// Our global game variables
c3dCamera g_Camera;
#define MAX_TEXTURES 7
UINT g_Texture[MAX_TEXTURES];
double g_MyScore = 0;

// Our global WinMain variables
UINT  g_ScreenWidth = 640, g_ScreenHeight = 480;
bool  g_bFullScreen = false;
HWND  g_hWnd;
RECT  g_rRect;
HDC   g_hDC;
HGLRC g_hRC;
HINSTANCE g_hInstance;
#define SCREEN_DEPTH 8
HFONT hOldFont;
UINT g_FontListID = 0;

void ShowMessage(UINT X, UINT Y, UINT Width, UINT Height, UINT FontListID, char *Message)
{
	// This will display text fonts with nice transparent backgrounds

	if(Message)
	{
		glColor3ub(255, 255, 255);
		glDrawText(X+10, Y, FontListID, 32, Message);
	}

	glColor4ub(0, 0, 0, 128);
	glBegin(GL_QUADS);
		glVertex2i(X, g_rRect.bottom - Y);
		glVertex2i(X, g_rRect.bottom - (Y + Height));
		glVertex2i(X+Width, g_rRect.bottom - (Y + Height));
		glVertex2i(X+Width, g_rRect.bottom - Y);
	glEnd();

	glColor3ub(255, 255, 255);
}

void CreateTexture(UINT textureArray[], LPSTR strFileName, int textureID)
{
	AUX_RGBImageRec *pBitmap = NULL;
	
	// Return from the function if no file name was passed in
	if(!strFileName)
		return;
	
	// Use the glaux.lib to load the image
	// it will popup an error if there is one.
	pBitmap = auxDIBImageLoad(strFileName); // Store image
	
	if(pBitmap == NULL) // If the texture couldn't be loaded exit
		exit(0);

	// Generate a texture with the associative texture ID stored in the array
	glGenTextures(1, &textureArray[textureID]);

	// Bind the texture to the texture arrays index and init the texture
	glBindTexture(GL_TEXTURE_2D, textureArray[textureID]);

	// Build Mipmaps (builds different versions of the picture for distances - looks better)
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pBitmap->sizeX, pBitmap->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pBitmap->data);

	// Lastly, we need to tell OpenGL the quality of our texture map.  GL_LINEAR_MIPMAP_LINEAR
	// is the smoothest.  GL_LINEAR_MIPMAP_NEAREST is faster than GL_LINEAR_MIPMAP_LINEAR, 
	// but looks blochy and pixilated.  Good for slower computers though.		
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

	// Now we need to free the bitmap data that we loaded since openGL stored it as a texture

	if (pBitmap)										// If we loaded the bitmap
	{
		if (pBitmap->data)								// If there is texture data
		{
			free(pBitmap->data);						// Free the texture data, we don't need it anymore
		}

		free(pBitmap);									// Free the bitmap structure
	}
}

c3dCandyCorn	g_CandyCorns	[	MAX_CANDYCORNS		];
c3dCandyGobber	g_CandyGobbers	[	MAX_CANDYGOBBERS	];
c3dBubbleGum	g_BubbleGums	[	MAX_BUBBLEGUMS		];
c3dSpeedBoost	g_SpeedBoosts	[	MAX_SPEEDBOOSTS		];

c3dOrange g_orange(0, 1, 0,  0, 1, 1,  0, 1, 0,  0, 270, 0, 0, 1);

void InitGame(void)
{
	// Enable environment
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.24f, 0.37f, 0.80f, 0.5f); // Nice blue background
	
	// Load textures
	CreateTexture(g_Texture, "data/orange_lady.bmp", 0);
	CreateTexture(g_Texture, "data/orange_mario.bmp", 1);
	CreateTexture(g_Texture, "data/ground_texture.bmp", 2);
	CreateTexture(g_Texture, "data/corn_texture.bmp", 3);
	CreateTexture(g_Texture, "data/gum_texture.bmp", 4);
	CreateTexture(g_Texture, "data/speed_texture.bmp", 5);
	CreateTexture(g_Texture, "data/gobber_texture.bmp", 6);

	// Position Objects
	float Z = -5;
	for(UINT i=0; i<MAX_CANDYCORNS; i++, Z+=0.5f)
	{
		float X;
		if(i%2)
			X = -2.5f;
		else
			X = 2.5f;
		g_CandyCorns[i] = c3dCandyCorn(X, 0, Z,  X, 20, Z,  0, 1, 0,  3, 0, 0, 2);
	}

	Z = -5;
	for(i=0; i<MAX_CANDYGOBBERS; i++, Z++)
	{
		float X;
		if(i%2)
			X = -5;
		else
			X = 5;
		g_CandyGobbers[i] = c3dCandyGobber(X, 0.5f, Z,  X, 0.5f, Z+1,  0, 1, 0,  6, 0, 0, 0.5f, 2);
	}

	for(i=0; i<MAX_BUBBLEGUMS; i++)
		g_BubbleGums[i] = c3dBubbleGum(0, 1, 0,  0, 2, 0,  0, 1, 0,  4, 0, 0, 0);

	for(i=0; i<MAX_SPEEDBOOSTS; i++)
		g_SpeedBoosts[i] = c3dSpeedBoost(0, 1, 0,  0, 2, 0,  0, 1, 0,  5, 0, 0, 0);
	
	g_MyScore = 0;
	g_orange = c3dOrange(0, 1, 0,  0, 1, 1,  0, 1, 0,  g_orange.m_TextureID, 270, 0, 0, 1);
}

///////////////////////////////// UPDATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function updates all the logic in the game.
/////
///////////////////////////////// UPDATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void Update(void)
{
	// Update the bubble gums
	for(UINT i=0; i<MAX_BUBBLEGUMS; i++)
		g_BubbleGums[i].Update();

	// Update the speed boosts
	for(i=0; i<MAX_SPEEDBOOSTS; i++)
		g_SpeedBoosts[i].Update();
	
	// Update the candy corns
	for(i=0; i<MAX_CANDYCORNS; i++)
		g_CandyCorns[i].Update();

	// Update the gobbers
	for(i=0; i<MAX_CANDYGOBBERS; i++)
		g_CandyGobbers[i].Update();

	// Update the orange
	g_orange.Update();

	// Place camera just behind the global orange -- 3rd person perspective
	//
	//
	// Calculate where to put the camera
	CVector3 vPos = g_orange.m_vPosition;
	CVector3 vView = Normalize( g_orange.m_vView - g_orange.m_vPosition );
	vView = vView * -8 * sqrtf(g_orange.m_Radius);
	//
	// And then place the camera
	if(g_orange.m_State)
	{
		g_Camera.PositionObject(
			vPos.X+vView.X, vPos.Y+vView.Y+g_orange.m_HeightOffset, vPos.Z+vView.Z,
			vPos.X, vPos.Y+g_orange.m_HeightOffset, vPos.Z,
			0, 1, 0);
	}
	else
	{
		g_Camera.PositionObject(
			vPos.X+vView.Z, vPos.Y+vView.Y+g_orange.m_HeightOffset+5, vPos.X+vView.Z,
			vPos.X, vPos.Y+g_orange.m_HeightOffset, vPos.Z,
			0, 1, 0);
	}
}

///////////////////////////////// RENDER \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function handles the display for the rendering code.
/////
///////////////////////////////// RENDER \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void Render(void)
{
	// Set our viewport to it's normal size
	glViewport(0, 0, g_rRect.right, g_rRect.bottom);

	// DRAW GROUND AND SKY WITH NEAR CLIPPING PLAN AT 0.1f

	// Setup our drawing mode
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,(GLfloat)g_rRect.right/(GLfloat)g_rRect.bottom, 0.1f, 30);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Clear the viewport
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Place our camera
	g_Camera.Look();

	// Turn on texturing
	glEnable(GL_TEXTURE_2D);

	//Draw Grid at exactly where the camera is
	CVector3 vPos = g_orange.m_vPosition;
	glBindTexture(GL_TEXTURE_2D, g_Texture[2]);
	glBegin(GL_QUADS);
			glTexCoord2f(   0,    20);
			glVertex3i(int(vPos.X)-20, 0, int(vPos.Z)+20);
			glTexCoord2f(   0,    0);
			glVertex3i(int(vPos.X)-20, 0, int(vPos.Z)-20);
			glTexCoord2f(   20,    0);
			glVertex3i(int(vPos.X)+20, 0, int(vPos.Z)-20);
			glTexCoord2f(   20,    20);
			glVertex3i(int(vPos.X)+20, 0, int(vPos.Z)+20);
	glEnd();
	
	// Draw the bubble gums
	for(UINT i=0; i<MAX_BUBBLEGUMS; i++)
		g_BubbleGums[i].Render();

	// Draw the speed boosts
	for(i=0; i<MAX_SPEEDBOOSTS; i++)
		g_SpeedBoosts[i].Render();

	// Draw candy corns
	for(i=0; i<MAX_CANDYCORNS; i++)
		g_CandyCorns[i].Render();

	// Update the gobbers
	for(i=0; i<MAX_CANDYGOBBERS; i++)
		g_CandyGobbers[i].Render();

	// Draw orange character
	g_orange.Render();

	// Turn off textures to display font
	glDisable(GL_TEXTURE_2D);

	char message[50] = {0};
	if(!g_orange.m_State)
		sprintf(message, "-- GAME OVER -- Score: %.0lf", g_MyScore);
	else
		sprintf(message, "Score: %.0lf", g_MyScore);
	ShowMessage(10, 10, 600, 50, g_FontListID, message);
	sprintf(message, "Health: %0.0f PERCENT", (g_orange.m_Radius-0.4f)/0.6f*100);
	ShowMessage(10, 10+FONT_HEIGHT, 600, 50, g_FontListID, message);
	sprintf(message, "Bubble Gum: %d / INFINITE", g_orange.m_BubbleQty);
	ShowMessage(10, 10+2*FONT_HEIGHT, 600, 50, g_FontListID, message);	
	sprintf(message, "Speed Boosts: %d / 5", g_orange.m_SpeedBoostQty);
	ShowMessage(10, 10+3*FONT_HEIGHT, 600, 50, g_FontListID, message);
	switch(g_orange.m_LastOpponent)
	{
	case TYPE_CANDYCORN:
		sprintf(message, "Last Opponent: CANDY CORN"); break;
	case TYPE_CANDYGOBBER:
		sprintf(message, "Last Opponent: CANDY GOBBER"); break;
	case TYPE_BUBBLEGUM:
		sprintf(message, "Last Enhancement: BUBBLE GUM"); break;
	case TYPE_SPEEDBOOST:
		sprintf(message, "Last Enhancement: SPEED BOOST"); break;
	default:
		sprintf(message, "Last Opponent: NONE"); break;
	}
	ShowMessage(10, 10+4*FONT_HEIGHT, 600, 50, g_FontListID, message);
}

///////////////////////////////// MAIN LOOP \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function handles %CPU, Updating, and Rendering.
/////
///////////////////////////////// MAIN LOOP \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

WPARAM MainLoop()
{
	MSG msg; // Message Loop
	int RunDemo = 1; // Keep track of our demo in stages
	long update_clock = GetTickCount();
	long render_clock = GetTickCount();
	long t_elapsed;

	MIDI OurMidi;
	bool result = OurMidi.OpenMidi("data/melody.mid");
	if(result == false)
	{
		MessageBox(g_hWnd, "Could not open Midi Music. (FILE IS MISSING)",
					       "ERROR", MB_OK | MB_ICONERROR);
		RunDemo = 0;
	}
	else
	{
		OurMidi.PlayMidi(g_hWnd, false); // Start song without looping
	}
	
	InitGame();

	g_Camera.PositionObject(0, 3, -3,  0, 0, 0,  0, 1, 0);

	while(RunDemo == 1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			switch(msg.message)
			{
			case WM_QUIT:
				RunDemo = 0;
				break;
			case MM_MCINOTIFY:
				{
					switch(msg.wParam)
					{
					case MCI_NOTIFY_SUCCESSFUL:
						OurMidi.PlayMidi(g_hWnd, false);
						break;
					}
				}
				break;
			}
			TranslateMessage(&msg);
            DispatchMessage(&msg);
		}
		else
		{
			// Update thrice as often as rendering
			t_elapsed = labs(GetTickCount() - update_clock);
			if(t_elapsed > 10)
			{
				Update(); // Update logic for the game
				update_clock = GetTickCount(); // Update Clock
			}

			// Render every 30 msecs ~ 30 FPS			
			t_elapsed = labs(GetTickCount() - render_clock);
			if(t_elapsed > 30)
			{
				Render(); // Display the game
				SwapBuffers(g_hDC); // Swap the display buffers
				render_clock = GetTickCount(); // Update Clock
			}
			Sleep(1);
		}
	}

	OurMidi.CloseMidi();

	glDeleteLists(g_FontListID, MAX_CHARS);
	SelectObject(g_hDC, hOldFont);

	return WM_QUIT;
}

// Window handling code below

///////////////////////////////// WIN PROC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function handles escaping and quit messages
/////
///////////////////////////////// WIN PROC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

LRESULT CALLBACK WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE: case WM_QUIT:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		{
			switch(wParam)
			{
			case VK_ESCAPE:
				PostQuitMessage(0);
				break;
			default:
				if(!g_orange.m_State)
				{
					if( labs(GetTickCount() - g_orange.m_Modified) >= 2000 )
						InitGame();
				}
				break;
			}
		}
	}
	return DefWindowProc (hWnd, uMsg, wParam, lParam);
}


///////////////////////////////// CREATE WIN32 WINDOW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function creates a window, then maps calls to winproc
/////
///////////////////////////////// CREATE WIN32 WINDOW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

HWND CreateWin32Window(LPSTR strWindowName, int width, int height, DWORD dwStyle, bool bFullScreen, HINSTANCE hInstance)
{
	ShowCursor(TRUE); // Show the mouse

	HWND hWnd;
	char class_name[] = "72Hours";

	if(!bFullScreen)
	{
		WNDCLASSEX wndclassex;
		wndclassex.cbSize = sizeof(WNDCLASSEX);
		wndclassex.style = CS_HREDRAW | CS_VREDRAW;
		wndclassex.lpfnWndProc = WinProc;
		wndclassex.cbClsExtra = 0;
		wndclassex.cbWndExtra = 0;
		wndclassex.hInstance = hInstance;
		wndclassex.hIcon = NULL;
		wndclassex.hCursor = NULL;
		wndclassex.hCursor = (HCURSOR)LoadImage(hInstance, MAKEINTRESOURCE(IDC_POINTER),
			IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE); // Window sets shape of mouse
		wndclassex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		wndclassex.lpszMenuName = NULL;
		wndclassex.lpszClassName = class_name;
		wndclassex.hIconSm = NULL;

		//Support my window without the min/max stuff
		RegisterClassEx(&wndclassex);

		hWnd = CreateWindowEx(WS_EX_APPWINDOW, // This is a flag that can give our window different properties
							class_name,
							strWindowName,
							WS_OVERLAPPED | WS_SYSMENU,	// Window won't be resizable
							0,
							0,
							width,
							height,
							NULL,
							NULL,
							hInstance,
							NULL);
	}

	if(!hWnd)
	{
		UnregisterClass(class_name, hInstance);
		return NULL;
	}

	ShowWindow(hWnd, SW_SHOWNORMAL);					// Show the window
	UpdateWindow(hWnd);									// Draw the window

	SetFocus(hWnd);										// Sets Keyboard Focus To The Window	

	return hWnd;
}

bool bSetupPixelFormat(HDC hdc) 
{ 
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
    int pixelformat; 
 
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);			// Set the size of the structure
    pfd.nVersion = 1;									// Always set this to 1
														// Pass in the appropriate OpenGL flags
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; 
    pfd.dwLayerMask = PFD_MAIN_PLANE;					// We want the standard mask (this is ignored anyway)
    pfd.iPixelType = PFD_TYPE_RGBA;						// We want RGB and Alpha pixel type
    pfd.cColorBits = SCREEN_DEPTH;						// Here we use our #define for the color bits
    pfd.cDepthBits = SCREEN_DEPTH;						// Depthbits is ignored for RGBA, but we do it anyway
    pfd.cAccumBits = 0;									// No special bitplanes needed
    pfd.cStencilBits = 0;								// We desire no stencil bits
 
	// This gets us a pixel format that best matches the one passed in from the device
    if ( (pixelformat = ChoosePixelFormat(hdc, &pfd)) == FALSE ) 
    { 
        MessageBox(NULL, "ChoosePixelFormat failed", "Error", MB_OK); 
        return FALSE; 
    } 
 
	// This sets the pixel format that we extracted from above
    if (SetPixelFormat(hdc, pixelformat, &pfd) == FALSE) 
    { 
        MessageBox(NULL, "SetPixelFormat failed", "Error", MB_OK); 
        return FALSE; 
    } 
 
    return TRUE;
}

///////////////////////////////// WIN MAIN \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function makes the game window
/////
///////////////////////////////// WIN MAIN \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprev, PSTR cmdline, int ishow)
{	
	HWND hWnd;
	
	// Create our window with our function we create that passes in the:
	// Name, width, height, any flags for the window, if we want fullscreen of not, and the hInstance
	hWnd = CreateWin32Window("72H Game Dev Competition - Authors: Tim/Linae Graupmann", 
						  g_ScreenWidth, g_ScreenHeight, 0, g_bFullScreen, hInstance);

	// If we never got a valid window handle, quit the program
	if(hWnd == NULL) return TRUE;	

	// INIT OpenGL
	g_hWnd = hWnd;
	GetClientRect(g_hWnd, &g_rRect);

	g_hDC = GetDC(g_hWnd);
	if (!bSetupPixelFormat(g_hDC)) // Exit if pixel format isn't supported
		PostQuitMessage (0);

	g_hRC = wglCreateContext(g_hDC);
	wglMakeCurrent(g_hDC, g_hRC);

	// Load fonts
	g_FontListID = CreateOpenGLFont("arial", FONT_HEIGHT, 0);

	// Make our viewport to draw in
	glViewport(0, 0, g_ScreenWidth, g_ScreenHeight);

	// Run our message loop and after it's done, return the result
	return (int)MainLoop();
}
