#include <Windows.h>
#include "res.h"
#include <stdio.h>
#include <vector>
#include <winuser.h>
#include "game_core.h"
#include "gl_paint.h"
#include <cassert>
#include <winuser.h>

#define GAME_RESULT_UNKNOWN "gra w trakcie"
#define GAME_RESULT_WINNER_CROSS "wygrąło krzyżyk"
#define GAME_RESULT_WINNER_CIRCLE "Wygrał Karakan"//"wygrtało kółko"
#define GAME_RESULT_DRAW "remis"
#define GAME_STATUS_PLAYING "gra się toczy"
#define GAME_STATUS_STOPED "gra nieaktywna"
#define RESTART_GAME "restart"
#define START_GAME "zacznij"
#define BUTTON_EMPTY 0
#define CIRCLE "O"
#define CROSS "X"


#define CHAR_CROSS 'X'
#define CHAR_CIRCLE 'O'
#define BOARD_SIZE  300
#define BOARD_POZITION_Y 70
#define BOARD_POZITION_X 60
#define GRID_SIZE 103
#define PEN_SIZE 10
#define BOARD_COLOR RGB(104, 63, 32)
#define CIRCLE_COLOR RGB(100, 150, 200)
#define CROSS_COLOR RGB(200, 150, 150)
#define BOARD_LINES_COLOR RGB(200, 200, 200)
#define BOARD_LINES_SIZE 5
#define APP_NAME "knur"

#define IDM_FILE_NEW 1
#define IDM_FILE_OPEN 2
#define IDM_FILE_QUIT 3

struct Bitmaps
{
  HBITMAP start;
  HBITMAP reset;
  HBITMAP cross;
  HBITMAP circle;
  HBITMAP board;
  SIZE board_size;
};


Bitmaps bitmaps;
CHAR sz_text[500];

std::vector<HWND> buttons;
HWND hwnd_radio_cross = nullptr;
HWND hwnd_radio_circle = nullptr;
HWND hwnd_static_game = nullptr;
HWND hwnd_static_result = nullptr;
HWND hwnd_button_start = nullptr;
HWND hwndMainWindow;
HWND hwndGL;
HWND hwnd_checkbox_against_computer = nullptr;

HINSTANCE h_instance;
WNDPROC wpOrgButtonProc;


void InitControlsHwnd(HWND hwndMainWindow);
LRESULT CALLBACK ButtonWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//void CheckGameResult(HWND b1, HWND b2, HWND b3, HWND b4, HWND b5, HWND b6, HWND b7, HWND b8, HWND b9);



void ClearButtons()
{
  for (const auto& button : buttons)
  {
    wsprintf(sz_text, "");
    SetWindowText(button, sz_text);
  }
}

void ChangeActivePlayer(HWND hwndDlg)
{
  if (is_player_cross == true)
  {
    CheckRadioButton(hwndDlg, IDC_RADIO_CROSS, IDC_RADIO_CIRCLE, IDC_RADIO_CIRCLE);
  }
  else
  {
    CheckRadioButton(hwndDlg, IDC_RADIO_CROSS, IDC_RADIO_CIRCLE, IDC_RADIO_CROSS);
  }
  ChangePlayer();

}

void SetCrossOrCircleText()
{
  if (is_player_cross)
  {
    wsprintf(sz_text, CROSS);
  }
  else
  {
    wsprintf(sz_text, CIRCLE);
  }
}

void InitGameControls(HWND hwndDlg)
{
  ChangeActivePlayer(hwndDlg);
  ClearButtons();
  wsprintf(sz_text, GAME_STATUS_PLAYING);
  SetWindowText(hwnd_static_game, sz_text);
  wsprintf(sz_text, GAME_RESULT_UNKNOWN);
  SetWindowText(hwnd_static_result, sz_text);
  // wsprintf(sz_text, RESTART_GAME);
  // SetWindowText(hwnd_button_start, sz_text);
  InitGameTable();
 // HDC hdc = GetDC(hwndDlg);
 // DrawEmptyBoard(hdc);
}

void CrossWon()
{
  wsprintf(sz_text, GAME_STATUS_STOPED);
  SetWindowText(hwnd_static_game, sz_text);
  wsprintf(sz_text, GAME_RESULT_WINNER_CROSS);
  SetWindowText(hwnd_static_result, sz_text);
  wsprintf(sz_text, RESTART_GAME);
  is_game_on = false;
  MessageBox(hwndMainWindow, GAME_RESULT_WINNER_CROSS, APP_NAME, MB_OK);
}

void CircleWon()
{
  wsprintf(sz_text, GAME_STATUS_STOPED);
  SetWindowText(hwnd_static_game, sz_text);
  wsprintf(sz_text, GAME_RESULT_WINNER_CIRCLE);
  SetWindowText(hwnd_static_result, sz_text);
  wsprintf(sz_text, RESTART_GAME);
  is_game_on = false;
  MessageBox(hwndMainWindow, GAME_RESULT_WINNER_CIRCLE, APP_NAME, MB_OK);
}


void Draw()
{


    wsprintf(sz_text, GAME_STATUS_STOPED);
    SetWindowText(hwnd_static_game, sz_text);
    wsprintf(sz_text, GAME_RESULT_DRAW);
    SetWindowText(hwnd_static_result, sz_text);
    wsprintf(sz_text, RESTART_GAME);
    is_game_on = false;
    MessageBox(hwndMainWindow,GAME_RESULT_DRAW, APP_NAME, MB_OK);
}

void UpdateResults()
{
  int result = CheckResults();

  switch(result)
  {
  case CIRCLE_WON:
    CircleWon();
    break;
  case CROSS_WON:
    CrossWon();
    break;
  case DRAW:
    Draw();
    break;
  case NO_RESULT:
    break;
  default:
    assert(true);
    break;
  }
   


}

bool IsMouseDownOnBoard(int x, int y)
{
  return (x > BOARD_POZITION_X && x < BOARD_POZITION_X + BOARD_SIZE) && (y > BOARD_POZITION_Y && y < BOARD_POZITION_Y + BOARD_SIZE);
}

void RepaintBoard(HDC hdc)
{
  for (int i = 0; i < TABLE_SIZE; ++i)
  {
    for (int j = 0; j < TABLE_SIZE; ++j)
    {
      if (game_table[i][j] == INT_CIRCLE);
//        DrawCircle(hdc, i, j);
      if (game_table[i][j] == INT_CROSS);
     //   DrawCross(hdc, i, j);
    }
  }
}

void InitBitmapHandlers()
{
  bitmaps.board = LoadBitmap(h_instance,MAKEINTRESOURCE(IDB_BOARD));
  bitmaps.circle = LoadBitmap(h_instance, MAKEINTRESOURCE(IDB_CIRCLE));
  bitmaps.cross = LoadBitmap(h_instance, MAKEINTRESOURCE(IDB_CROSS));
  bitmaps.start = LoadBitmap(h_instance, MAKEINTRESOURCE(IDB_START));
  bitmaps.reset = LoadBitmap(h_instance, MAKEINTRESOURCE(IDB_RESET));
  bitmaps.board_size.cx = 0;
  bitmaps.board_size.cy = 0;
}

void DeleteBitmaps()
{
  DeleteObject(bitmaps.board);
  DeleteObject(bitmaps.circle);
  DeleteObject(bitmaps.cross);
  DeleteObject(bitmaps.start);
  DeleteObject(bitmaps.reset);
}

void AddMenus(HWND hwnd) {

  HMENU hMenubar;
  HMENU hMenu;

  hMenubar = CreateMenu();
  hMenu = CreateMenu();

  AppendMenuW(hMenu, MF_STRING, IDM_FILE_NEW, L"&New");
  AppendMenuW(hMenu, MF_STRING, IDM_FILE_OPEN, L"&Open");
  AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
  AppendMenuW(hMenu, MF_STRING, IDM_FILE_QUIT, L"&Quit");

  AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hMenu, L"&File");
  SetMenu(hwnd, hMenubar);
}

LRESULT CALLBACK ButtonWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
  {
  case WM_INITDIALOG:
    {
      
    }
    return TRUE;

  case WM_SIZE:
    {
    GLint iWidth = LOWORD(lParam);
    GLint iHeight = HIWORD(lParam);

    if (iWidth == 0)iWidth = 1;
    if (iHeight == 0)iHeight = 1;


    glViewport(0, 0, iWidth, iHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //glLoadMatrix();

    gluPerspective(45.0f, iWidth / (float)iHeight, 0.1f, 1000.0f); //k¹t widzenia, Rozmiar okna, Z min, Z max

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    }
    return TRUE;
  case WM_PAINT:
    {
      PAINTSTRUCT ps;
      HDC hdc = BeginPaint(hwnd, &ps);
      //  auto hdc = GetDC(hwnd);

      auto hdc_bitmap = CreateCompatibleDC(hdc);
      HBITMAP bitmap;
      if (is_game_on == false)
      {
        bitmap = bitmaps.start;
      }
      else
      {
        bitmap = bitmaps.reset;
      }
      SelectObject(hdc_bitmap, bitmap);
      BITMAP board = {0};
      GetObject(bitmap, sizeof(board), &board);
      StretchBlt(hdc, 0, 0, 120, 50, hdc_bitmap, 0, 0, board.bmWidth, board.bmHeight, SRCCOPY);
      EndPaint(hwnd, &ps);
      DeleteDC(hdc_bitmap);
    }
    return 0;

  case WM_DRAWITEM:
    {
      PAINTSTRUCT ps;
      HDC hdc = BeginPaint(hwnd, &ps);
      //  auto hdc = GetDC(hwnd);
      BITMAP board = {0};
      GetObject(bitmaps.start, sizeof(board), &board);
      auto hdc_bitmap = CreateCompatibleDC(hdc);
      if (is_game_on == false)
        SelectObject(hdc_bitmap, bitmaps.start);
      else
        SelectObject(hdc_bitmap, bitmaps.reset);

      StretchBlt(hdc, 0, 0, 120, 50, hdc_bitmap, 0, 0, board.bmWidth, board.bmHeight, SRCCOPY);
      EndPaint(hwnd, &ps);
      DeleteDC(hdc_bitmap);
      ReleaseDC(hwnd, hdc);
    }
    return 0;


  default:
    {
      return CallWindowProc(wpOrgButtonProc, hwnd, uMsg, wParam, lParam);
      //return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
  }
}

INT_PTR CALLBACK DialogProcGL(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
  {
  case WM_INITDIALOG:
    {
      SetWindowLong(hwndDlg, GWL_STYLE, CS_HREDRAW | CS_VREDRAW | CS_OWNDC | WS_OVERLAPPEDWINDOW);
      InitGlPaint(hwndDlg);
    }
    return TRUE;
  case WM_SIZE:
    {
    GLint iWidth = LOWORD(lParam);
    GLint iHeight = HIWORD(lParam);

    if (iWidth == 0)iWidth = 1;
    if (iHeight == 0)iHeight = 1;


    glViewport(0, 0, iWidth, iHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //glLoadMatrix();

    gluPerspective(45.0f, iWidth / (float)iHeight, 0.1f, 1000.0f); //k¹t widzenia, Rozmiar okna, Z min, Z max

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    }
    return TRUE;
  }
  return FALSE;
}


INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
  {
  case WM_COMMAND:
    {
      switch (HIWORD(wParam))
      {
      case BN_CLICKED:
        {
          switch (LOWORD(wParam))
          {
          case IDC_BUTTON_START:
            {
              is_game_on = true;
              InvalidateRect(hwnd_button_start, NULL, TRUE);
              UpdateWindow(hwnd_button_start);
              if (has_cross_started == true && is_player_cross == true)
              {
                ChangeActivePlayer(hwndDlg);
                has_cross_started = false;
              }
              else
                has_cross_started = true;
              InitGameControls(hwndDlg);
              if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_AGAINST_COMPUTER) == BST_CHECKED && is_player_cross == false)
              {
                EmplaceByComputer();
                ChangeActivePlayer(hwndDlg);
                InvalidateRect(hwndDlg, NULL, TRUE);
                UpdateWindow(hwndDlg);
                UpdateResults();
              }
              UpdateWindow(hwndDlg);
              return TRUE;
            }
          }
          return TRUE;
        }
        return TRUE;
      }
      return TRUE;
    }
    return TRUE;
  case WM_MOUSEWHEEL:
    {
      auto zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
      board_rotate_angle += 0.05f*zDelta;
      lx = sin(board_rotate_angle);
     // lz = -cos(angle);
    }
    return TRUE;
  case WM_TIMER:
    {
      if(wParam == 2)
      {

        auto fStep = 5;
        int board_offset_x = -0;
        int board_offset_y = -0;
        DrawGLScene();
        glPushMatrix();
        glTranslatef(0, 0, 0);
        static float fangle = 0;
        fangle += fStep;
        glRotatef(board_rotate_angle, 0, 1, 0);
        glDrawBoard();
        glPopMatrix();


        for (int i = 0; i < TABLE_SIZE; ++i)
        {
          for (int j = 0; j < TABLE_SIZE; ++j)
          {
            int offset = 15;
            int start_x =  (i-1) * (GRID_SIZE/2 + offset / 2);
            int start_y = (-j+1) * (GRID_SIZE/2 + offset / 2);
            glPushMatrix();
            glTranslatef(0, 0, 0);
            glRotatef(board_rotate_angle, 0, 1, 0);
              glTranslatef(board_offset_x + start_x, board_offset_y + start_y, 0);
            glRotatef(fangle, 0, 1, 0);
            if (game_table[i][j] == INT_CIRCLE)
              glDrawO(-19, -20, 5);
            if (game_table[i][j] == INT_CROSS)
              glDrawX(-19, -20, 5);
            glPopMatrix();
            
          }
        }
        glLoadIdentity();
        SwapBuffers(gl_game_paint.hDc);

      }
    }
    return TRUE;
  case WM_INITDIALOG:
    {
      InitGameVars();
      InitControlsHwnd(hwndDlg);
 //   SetWindowLong(hwndDlg, GWL_STYLE, CS_HREDRAW | CS_VREDRAW | CS_OWNDC | WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME);
    //  InitBitmapHandlers();
  //   HWND hwndGL = CreateDialog(h_instance, MAKEINTRESOURCE(IDD_MAINVIEW), hwndDlg, DialogProc);

     // wpOrgButtonProc = (WNDPROC)SetWindowLongPtr(hwnd_button_start, -4, (LONG_PTR) ButtonWndProc);
    //  HWND hwnd_static3 = GetDlgItem(hwndMainWindow, IDC_STATIC3);
    SetWindowLong(hwndDlg, GWL_STYLE, CS_HREDRAW | CS_VREDRAW | CS_OWNDC | WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME);//
  //  SendMessage(hwnd_button_start, BM_SETSTYLE, BS_OWNERDRAW, TRUE);
    //   SendMessage(hwndDlg, WM_PAINT, NULL, NULL);
   //   InitGameControls(hwndMainWindow);
      InitGlPaint(hwndDlg);
      AddMenus(hwndDlg);
      is_game_on = true;

   

    
      //    SendMessage(GetDlgItem(hwndDlg, IDC_BUTTON_START), BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)bitmaps.start);
      // ClearButtons();
    }
    return TRUE;
  case WM_SIZE:
    {
    GLint iWidth = LOWORD(lParam);
    GLint iHeight = HIWORD(lParam);

    if (iWidth == 0)iWidth = 1;
    if (iHeight == 0)iHeight = 1;


    glViewport(0, 0, iWidth, iHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //glLoadMatrix();

    gluPerspective(45.0f, iWidth / (float)iHeight, 0.1f, 1000.0f); //k¹t widzenia, Rozmiar okna, Z min, Z max

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    }
    return TRUE;
  case WM_LBUTTONDOWN:
    {
      if (is_game_on)
      {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);

        if (IsMouseDownOnBoard(x, y))
        {
          int field_index_x = (x - BOARD_POZITION_X) / GRID_SIZE;
          int field_index_y = (y - BOARD_POZITION_Y) / GRID_SIZE;
          if (IsFieldNotFree(field_index_x,field_index_y))
            return TRUE;
       
          EmplaceByPlayer(field_index_x, field_index_y);
          InvalidateRect(hwndDlg, NULL, TRUE);
          UpdateWindow(hwndDlg);
          UpdateResults();
          ChangeActivePlayer(hwndDlg);
      

          if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_AGAINST_COMPUTER) == BST_CHECKED)
          {
            EmplaceByComputer();
            ChangeActivePlayer(hwndDlg);
            InvalidateRect(hwndDlg, NULL, TRUE);
            UpdateWindow(hwndDlg);
            UpdateResults();
          }
  
        }
      }
    }
    return TRUE;

  case WM_PAINT:
    {
      PAINTSTRUCT ps;
      HDC hdc = BeginPaint(hwndDlg, &ps);
      //HDC hdc = GetDC(hwndDlg);
      // BitBlt(hdc, 0, 0, 289, 290, GetDC(0), 0, 0, SRCCOPY);
      UpdateWindow(hwnd_button_start);
      RepaintBoard(hdc);

      EndPaint(hwndDlg, &ps);
      ReleaseDC(hwndDlg, hdc);

      //return DefWindowProc(hwndDlg, uMsg, wParam, lParam);
      return 0;
    }
    return TRUE;
  case WM_CLOSE:
    {
      DestroyWindow(hwndDlg); // zniszczenie okna
      DeleteBitmaps();
      PostQuitMessage(0); //Komunikat polecenia zakończenia aplikacji
      return TRUE;
    }
    return FALSE;
  }
  return FALSE;
}




void InitControlsHwnd(HWND hwndMainWindow)
{
  hwnd_checkbox_against_computer = GetDlgItem(hwndMainWindow, IDC_CHECK_AGAINST_COMPUTER);
  hwnd_static_game = GetDlgItem(hwndMainWindow, IDC_STATIC_GAME);
  hwnd_static_result = GetDlgItem(hwndMainWindow, IDC_STATIC_RESULT);
  hwnd_radio_circle = GetDlgItem(hwndMainWindow, IDC_RADIO_CIRCLE);
  hwnd_radio_cross = GetDlgItem(hwndMainWindow, IDC_RADIO_CROSS);
  hwnd_button_start = GetDlgItem(hwndMainWindow, IDC_BUTTON_START);
  buttons.push_back(GetDlgItem(hwndMainWindow, IDC_BUTTON1));
  buttons.push_back(GetDlgItem(hwndMainWindow, IDC_BUTTON2));
  buttons.push_back(GetDlgItem(hwndMainWindow, IDC_BUTTON3));
  buttons.push_back(GetDlgItem(hwndMainWindow, IDC_BUTTON4));
  buttons.push_back(GetDlgItem(hwndMainWindow, IDC_BUTTON5));
  buttons.push_back(GetDlgItem(hwndMainWindow, IDC_BUTTON6));
  buttons.push_back(GetDlgItem(hwndMainWindow, IDC_BUTTON7));
  buttons.push_back(GetDlgItem(hwndMainWindow, IDC_BUTTON8));
  buttons.push_back(GetDlgItem(hwndMainWindow, IDC_BUTTON9));
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
  h_instance = hInstance;


  hwndMainWindow = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MAINVIEW1), NULL, DialogProc);
  ShowWindow(hwndMainWindow, iCmdShow);


  MSG msg = {};
  SetTimer(hwndMainWindow, 2, 50, NULL);
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
}

/*void CheckGameResult(HWND b1, HWND b2, HWND b3, HWND b4, HWND b5, HWND b6, HWND b7, HWND b8, HWND b9)
{
  //int i_text_length = GetWindowTextLength(b1);

    if ((GetWindowTextLength(b1) == GetWindowTextLength(b2))&& (GetWindowTextLength(b1) == GetWindowTextLength(b3)) && (GetWindowTextLength(b1) !=0))
    {
      game_result = true;
      is_game_on = false;
    }
  
}
/*
int iTextLength = GetWindowTextLength(hwndEditBox);//Pobranie długości tekstu
          GetWindowText(hwndEditBox, szText, iTextLength + 1)
          */
