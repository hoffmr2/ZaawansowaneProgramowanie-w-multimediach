#ifndef GL_PAINT_H_
#define GL_PAINT_H_
#include <Windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <glext.h>
#pragma comment (lib,"opengl32.lib")
#pragma comment (lib,"glu32.lib")
#define GL_GLEXT_PROTOTYPES



struct Textures
{
  static const GLuint count = 4;
  GLuint board;
  GLuint circle;
  GLuint cross;
  GLuint background;
  GLuint textures_id[count];

  Textures() { background = 0; board = 0; circle = 0; cross = 0; }
};

struct GlPaint
{
  HDC hDc;
  HGLRC hRc;
  Textures textures;

  GlPaint() { hRc = NULL; hDc = NULL;  }
};

struct Button
{
  int width;
  int height;
  int y_position;
  int x_position;
};

_declspec(selectany) GlPaint gl_game_paint;
_declspec(selectany) Button start_button;

_declspec(selectany) unsigned char* pMyImage;

// angle of rotation for the camera direction
_declspec(selectany) float board_rotate_angle = 0.0;
// actual vector representing the camera's direction
_declspec(selectany) float lx = 0.0f, lz = -1.0f;
// XZ position of the camera
_declspec(selectany) float x = 0.0f, z = 1.0f;
//Init 
void InitTextures();
void InitGlPaint(HWND hwnd);


//Draw Functions
GLint DrawGLScene();
void DrawCube1(int a, int b, int c);
void DrawCube(int xmin, int xmax, int ymin, int ymax, int zmin, int zmax, int r, int g, int b);
void DrawSide(int xmin, int xmax, int ymin, int ymax, int zmin, int zmax, int r, int g, int b);
void glDrawBoard();
void glDrawX(int a, int b, int c);
void glDrawO(int a, int b, int c);
void glDrawStartButton();

//Other
unsigned char* ReadBmpFromFile(char* szFileName, int &riWidth, int &riHeight);

#endif


