#include "gl_paint.h"
#include <cstdio>


void BindBoardTexture(int iWidth, int iHeight)
{
  pMyImage = ReadBmpFromFile("..\\res\\tekstura_planszy.bmp", iWidth, iHeight);

  glActiveTexture(GL_TEXTURE);
  glBindTexture(GL_TEXTURE_2D, gl_game_paint.textures.textures_id[0]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iWidth, iHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, pMyImage);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void InitTextures()
{

  int iWidth=0;
  int iHeight=0;
  //texture board
  glGenTextures(gl_game_paint.textures.count, gl_game_paint.textures.textures_id);
   BindBoardTexture(iWidth, iHeight);
   pMyImage = ReadBmpFromFile("..\\res\\tekstura_circle.bmp", iWidth, iHeight);
 // glGenTextures(1, &gl_game_paint.textures.circle);
  glActiveTexture(GL_TEXTURE+1);
  glBindTexture(GL_TEXTURE_2D, gl_game_paint.textures.textures_id[1]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,iWidth, iHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, pMyImage);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  pMyImage = ReadBmpFromFile("..\\res\\tekstura_cross.bmp", iWidth, iHeight);
  // glGenTextures(1, &gl_game_paint.textures.circle);
  glActiveTexture(GL_TEXTURE + 2);
  glBindTexture(GL_TEXTURE_2D, gl_game_paint.textures.textures_id[2]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iWidth, iHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, pMyImage);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  delete[] pMyImage;
}

void InitGlPaint(HWND hwnd)
{
  //Stworzyæ maszynê stanu
  static PIXELFORMATDESCRIPTOR pfd = {
    sizeof(PIXELFORMATDESCRIPTOR), //rozmiar
    1,                            //wersja
    PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, //flagi
    PFD_TYPE_RGBA,//typ pixeli
    16,//
    0,0,0,0,0,0,
    0,
    0,
    0,
    0,0,0,0,
    16,
    0,
    0,
    PFD_MAIN_PLANE,
    0,
    0,0,0
  };
  //zmienne pomocnicze
  GLuint PixelFormat;

  gl_game_paint.hDc = GetDC(hwnd);
  PixelFormat = ChoosePixelFormat(gl_game_paint.hDc, &pfd); //sprawdzamy czy nasz format graficzny jest wspierany
                                              //ustawiamy format
  SetPixelFormat(gl_game_paint.hDc, PixelFormat, &pfd);
  //tworzymy maszynê stanu
  gl_game_paint.hRc = wglCreateContext(gl_game_paint.hDc);
  wglMakeCurrent(gl_game_paint.hDc, gl_game_paint.hRc);
  InitTextures();


}




GLint DrawGLScene()
{
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_TEXTURE_2D);

  glLoadIdentity();

  glTranslatef(0, 0, -300);
  glRotatef(0, 0, 0, 0);



  return 1;
}

void DrawCube(int xmin, int xmax, int ymin, int ymax, int zmin, int zmax, int r, int g, int b)
{
 // glColor3d(r, g, b);
  glNormal3d(0, -1, 0);
  glTexCoord2f(0.0, 0.0); glVertex3d(xmin, ymin, zmin);
  glTexCoord2f(0.0, 1.0); glVertex3d(xmin, ymin, zmax);
  glTexCoord2f(1.0, 1.0); glVertex3d(xmax, ymin, zmax);
  glTexCoord2f(1.0, 0.0); glVertex3d(xmax, ymin, zmax);

  glNormal3d(0, -1, 0);
  glTexCoord2f(0.0, 0.0); glVertex3d(xmax, ymin, zmin);
  glTexCoord2f(0.0, 1.0); glVertex3d(xmax, ymin, zmax);
  glTexCoord2f(1.0, 1.0); glVertex3d(xmax, ymax, zmax);
  glTexCoord2f(1.0, 0.0); glVertex3d(xmin, ymax, zmin);

  glNormal3d(0, 1, 0);
  glTexCoord2f(0.0, 0.0); glVertex3d(xmax, ymax, zmin);
  glTexCoord2f(0.0, 1.0); glVertex3d(xmax, ymax, zmax);
  glTexCoord2f(1.0, 1.0); glVertex3d(xmin, ymax, zmax);
  glTexCoord2f(1.0, 0.0); glVertex3d(xmin, ymax, zmin);

  glNormal3d(-1, 0, 0);
  glTexCoord2f(0.0, 0.0); glVertex3d(xmin, ymax, zmin);
  glTexCoord2f(0.0, 1.0); glVertex3d(xmin, ymax, zmax);
  glTexCoord2f(1.0, 1.0); glVertex3d(xmin, ymin, zmax);
  glTexCoord2f(1.0, 0.0); glVertex3d(xmin, ymin, zmin);

  glNormal3d(0, 0, 1);
  glTexCoord2f(0.0, 0.0); glVertex3d(xmin, ymin, zmax);
  glTexCoord2f(0.0, 1.0); glVertex3d(xmin, ymax, zmax);
  glTexCoord2f(1.0, 1.0); glVertex3d(xmax, ymax, zmax);
  glTexCoord2f(1.0, 0.0); glVertex3d(xmax, ymin, zmax);

  glNormal3d(0, 0, -1);
  glTexCoord2f(0.0, 0.0); glVertex3d(xmin, ymin, zmin);
  glTexCoord2f(0.0, 1.0); glVertex3d(xmax, ymin, zmin);
  glTexCoord2f(1.0, 1.0); glVertex3d(xmax, ymax, zmin);
  glTexCoord2f(1.0, 0.0); glVertex3d(xmin, ymax, zmin);

}

void DrawSide(int xmin, int xmax, int ymin, int ymax, int zmin, int zmax, int r, int g, int b)
{
 // glColor3d(r, g, b);


  glVertex3d(xmin, ymin, zmax);
  glVertex3d(xmax, ymax, zmax);
  glVertex3d(xmax, ymax, zmin);

  glVertex3d(xmax, ymax, zmin);
  glVertex3d(xmin, ymin, zmin);
  glVertex3d(xmin, ymin, zmax);
}

void glDrawBoard()
{
  glActiveTexture(GL_TEXTURE);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, gl_game_paint.textures.textures_id[0]);
 // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 100, 200, 0, GL_RGB, GL_UNSIGNED_BYTE, pMyImage);
  //glColor3d(55, 30, 200);
  glBegin(GL_QUADS);
  DrawCube(-85, 85, -35, -25, -10, 0, 1, 1, 1);
  DrawCube(-85, 85, 25, 35, -10, 0, 1, 1, 1);
  DrawCube(-35, -25, -85, 85, -10, 0, 1, 1, 1);
  DrawCube(25, 35, -85, 85, -10, 0, 1, 1, 1);
  glEnd();
}


void glDrawX(int a, int b, int c)
{
  glActiveTexture(GL_TEXTURE);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, gl_game_paint.textures.textures_id[2]);
 
  glBegin(GL_TRIANGLES);
 // a = b = c = 0;


  //front
  {
   // glColor3d(1, 0, 1);
    glNormal3d(1, 0, 0);
    glVertex3d(a + 5, b + 5, c - 0);
    glVertex3d(a + 15, b + 5, c - 0);
    glVertex3d(a + 38, b + 41, c - 0);//

    glVertex3d(a + 38, b + 41, c - 0);
    glVertex3d(a + 28, b + 41, c - 0);
    glVertex3d(a + 5, b + 5, c - 0);//

    glVertex3d(a + 5, b + 41, c - 0);
    glVertex3d(a + 15, b + 41, c - 0);
    glVertex3d(a + 38, b + 5, c - 0);//

    glVertex3d(a + 38, b + 5, c - 0);
    glVertex3d(a + 28, b + 5, c - 0);
    glVertex3d(a + 5, b + 41, c - 0);
  }

  //ty³
  {
   // glColor3d(1, 1, 0);
    glNormal3d(-1, 0, 0);
    glVertex3d(a + 5, b + 5, c - 10);
    glVertex3d(a + 15, b + 5, c - 10);
    glVertex3d(a + 38, b + 41, c - 10);//

    glVertex3d(a + 38, b + 41, c - 10);
    glVertex3d(a + 28, b + 41, c - 10);
    glVertex3d(a + 5, b + 5, c - 10);//

    glVertex3d(a + 5, b + 41, c - 10);
    glVertex3d(a + 15, b + 41, c - 10);
    glVertex3d(a + 38, b + 5, c - 10);//

    glVertex3d(a + 38, b + 5, c - 10);
    glVertex3d(a + 28, b + 5, c - 10);
    glVertex3d(a + 5, b + 41, c - 10);
  }

  //dó³
  {
  //  glColor3d(0, 0, 1);

    //lewy dol
    glNormal3d(0, -1, 0);
    glVertex3d(a + 5, b + 5, c - 0);
    glVertex3d(a + 15, b + 5, c - 0);
    glVertex3d(a + 5, b + 5, c - 10);

    glVertex3d(a + 15, b + 5, c - 10);
    glVertex3d(a + 5, b + 5, c - 10);
    glVertex3d(a + 15, b + 5, c - 0);

    //lewy gora
    glNormal3d(0, 1, 0);
    glVertex3d(a + 5, b + 41, c - 0);
    glVertex3d(a + 15, b + 41, c - 0);
    glVertex3d(a + 5, b + 41, c - 10);


    glVertex3d(a + 15, b + 41, c - 10);
    glVertex3d(a + 5, b + 41, c - 10);
    glVertex3d(a + 15, b + 41, c - 0);
  }
  //góra
  {
  //  glColor3d(0, 0, 1);

    //prawy dol
    glNormal3d(0, -1, 0);
    glVertex3d(a + 28, b + 5, c - 0);
    glVertex3d(a + 38, b + 5, c - 0);
    glVertex3d(a + 28, b + 5, c - 10);

    glVertex3d(a + 38, b + 5, c - 10);
    glVertex3d(a + 28, b + 5, c - 10);
    glVertex3d(a + 38, b + 5, c - 0);

    //prawy gora
    glNormal3d(0, 1, 0);
    glVertex3d(a + 28, b + 41, c - 0);
    glVertex3d(a + 38, b + 41, c - 0);
    glVertex3d(a + 28, b + 41, c - 10);


    glVertex3d(a + 38, b + 41, c - 10);
    glVertex3d(a + 28, b + 41, c - 10);
    glVertex3d(a + 38, b + 41, c - 0);
  }
  //boki
  {
   // glColor3d(0, 1, 1);

    //1
    glNormal3d(-1, 1, 0);
    glVertex3d(a + 5, b + 5, c - 0);
    glVertex3d(a + 28, b + 41, c - 0);
    glVertex3d(a + 5, b + 5, c - 10);//

    glVertex3d(a + 28, b + 41, c - 0);
    glVertex3d(a + 28, b + 41, c - 10);
    glVertex3d(a + 5, b + 5, c - 10);//

                                     //2
    glNormal3d(1, -1, 0);
    glVertex3d(a + 15, b + 5, c - 0);
    glVertex3d(a + 38, b + 41, c - 0);
    glVertex3d(a + 15, b + 5, c - 10);//

    glVertex3d(a + 38, b + 41, c - 0);
    glVertex3d(a + 38, b + 41, c - 10);
    glVertex3d(a + 15, b + 5, c - 10);//

                                      //3
    glVertex3d(a + 5, b + 41, c - 0);
    glVertex3d(a + 28, b + 5, c - 0);
    glVertex3d(a + 5, b + 41, c - 10);//

    glVertex3d(a + 28, b + 5, c - 0);
    glVertex3d(a + 28, b + 5, c - 10);
    glVertex3d(a + 5, b + 41, c - 10);//

                                      //4
    glVertex3d(a + 15, b + 41, c - 0);
    glVertex3d(a + 38, b + 5, c - 0);
    glVertex3d(a + 15, b + 41, c - 10);//

    glVertex3d(a + 38, b + 5, c - 0);
    glVertex3d(a + 38, b + 5, c - 10);
    glVertex3d(a + 15, b + 41, c - 10);//


  }
  glEnd();
}


void glDrawStartButton()
{
 
  glBegin(GL_TRIANGLES);
  glColor3d(0, 1, 0.1);
  glVertex3d( 0,  0,  0);
  glVertex3d( 0, 20,  0);
  glVertex3d( 60, 0,  0);//

  glVertex3d(60, 0, 0);
  glVertex3d(60, 20, 0);
  glVertex3d(0, 20, 0); 

  glEnd();

}

unsigned char* ReadBmpFromFile(char* szFileName, int& riWidth, int& riHeight)
{
  BITMAPFILEHEADER     bfh;
  BITMAPINFOHEADER     bih;

  int                i, j, h, v, lev, l, ls;
  unsigned char*     buff = NULL;

  unsigned char* p_palette = NULL;
  unsigned short n_colors = 0;

  unsigned char* pRGBBuffer = nullptr;

  FILE* hfile = nullptr;
  fopen_s(&hfile, szFileName, "rb");

  if (hfile != NULL)
  {
    fread(&bfh, sizeof(bfh), 1, hfile);
    if (!(bfh.bfType != 0x4d42 || (bfh.bfOffBits < (sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)))))
    {
      fread(&bih, sizeof(bih), 1, hfile);
      v = bih.biWidth;
      h = bih.biHeight;
      lev = bih.biBitCount;

      riWidth = v;
      riHeight = h;
      pRGBBuffer = new unsigned char[riWidth*riHeight * 3]; //Zaalokowanie odpowiedniego buffora obrazu

                                                            //Za³aduj Palete barw jesli jest
      if ((lev == 1) || (lev == 4) || (lev == 8))
      {
        n_colors = 1 << lev;
        p_palette = new unsigned char[4 * n_colors];
        fread(p_palette, 4 * n_colors, 1, hfile);
      }

      fseek(hfile, bfh.bfOffBits, SEEK_SET);

      buff = new unsigned char[v * 4];

      switch (lev)
      {
      case 1:
        //Nie obs³ugiwane
        break;
      case 4:
        //nie Obs³ugiwane
        break;
      case 8: //Skala szaroœci
        ls = (v + 3) & 0xFFFFFFFC;
        for (j = (h - 1); j >= 0; j--)
        {
          fread(buff, ls, 1, hfile);
          for (i = 0, l = 0; i<v; i++)
          {
            pRGBBuffer[((j*riWidth) + i) * 3 + 2] = p_palette[(buff[i] << 2) + 2];//R
            pRGBBuffer[((j*riWidth) + i) * 3 + 1] = p_palette[(buff[i] << 2) + 1];//G
            pRGBBuffer[((j*riWidth) + i) * 3 + 0] = p_palette[(buff[i] << 2) + 0];//B
          }
        };
        break;
      case 24:
        //bitmapa RGB
        ls = (v * 3 + 3) & 0xFFFFFFFC;
        for (j = (h - 1); j >= 0; j--)
        {
          //x_fread(hfile,buff,ls);
          fread(buff, ls, 1, hfile);
          for (i = 0, l = 0; i<v; i++, l += 3)
          {
            pRGBBuffer[((j*riWidth) + i) * 3 + 0] = buff[l + 2];
            pRGBBuffer[((j*riWidth) + i) * 3 + 1] = buff[l + 1];
            pRGBBuffer[((j*riWidth) + i) * 3 + 2] = buff[l + 0];
          };
        };
        break;
      case 32:
        // RGBA bitmap 
        for (j = (h - 1); j >= 0; j--)
        {
          fread(buff, v * 4, 1, hfile);
          for (i = 0, l = 0; i<v; i++, l += 4)
          {
            pRGBBuffer[((j*riWidth) + i) * 3 + 0] = buff[l + 2];
            pRGBBuffer[((j*riWidth) + i) * 3 + 1] = buff[l + 1];
            pRGBBuffer[((j*riWidth) + i) * 3 + 2] = buff[l + 0];
          }
        };
        break;
      };
      delete buff;
      if (p_palette) delete p_palette;

    }
  }
  return pRGBBuffer;
}


void glDrawO(int a, int b, int c)
{
  glActiveTexture(GL_TEXTURE+1);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, gl_game_paint.textures.textures_id[1]);
  glBegin(GL_TRIANGLES);
  //przód
  {
   // glColor3d(1, 1, 0);

    //lewy
    glVertex3d(a + 5, b + 16, c + 0);
    glVertex3d(a + 16, b + 16, c + 0);
    glVertex3d(a + 5, b + 27, c + 0);//

    glVertex3d(a + 16, b + 27, c + 0);
    glVertex3d(a + 16, b + 16, c + 0);
    glVertex3d(a + 5, b + 27, c + 0);//

                                     //skos
    glVertex3d(a + 5, b + 27, c + 0);
    glVertex3d(a + 16, b + 27, c + 0);
    glVertex3d(a + 16, b + 38, c + 0);//

                                      //gora
    glVertex3d(a + 16, b + 27, c + 0);
    glVertex3d(a + 16, b + 38, c + 0);
    glVertex3d(a + 27, b + 27, c + 0);//

    glVertex3d(a + 16, b + 38, c + 0);
    glVertex3d(a + 27, b + 38, c + 0);
    glVertex3d(a + 27, b + 27, c + 0);//

                                      //skos
    glVertex3d(a + 27, b + 27, c + 0);
    glVertex3d(a + 27, b + 38, c + 0);
    glVertex3d(a + 38, b + 27, c + 0);//

                                      //prawy
    glVertex3d(a + 27, b + 16, c + 0);
    glVertex3d(a + 38, b + 16, c + 0);
    glVertex3d(a + 27, b + 27, c + 0);//

    glVertex3d(a + 38, b + 27, c + 0);
    glVertex3d(a + 38, b + 16, c + 0);
    glVertex3d(a + 27, b + 27, c + 0);//

                                      //skos
    glVertex3d(a + 27, b + 5, c + 0);
    glVertex3d(a + 27, b + 16, c + 0);
    glVertex3d(a + 38, b + 16, c + 0);//

                                      //dol
    glVertex3d(a + 16, b + 5, c + 0);
    glVertex3d(a + 16, b + 16, c + 0);
    glVertex3d(a + 27, b + 5, c + 0);//

    glVertex3d(a + 16, b + 16, c + 0);
    glVertex3d(a + 27, b + 16, c + 0);
    glVertex3d(a + 27, b + 5, c + 0);//

                                     //skos
    glVertex3d(a + 5, b + 16, c + 0);
    glVertex3d(a + 16, b + 16, c + 0);
    glVertex3d(a + 16, b + 5, c + 0);//
  }
  //ty³
  {
   // glColor3d(0, 1, 1);

    //lewy
    glVertex3d(a + 5, b + 16, c - 10);
    glVertex3d(a + 16, b + 16, c - 10);
    glVertex3d(a + 5, b + 27, c - 10);//

    glVertex3d(a + 16, b + 27, c - 10);
    glVertex3d(a + 16, b + 16, c - 10);
    glVertex3d(a + 5, b + 27, c - 10);//

                                      //skos
    glVertex3d(a + 5, b + 27, c - 10);
    glVertex3d(a + 16, b + 27, c - 10);
    glVertex3d(a + 16, b + 38, c - 10);//

                                       //gora
    glVertex3d(a + 16, b + 27, c - 10);
    glVertex3d(a + 16, b + 38, c - 10);
    glVertex3d(a + 27, b + 27, c - 10);//

    glVertex3d(a + 16, b + 38, c - 10);
    glVertex3d(a + 27, b + 38, c - 10);
    glVertex3d(a + 27, b + 27, c - 10);//

                                       //skos
    glVertex3d(a + 27, b + 27, c - 10);
    glVertex3d(a + 27, b + 38, c - 10);
    glVertex3d(a + 38, b + 27, c - 10);//

                                       //prawy
    glVertex3d(a + 27, b + 16, c - 10);
    glVertex3d(a + 38, b + 16, c - 10);
    glVertex3d(a + 27, b + 27, c - 10);//

    glVertex3d(a + 38, b + 27, c - 10);
    glVertex3d(a + 38, b + 16, c - 10);
    glVertex3d(a + 27, b + 27, c - 10);//

                                       //skos
    glVertex3d(a + 27, b + 5, c - 10);
    glVertex3d(a + 27, b + 16, c - 10);
    glVertex3d(a + 38, b + 16, c - 10);//

                                       //dol
    glVertex3d(a + 16, b + 5, c - 10);
    glVertex3d(a + 16, b + 16, c - 10);
    glVertex3d(a + 27, b + 5, c - 10);//

    glVertex3d(a + 16, b + 16, c - 10);
    glVertex3d(a + 27, b + 16, c - 10);
    glVertex3d(a + 27, b + 5, c - 10);//

                                      //skos
    glVertex3d(a + 5, b + 16, c - 10);
    glVertex3d(a + 16, b + 16, c - 10);
    glVertex3d(a + 16, b + 5, c - 10);//
  }
  //boki
  {
    //lewy
    DrawSide(a + 5, a + 5, b + 16, b + 27, c - 10, c, 0, 1, 0);
    //skos
    DrawSide(a + 5, a + 16, b + 27, b + 38, c - 10, c, 0, 1, 0);
    //gora
    DrawSide(a + 16, a + 27, b + 38, b + 38, c - 10, c, 0, 1, 0);
    //skos
    DrawSide(a + 38, a + 27, b + 27, b + 38, c - 10, c, 0, 1, 0);
    //prawy
    DrawSide(a + 38, a + 38, b + 16, b + 27, c - 10, c, 0, 1, 0);
    //skos
    DrawSide(a + 27, a + 38, b + 5, b + 16, c - 10, c, 0, 1, 0);
    //dol
    DrawSide(a + 16, a + 27, b + 5, b + 5, c - 10, c, 0, 1, 0);
    //skos
    DrawSide(a + 5, a + 16, b + 16, b + 5, c - 10, c, 0, 1, 0);
    //srodek lewy
    DrawSide(a + 16, a + 16, b + 16, b + 27, c - 10, c, 0, 1, 0);
    //srodek góra
    DrawSide(a + 16, a + 27, b + 27, b + 27, c - 10, c, 0, 1, 0);
    //srodek prawy
    DrawSide(a + 27, a + 27, b + 16, b + 27, c - 10, c, 0, 1, 0);
    //srodek dol
    DrawSide(a + 16, a + 27, b + 16, b + 16, c - 10, c, 0, 1, 0);
  }
  glEnd();
}
