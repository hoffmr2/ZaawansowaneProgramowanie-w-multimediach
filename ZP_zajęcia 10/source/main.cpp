#include <Windows.h>
#include "res.h"
#include <winuser.h>
#include <cstdio>
#include <map>
#include "game_data.h"

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "advapi32.lib")
/*
 * gra w zgadywanie
 * komputer losuje cyfre z zakresu 1-40
 * wpisuje w text box
 * komputer odpowiada czy iwêksza czy mniejsza
 * 2 pola static box
 */

//TODO:
/*
 * przyspieszenie obliczen
 * przeczytac techniki i zrobic ze strobny
 * techniki sst AVX
 */
#define APP_NAME "Knur"
#define CONGRATULATIONS "Brawo zgad³eœ"
#define ATTEMPT_NUMBER "liczba prób %d"
#define IS_OK_DEFAULT "wpisz wartoœæ z zakresu 1 - 40"
#define IS_OK_TOLOW "za ma³oooo"
#define IS_OK_TOHIGH "za du¿oooo"
#define ERROR_MESSAGE "nastepnym razem wpisz poprawne dane :)"
#define MIN_VALUE 1
#define MAX_VALUE 40

#define ID_TIMER_REDRAW 10
HANDLE hFile= NULL;
BYTE* imageBufferY;
BYTE* imageBufferU;
BYTE* imageBufferV;

BYTE redLookupTable[256][256][256];
BYTE greenLookupTable[256][256][256];
BYTE blueLookupTable[256][256][256];
void InitLookupTables();

__int64* rgb_indexs;
void InitRGBIndexs();
__int64 iEnd = 0;
__int64 iStart = 0;

BYTE*imageBufferRGBA;
void InitControls(HWND hwnd_main_window);
BOOL Magic();

GameData* game_data;

void IncreaseAttemptsNumberText(HWND hwndDlg)
{
  game_data->IncreaseAttemptsNumber();
  auto hwnd_attempt_number = GetDlgItem(hwndDlg, IDC_STATIC_ATTEMPT_NUMBER);
  CHAR text[50];
  wsprintf(text, ATTEMPT_NUMBER, game_data->GetAttemptsNumber());
  SetWindowText(hwnd_attempt_number, text);
}

void UpdateTohighTolowText(HWND hwndDlg, int value)
{
  IncreaseAttemptsNumberText(hwndDlg);
  auto hwnd_tohigh_tolow = GetDlgItem(hwndDlg, IDC_STATIC_TOHIGH_TOLOW);
  CHAR text[50];
  if(value < game_data->GetNumberToGues())
    wsprintf(text, IS_OK_TOLOW);
  else
    wsprintf(text, IS_OK_TOHIGH);
  SetWindowText(hwnd_tohigh_tolow, text);

}

void GetNumberFromTextEdit(HWND hwnd_edit_box, int& value)
{
  INT text_length = GetWindowTextLength(hwnd_edit_box);
  CHAR text[500];
  GetWindowText(hwnd_edit_box, text, text_length + 1);
  value = atoi(text);

  if(value < MIN_VALUE || value > MAX_VALUE)
  {
    MessageBox(hwnd_edit_box, ERROR_MESSAGE, nullptr, MB_OK | MB_ICONERROR);
    Magic();
  }
 
}

INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
  {
  case WM_COMMAND:
    switch (HIWORD(wParam))
    {
    case BN_CLICKED:
      switch (LOWORD(wParam))
      {
      case IDC_BUTTON_START:
        game_data->ResetData();
        InitControls(hwndDlg);
        return TRUE;

      case IDC_BUTTON_CHECK:
        auto hwnd_edit_box = GetDlgItem(hwndDlg, IDC_EDIT1);
        int value;
        GetNumberFromTextEdit(hwnd_edit_box, value);

        if (game_data->Compare(value) == true)
        {
          MessageBox(hwndDlg, CONGRATULATIONS, nullptr, MB_OK);
          game_data->ResetData();
          InitControls(hwndDlg);
        }
        else
          UpdateTohighTolowText(hwndDlg, value);
        return TRUE;
      }



    }
    return FALSE;
  case WM_CLOSE:
    DestroyWindow(hwndDlg);
    PostQuitMessage(0);
    free(imageBufferU);
    free(imageBufferY);
    free(imageBufferV);
    free(imageBufferRGBA);
    CloseHandle(hFile);

    return TRUE;
  case WM_TIMER:
  {
    switch (wParam)
    {
    case ID_TIMER_REDRAW:
    {
      if (hFile ==NULL)break;

      static const int val_r = 256 * 1.40200;
      static const int val_g1 = 256 * -0.34414;
      static const int val_g2 = 256 * -0.71414;
      static const int val_b = 256 * 1.77200;

      DWORD ret;
      ReadFile(hFile, imageBufferY, 352 * 288, &ret, NULL);
      ReadFile(hFile, imageBufferU, 352 * 288/4, &ret, NULL);
      ReadFile(hFile, imageBufferV, 352 * 288/4, &ret, NULL);
    //   iStart = GetTickCount();
       QueryPerformanceCounter((LARGE_INTEGER*)&iStart);
      for(int y=0;y<288;++y)
        for(int x=0;x<352;++x)
        {
          int yy = imageBufferY[y * 352 + x];
          int uu = imageBufferU[(y >> 1) * 176 + (x >> 1)] - 128;
          int vv = imageBufferV[(y >> 1) * 176 + (x >> 1)] - 128;

          yy <<= 8;
          auto r = ((val_r)*vv +  yy) >> 8;
          int g = ((val_g1)*uu + (val_g2)*vv + yy) >> 8;
          int b = ((val_b)*uu + yy) >>8;

       //  int r = redLookupTable[yy][uu + 128][vv + 128];
       //  int g = greenLookupTable[yy][uu + 128][vv + 128];
      //   int b = blueLookupTable[yy][uu + 128][vv + 128];

          // kolory wpisywane w windowsie s¹ od koñca wiec ³adujemy BGR a nie RGB
          //ImageBufferRGBA[3*(y * 352 + x)+0] = b;
          //ImageBufferRGBA[3*(y * 352 + x)+1] = g;
          //ImageBufferRGBA[3*(y * 352 + x)+2] = r;
          //wpisywanie do bufora tak ¿eby nie by³o do góry nogami
          auto basic_index = 3 * ((287 - y) * 352 + x);
          imageBufferRGBA[basic_index] = b;
          imageBufferRGBA[basic_index + 1] = g;
          imageBufferRGBA[basic_index + 2] = r;
        }

      // iEnd = GetTickCount();
       QueryPerformanceCounter((LARGE_INTEGER*)&iEnd);
      RECT rc;
      GetClientRect(hwndDlg, &rc);
      InvalidateRect(hwndDlg, &rc, FALSE);
      RedrawWindow(hwndDlg, &rc, NULL, RDW_ERASE);
    }
    return TRUE;
    }
  }
  return TRUE;

  case WM_PAINT:
  {
    HDC hDC = GetDC(hwndDlg);
    HDC hdcMemDC;
    hdcMemDC = CreateCompatibleDC(hDC);
    //wczytanie
    HBITMAP hBmp;
    hBmp = CreateCompatibleBitmap(hDC, 352, 288);
    SelectObject(hdcMemDC, hBmp);

    BITMAP bmp;
    GetObject(hBmp, sizeof(BITMAP), &bmp);
    BITMAPINFOHEADER bi;
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = 352;
    bi.biHeight = 288;
    bi.biPlanes = 1;
    bi.biBitCount = 24;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    SetDIBits(hdcMemDC, hBmp, 0, 352*288, imageBufferRGBA,(BITMAPINFO*) &bi, DIB_RGB_COLORS);
    BitBlt(hDC, 0, 0, 352, 288, hdcMemDC, 0, 0, SRCCOPY);
   // static int x = 0;
   // x += 1;
    //x %= 100;
    //if (x == 99)
      //return DefWindowProc(hwndDlg, uMsg, wParam, lParam);
    char text[200];
    auto ans = iEnd - iStart;
    sprintf_s(text, "czas wykonania petli %i", ans);
    TextOut(hDC, 0, 100, text, 50);
    ReleaseDC(hwndDlg, hDC);
  }
  return DefWindowProc(hwndDlg, uMsg, wParam, lParam);

  case WM_INITDIALOG:
  {
     hFile = CreateFile("../res/claire.cif",
      GENERIC_READ,
      0,
      (LPSECURITY_ATTRIBUTES)0,
      OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);
     imageBufferY = (BYTE*)malloc(352 * 288);
     imageBufferU = (BYTE*)malloc(352 * 288/4);
     imageBufferV = (BYTE*)malloc(352 * 288/4);
     rgb_indexs = (__int64*)malloc(352 * 288);

     imageBufferRGBA = (BYTE*)malloc(352 * 288 * 4);
     InitLookupTables();
     InitRGBIndexs();

  }
  return TRUE;
  }
  return FALSE;
}


void InitAttemptNumberText(HWND hwnd_main_window)
{
  auto hwnd_attempt_number = GetDlgItem(hwnd_main_window, IDC_STATIC_ATTEMPT_NUMBER);
  CHAR text[50];
  wsprintf(text, ATTEMPT_NUMBER, 0);
  SetWindowText(hwnd_attempt_number, text);
}

void InitTohighTolowText(HWND hwnd_main_window)
{
  auto hwnd_tohigh_tolow = GetDlgItem(hwnd_main_window, IDC_STATIC_TOHIGH_TOLOW);
  CHAR text[50];
  wsprintf(text, IS_OK_DEFAULT);
  SetWindowText(hwnd_tohigh_tolow, text);
}

void InitEditText(HWND hwnd_main_window)
{
  auto hwnd_edit = GetDlgItem(hwnd_main_window, IDC_EDIT1);
  SetWindowText(hwnd_edit, nullptr);
}

void InitLookupTables()
{
  int size =256;
  for(auto y=0;y<size;++y)
    for (auto u = -size/2; u<size/2; ++u)
      for (auto v = -size/2; v<size/2; ++v)
      {
        redLookupTable[y][u + size / 2][v + size / 2] = ((256 * 1.40200)*v + 256 * y) / 256 ;
        greenLookupTable[y][u+size/2][v+size/2] = ((256 * -0.34414)*u + (256 * -0.71414)*v + 256 * y) / 256 ;
        blueLookupTable[y][u+size/2][v+size/2] = ((256 * 1.77200)*u + 256 * y) / 256 ;
      }
}

void InitRGBIndexs()
{

}

void InitControls(HWND hwnd_main_window)
{
  InitAttemptNumberText(hwnd_main_window);
  InitTohighTolowText(hwnd_main_window);
  InitEditText(hwnd_main_window);
}




int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
  game_data = new GameData();

  HWND hwnd_main_window = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MAINVIEW), NULL, DialogProc);
  ShowWindow(hwnd_main_window, iCmdShow);
  SetTimer(hwnd_main_window, ID_TIMER_REDRAW, 100, NULL);
  InitControls(hwnd_main_window);

  //pêtla obs³ugi komunikatów

  MSG msg = {};

  while(GetMessage(&msg,NULL,0,0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
 
  return 0;
}

BOOL Magic()
{
  
  return TRUE;
}
