#include <Windows.h>
#include "res.h"
#include <stdio.h>
#include <vector>
#include <winuser.h>
#include "game_core.h"
#include "gl_paint.h"
#include <cassert>
#include "note_player.h"
#include <winuser.h>
#include <thread>

#define GAME_RESULT_UNKNOWN "gra w trakcie"
#define GAME_RESULT_WINNER_CROSS "wygrąło krzyżyk"
#define GAME_RESULT_WINNER_CIRCLE "Wygrał Karakan"//"wygrtało kółko"
#define GAME_RESULT_DRAW "remis"



#define BOARD_SIZE  300
#define BOARD_POZITION_Y 70
#define BOARD_POZITION_X 60
#define GRID_SIZE 103


#define APP_NAME "knur"
#define REPAINT_TIMER 2
#define ROTATE_TIMER 3
#define IDM_NEW_GAME_VS_COMPUTER 1
#define IDM_NEW_GAME_VS_HUMAN 2
#define IDM_FILE_QUIT 3



struct LastPlaced
{
  int x;
  int y;
};
LastPlaced last_placed;

CHAR sz_text[500];

HWND hwndMainWindow;
HWND hwndGL;
HWND hwnd_checkbox_against_computer = nullptr;

HINSTANCE h_instance;
WNDPROC wpOrgButtonProc;
NotePlayer* note_player;
GLfloat active_figure_angle = 0;




void ChangeActivePlayer(HWND hwndDlg)
{
  ChangePlayer();
}


void InitGameControls(HWND hwndDlg)
{
  ChangeActivePlayer(hwndDlg);
  InitGameTable();
}

void CrossWon()
{
  is_game_on = false;
  MessageBox(hwndMainWindow, GAME_RESULT_WINNER_CROSS, APP_NAME, MB_OK);
}

void CircleWon()
{
  is_game_on = false;
  MessageBox(hwndMainWindow, GAME_RESULT_WINNER_CIRCLE, APP_NAME, MB_OK);
}


void Draw()
{
    is_game_on = false;
    MessageBox(hwndMainWindow,GAME_RESULT_DRAW, APP_NAME, MB_OK);
}

void UpdateResults()
{
  int result = CheckResults();
  if(result != NO_RESULT)
  {
    is_game_vs_computer = false;
  }
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



void AddMenus(HWND hwnd) {

  HMENU hMenubar;
  HMENU hMenu;

  hMenubar = CreateMenu();
  hMenu = CreateMenu();

  AppendMenuW(hMenu, MF_STRING, IDM_NEW_GAME_VS_COMPUTER, L"&vs Computer");
  AppendMenuW(hMenu, MF_STRING, IDM_NEW_GAME_VS_HUMAN, L"&vs Other Player");
  AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);

  AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hMenu, L"&New Game");
  SetMenu(hwnd, hMenubar);
}




void DrawFigures(int table[TABLE_SIZE][TABLE_SIZE],float& fangle)
{
  for (int i = 0; i < TABLE_SIZE; ++i)
  {
    for (int j = 0; j < TABLE_SIZE; ++j)
    {
      int offset = 15;
      int start_x = (i - 1) * (GRID_SIZE / 2 + offset / 2);
      int start_y = (-j + 1) * (GRID_SIZE / 2 + offset / 2);
      glPushMatrix();
      glTranslatef(0, 0, 0);
      glRotatef(board_rotate_angle, 0, 1, 0);
      glTranslatef(start_x, start_y, 0);

      glRotatef(fangle, 0, 1, 0);
      if (i == last_placed.x && j == last_placed.y)
        glRotatef(active_figure_angle, 1, 0, 0);
      if (table[i][j] == INT_CIRCLE)

        glDrawO(-19, -20, 5);
      if (table[i][j] == INT_CROSS)
        glDrawX(-19, -20, 5);
      glPopMatrix();
            
    }
  }
}

void Repaint(float& camera_angle)
{
  auto fStep = 5;
  DrawGLScene();
  glPushMatrix();
  glTranslatef(0, 0, 0);
      
  camera_angle += fStep;
  glRotatef(board_rotate_angle, 0, 1, 0);
  glDrawBoard();
  glPopMatrix();
  DrawFigures(game_table,camera_angle);
  glLoadIdentity();
  SwapBuffers(gl_game_paint.hDc);
}

void OnWmInnitDialog(HWND hwndDlg)
{
  InitGameVars();
  SetWindowLong(hwndDlg, GWL_STYLE, CS_HREDRAW | CS_VREDRAW | CS_OWNDC | WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^WS_MINIMIZEBOX ^WS_MAXIMIZEBOX);//
  InitGlPaint(hwndDlg);
  AddMenus(hwndDlg);
}

void OnWmSize(LPARAM lParam)
{
  GLint iWidth = LOWORD(lParam);
  GLint iHeight = HIWORD(lParam);

  if (iWidth == 0)iWidth = 1;
  if (iHeight == 0)iHeight = 1;


  glViewport(0, 0, iWidth, iHeight);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0f, iWidth / (float)iHeight, 0.1f, 1000.0f); //k¹t widzenia, Rozmiar okna, Z min, Z max
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void OnIdmNewGameVsHuman(HWND hwndDlg)
{
  is_game_on = true;
  if (has_cross_started == true && is_player_cross == true)
  {
    ChangeActivePlayer(hwndDlg);
    has_cross_started = false;
  }
  else
    has_cross_started = true;
  InitGameControls(hwndDlg);
  if (is_game_vs_computer && is_player_cross == false)
  {
    EmplaceByComputer();
    ChangeActivePlayer(hwndDlg);
    InvalidateRect(hwndDlg, NULL, TRUE);
    UpdateWindow(hwndDlg);
    UpdateResults();
  }
  UpdateWindow(hwndDlg);
}

INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
  {
  case WM_COMMAND:
    {
      switch (LOWORD(wParam))
      {
      case IDM_NEW_GAME_VS_COMPUTER:
        is_game_vs_computer = true;
      case IDM_NEW_GAME_VS_HUMAN:
        {          
              OnIdmNewGameVsHuman(hwndDlg);      
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
    }
    return TRUE;
  case WM_TIMER:
    {
    static float camera_angle = 0;
      if(wParam == REPAINT_TIMER)
      {
        Repaint(camera_angle);
      }

    if(wParam ==ROTATE_TIMER)
    {
      if (active_figure_angle > 170)
      {
        active_figure_angle = 0;
        KillTimer(hwndDlg, ROTATE_TIMER);
      }
      else
      {
        auto fstep = 7;
        active_figure_angle += fstep;
      }
    }
    }
    return TRUE;
  case WM_INITDIALOG:
    {
      OnWmInnitDialog(hwndDlg);
    }
    return TRUE;
  case WM_SIZE:
    {
      OnWmSize(lParam);
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

          //thread to play sound
         auto t = std::thread([]() {note_player->PlayMelody("G1;D2;G2", "16;16;8",300); });
         t.detach();
         last_placed.x = field_index_x;
         last_placed.y = field_index_y;
         active_figure_angle = 0;
         SetTimer(hwndMainWindow, ROTATE_TIMER, 20, NULL);
          EmplaceByPlayer(field_index_x, field_index_y);
          UpdateResults();
          ChangeActivePlayer(hwndDlg);
        
          if (is_game_vs_computer)
          {
            EmplaceByComputer();
            ChangeActivePlayer(hwndDlg);
            UpdateResults();
          }
        }
      }
    }
    return TRUE;
  case WM_CLOSE:
    {
      DestroyWindow(hwndDlg); // zniszczenie okna
      delete note_player;
      PostQuitMessage(0); //Komunikat polecenia zakończenia aplikacji
      return TRUE;
    }
    return FALSE;
  }
  return FALSE;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
  h_instance = hInstance;
  note_player = new NotePlayer();
  note_player->InitNotePlayer(1, 44100, 8);

  hwndMainWindow = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MAINVIEW1), NULL, DialogProc);
  ShowWindow(hwndMainWindow, iCmdShow);


  MSG msg = {};
  SetTimer(hwndMainWindow, REPAINT_TIMER, 50, NULL);
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  delete note_player;
}

