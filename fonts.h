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

#ifndef _FONTS_H
#define _FONTS_H

#include <windows.h> // Needed for WIN32 Information
#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glaux.h>
#include <math.h>
#include <stdio.h>

#define MAX_CHARS	256
#define FONT_HEIGHT	16

///////////////////////////////// CREATE OPENGL FONT \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function creates a windows font like "Arial" and returns a display list ID
/////
///////////////////////////////// CREATE OPENGL FONT \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

UINT CreateOpenGLFont(LPSTR strFontName, int height, UINT ID);

///////////////////////////////// POSITION TEXT \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function sets the drawing position for the text
/////
///////////////////////////////// POSITION TEXT \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void PositionText(int x, int y, UINT Size);


///////////////////////////////// GL DRAW TEXT \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This draws 2D text onto the screen using OpenGL, given an X and Y position 
/////
///////////////////////////////// GL DRAW TEXT \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void glDrawText(int x, int y, UINT ID, UINT Size, const char *strString, ...);


///////////////////////////////// DESTROY FONT \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function frees our display lists
/////
///////////////////////////////// DESTROY FONT \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void DestroyFont();


#endif