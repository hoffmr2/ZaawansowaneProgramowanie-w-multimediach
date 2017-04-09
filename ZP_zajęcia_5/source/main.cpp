#include <Windows.h>
#include "res.h"
#include <stdio.h>
#include <vector>

#define GAME_RESULT_UNKNOWN "gra w trakcie"
#define GAME_RESULT_WINNER_CROSS "wygr¹³o krzy¿yk"
#define GAME_RESULT_WINNER_CIRCLE "wygrta³o kó³ko"
#define GAME_RESULT_DRAW "remis"
#define GAME_STATUS_PLAYING "gra siê toczy"
#define GAME_STATUS_STOPED "gra nieaktywna"
#define RESTART_GAME "restart"
#define START_GAME "zacznij"
#define BUTTON_EMPTY 0
#define CIRCLE "O"
#define CROSS "X"
#define TABLE_SIZE 3
#define INT_CROSS 1
#define INT_CIRCLE -1
#define CHAR_CROSS 'X'
#define CHAR_CIRCLE 'O'
#define BOARD_SIZE  300
#define BOARD_POZITION_Y 120
#define BOARD_POZITION_X 50
#define GRID_SIZE 100
#define PEN_SIZE 10
#define BOARD_COLOR RGB(104, 63, 32)
#define CIRCLE_COLOR RGB(100, 150, 200)
#define CROSS_COLOR RGB(200, 150, 150)
#define BOARD_LINES_COLOR RGB(200, 200, 200)
#define BOARD_LINES_SIZE 5
#define APP_NAME "knur"



  CHAR sz_text[500];
  bool is_game_on = false;
  bool is_player_cross = true;
  bool game_result = false;
  bool has_cross_started = false;
  std::vector<HWND> buttons;
  HWND hwnd_radio_cross = nullptr;
  HWND hwnd_radio_circle = nullptr;
  HWND hwnd_static_game = nullptr;
  HWND hwnd_static_result = nullptr;
  HWND hwnd_button_start = nullptr;
  HWND hwndMainWindow;
  HWND hwnd_checkbox_against_computer = nullptr;
  int game_table[TABLE_SIZE][TABLE_SIZE];

  void DrawEmptyBoard(HDC hdc);
  void DrawCircle(HDC hdc, int x, int y);
  void DrawCross(HDC hdc, int x, int y);


//void CheckGameResult(HWND b1, HWND b2, HWND b3, HWND b4, HWND b5, HWND b6, HWND b7, HWND b8, HWND b9);

void SymbolsInRow(int row,int &empty,int &circles, int& crosses)
{
  empty = circles = crosses = 0;
  for (int i = 0; i < TABLE_SIZE; ++i)
  {
    switch (game_table[row][i])
    {
    case 0:
      ++empty;
      break;
    case INT_CROSS:
      ++crosses;
      break;
    case INT_CIRCLE:
      ++circles;
      break;
    }
  }  
}

void SymbolsInColumn(int column, int &empty, int &circles, int& crosses)
{
  empty = circles = crosses = 0;
  for (int i = 0; i < TABLE_SIZE; ++i)
  {
    switch (game_table[i][column])
    {
    case 0:
      ++empty;
      break;
    case INT_CROSS:
      ++crosses;
      break;
    case INT_CIRCLE:
      ++circles;
      break;
    }
  }
}

void SymbolsInCrossUpDown(int &empty, int &circles, int& crosses)
{
  empty = circles = crosses = 0;
  for (int i = 0; i < TABLE_SIZE; ++i)
  {
    switch (game_table[i][i])
    {
    case 0:
      ++empty;
      break;
    case INT_CROSS:
      ++crosses;
      break;
    case INT_CIRCLE:
      ++circles;
      break;
    }
  }
}

void SymbolsInCrossDownUp(int &empty, int &circles, int& crosses)
{
  empty = circles = crosses = 0;
  for (int i = 0,j=2; i < TABLE_SIZE; ++i,--j)
  {
    switch (game_table[j][i])
    {
    case 0:
      ++empty;
      break;
    case INT_CROSS:
      ++crosses;
      break;
    case INT_CIRCLE:
      ++circles;
      break;
    }
  }
}

void EmplaceInUpLeftCorner(HDC hdc)
{
  game_table[0][0] = INT_CIRCLE;
  if (is_player_cross)
    DrawCross(hdc, 0, 0);
  else
    DrawCircle(hdc, 0, 0);
}

void EmplaceInDownLeftCorner(HDC hdc)
{
  game_table[2][0] = INT_CIRCLE;
  if (is_player_cross)
    DrawCross(hdc, 2, 0);
  else
    DrawCircle(hdc, 2, 0);
}

void EmplaceInUpRightCorner(HDC hdc)
{
  game_table[0][2] = INT_CIRCLE;
  if (is_player_cross)
    DrawCross(hdc, 0, 2);
  else
    DrawCircle(hdc, 0, 2);
}

void EmplaceInDownRightCorner(HDC hdc)
{
  game_table[2][2] = INT_CIRCLE;
  if (is_player_cross)
    DrawCross(hdc, 2, 2);
  else
    DrawCircle(hdc, 2, 2);
}

bool EmplaceInCorner(HDC hdc)
{
  int empty, circles, crosses, first_row =0,last_row =2;
  SymbolsInRow(first_row, empty, circles, crosses);
  if(circles == first_row)
  {
    if (game_table[0][0] == 0)
    {
      EmplaceInUpLeftCorner(hdc);
      return true;
    }
    if (game_table[0][2] == 0)
    {
      EmplaceInUpRightCorner(hdc);
      return true;
    }

  }

  SymbolsInRow(last_row, empty, circles, crosses);
  if (circles == first_row)
  {
    if (game_table[2][0] == 0)
    {
      EmplaceInDownLeftCorner(hdc);
      return true;
    }
    if (game_table[0][2] == 0)
    {
      EmplaceInDownRightCorner(hdc);
      return true;
    }

  }
  if(game_table[2][2] == 0)
  {
    EmplaceInDownRightCorner(hdc);
    return true;
  }
  if (game_table[0][2] == 0)
  {
    EmplaceInUpRightCorner(hdc);
    return true;
  }
  if (game_table[2][0] == 0)
  {
    EmplaceInDownLeftCorner(hdc);
    return true;
  }
  if (game_table[0][0] == 0)
  {
    EmplaceInUpLeftCorner(hdc);
    return true;
  }
  return false;
}

bool IsMiddleFree()
{
  return game_table[1][1] == 0;
}

bool EmplaceInTheMiddle(HDC hdc)
{
  if (game_table[1][1] == 0)
  {
    game_table[1][1] = INT_CIRCLE;
    if (is_player_cross)
      DrawCross(hdc, 1, 1);
    else
      DrawCircle(hdc, 1, 1);
    return true;
  }
  return false;
}

bool EmplaceInRow(int row,HDC hdc)
{
  for(int i=0;i<TABLE_SIZE;++i)
  {
    if(game_table[row][i] ==0)
    {
      game_table[row][i] = INT_CIRCLE;
      if (is_player_cross)
        DrawCross(hdc, row, i);
      else
        DrawCircle(hdc, row, i);
      return true;
    }
  }
  return false;
}

bool EmplaceInColumn(int column,HDC hdc)
{
  for (int i = 0; i<TABLE_SIZE; ++i)
  {
    if (game_table[i][column] == 0)
    {
      game_table[i][column] = INT_CIRCLE;
      if (is_player_cross)
        DrawCross(hdc, i, column);
      else
        DrawCircle(hdc, i, column);
      return true;
    }
  }
  return false;
}

void EmplaceAnyWhere(HDC hdc)
{
  for (int i = 0; i<TABLE_SIZE; ++i)
    for (int j = 0; j < TABLE_SIZE; ++j)
    {
      if (game_table[i][j] == 0)
      {
        game_table[i][j] = INT_CIRCLE;
        if (is_player_cross)
          DrawCross(hdc, i, j);
        else
          DrawCircle(hdc, i, j);
      }
    }
}

bool EmplaceInCrossUpDown(HDC hdc)
{

  for (int i = 0; i < TABLE_SIZE; ++i)
  {
    if(game_table[i][i] ==0)
    {
      game_table[i][i] = INT_CIRCLE;
      if (is_player_cross)
        DrawCross(hdc, i, i);
      else
        DrawCircle(hdc, i, i);
      return true;
    }
  }
  return false;
}

bool EmplaceInCrossDownUp(HDC hdc)
{
  
  for (int i = 0, j = 2; i < TABLE_SIZE; ++i, --j)
  {
    if (game_table[i][j] == 0)
    {
      game_table[i][j] = INT_CIRCLE;
      if (is_player_cross)
        DrawCross(hdc, i, j);
      else
        DrawCircle(hdc, i, j);
      return true;
    }
  }
  return false;
}


void EmplaceByComputer(HDC hdc)
{
  int empty, circles, crosses;

  for (int i = 0; i < TABLE_SIZE; ++i)
  {
    SymbolsInColumn(i, empty, circles, crosses);
    if (circles == 2 || crosses == 2)
    {
      if (EmplaceInColumn(i,hdc))return;
    }
    SymbolsInRow(i, empty, circles, crosses);
    if (circles == 2 || crosses == 2)
      if (EmplaceInRow(i,hdc))return;
  }
  SymbolsInCrossDownUp(empty, circles, crosses);
  if (circles == 2 || crosses == 2)
  {
    if (EmplaceInCrossDownUp(hdc))return;
  }

  SymbolsInCrossUpDown(empty, circles, crosses);
  if (circles == 2 || crosses == 2)
  {
    if (EmplaceInCrossUpDown(hdc))return;
  }
  if (EmplaceInTheMiddle(hdc))return;
  if (EmplaceInCorner(hdc))return;
  EmplaceAnyWhere(hdc);
}
void ClearButtons()
{
  for (const auto &button : buttons)
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
  is_player_cross = !is_player_cross;
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
  is_game_on = true;
  wsprintf(sz_text, GAME_STATUS_PLAYING);
  SetWindowText(hwnd_static_game, sz_text);
  wsprintf(sz_text, GAME_RESULT_UNKNOWN);
  SetWindowText(hwnd_static_result, sz_text);                        
  wsprintf(sz_text, RESTART_GAME);
  SetWindowText(hwnd_button_start, sz_text);
  for (int i = 0; i < TABLE_SIZE; ++i)
    for (int j = 0; j < TABLE_SIZE; ++j)
      game_table[i][j] = 0;
  HDC hdc = GetDC(hwndDlg);
  DrawEmptyBoard(hdc);

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

void UpdateGameTable()
{
  for(int i=0;i<buttons.size();++i)
  {
    GetWindowText(buttons[i], sz_text, 50);
    if (sz_text[0] == CHAR_CROSS)
    {
      game_table[i / TABLE_SIZE][i % TABLE_SIZE] =  INT_CROSS;
    }
    if (sz_text[0] == CHAR_CIRCLE)
    {
      game_table[i / TABLE_SIZE][i % TABLE_SIZE] = INT_CIRCLE;
    }
  }
}

void CheckForDraw(int& empty)
{
  empty = 0;
  for (int i = 0; i < TABLE_SIZE; ++i)
    for (int j = 0; j < TABLE_SIZE; ++j)
      if (game_table[i][j] == 0)
        ++empty;
  if(empty == 0)
  {
    wsprintf(sz_text, GAME_STATUS_STOPED);
    SetWindowText(hwnd_static_game, sz_text);
    wsprintf(sz_text, GAME_RESULT_DRAW);
    SetWindowText(hwnd_static_result, sz_text);
    wsprintf(sz_text, RESTART_GAME);
    is_game_on = false;
    MessageBox(hwndMainWindow,GAME_RESULT_DRAW, APP_NAME, MB_OK);
  }
}

void CheckResults()
{
  int circles, crosses, empty;
  for (int i = 0; i < TABLE_SIZE; ++i)
  {
    SymbolsInColumn(i,empty,circles,crosses);
    if(circles == TABLE_SIZE)
    {
      CircleWon();
    }
    if(crosses == TABLE_SIZE)
    {
      CrossWon();
    }
    SymbolsInRow(i, empty, circles, crosses);
    if (circles == TABLE_SIZE)
    {
      CircleWon();
    }
    if (crosses == TABLE_SIZE)
    {
      CrossWon();
    }

  }
  SymbolsInCrossDownUp( empty, circles, crosses);
  if (circles == TABLE_SIZE)
  {
    CircleWon();
  }
  if (crosses == TABLE_SIZE)
  {
    CrossWon();
  }
  SymbolsInCrossUpDown(empty, circles, crosses);
  if (circles == TABLE_SIZE)
  {
    CircleWon();
  }
  if (crosses == TABLE_SIZE)
  {
    CrossWon();
  }
  CheckForDraw(empty);
}

bool IsMouseDownOnBoard(int x, int y)
{
  return (x > BOARD_POZITION_X && x < BOARD_POZITION_X + BOARD_SIZE) && (y > BOARD_POZITION_Y && y < BOARD_POZITION_Y + BOARD_SIZE);
}

void RepaintBoard(HDC hdc)
{
  for(int i=0;i<TABLE_SIZE;++i)
  {
    for(int j=0;j<TABLE_SIZE;++j)
    {
      if (game_table[i][j] == INT_CIRCLE)
        DrawCircle(hdc, i, j);
      if (game_table[i][j] == INT_CROSS)
        DrawCross(hdc, i, j);
    }
  }
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
                      HDC hdc = GetDC(hwndDlg);
                      EmplaceByComputer(hdc);
                      UpdateGameTable();
                      ChangeActivePlayer(hwndDlg);
                      CheckResults();
                      
                    }
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
    case WM_LBUTTONDOWN:
      {
        if(is_game_on)
        {
          int x = LOWORD(lParam);
          int y = HIWORD(lParam);
         
          if(IsMouseDownOnBoard(x, y))
          {
            int field_index_x = (x - BOARD_POZITION_X) / GRID_SIZE;
            int field_index_y = (y - BOARD_POZITION_Y) / GRID_SIZE;
            if (game_table[field_index_x][field_index_y] != 0)
              return TRUE;
           
              HDC hdc = GetDC(hwndDlg);
              if (is_player_cross)
              {
                
                DrawCross(hdc, field_index_x, field_index_y);
                game_table[field_index_x][field_index_y] = INT_CROSS;
              }
              else
              {
                DrawCircle(hdc, field_index_x, field_index_y);
                game_table[field_index_x][field_index_y] = INT_CIRCLE;
              }
              ChangeActivePlayer(hwndDlg);
              CheckResults();

              if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_AGAINST_COMPUTER) == BST_CHECKED)
              {
                EmplaceByComputer(hdc);
                ChangeActivePlayer(hwndDlg);
                CheckResults();

              }
              ReleaseDC(hwndDlg, hdc);
            
          }
        }
      }
      return TRUE;

    case WM_PAINT:
    {
      HDC hdc = GetDC(hwndDlg);
      DrawEmptyBoard(hdc);
      RepaintBoard(hdc);
      ReleaseDC(hwndDlg, hdc);
      return DefWindowProc(hwndDlg, uMsg, wParam, lParam);
    }
    return TRUE;
    case WM_CLOSE:
    {
        DestroyWindow(hwndDlg); // zniszczenie okna
        PostQuitMessage(0); //Komunikat polecenia zakoñczenia aplikacji
        return TRUE;
    }  
    return FALSE;
  }
  return FALSE;
}
void DrawCircle(HDC hdc,int x,int y)
{
  int offset = 20;
  int start_x = BOARD_POZITION_X + x*GRID_SIZE + offset;
  int start_y = BOARD_POZITION_Y + y*GRID_SIZE + offset;
  int end_x = start_x + GRID_SIZE - 2*offset;
  int end_y = start_y + GRID_SIZE - 2*offset;
  HPEN h_my_pen = CreatePen(PS_SOLID, PEN_SIZE, CIRCLE_COLOR);
  auto brush = CreateSolidBrush(BOARD_COLOR);
  SelectObject(hdc, h_my_pen);
  SelectObject(hdc, brush);
  MoveToEx(hdc, start_x,start_y , NULL);
  Ellipse(hdc,start_x ,start_y ,end_x,end_y);
  DeleteObject(h_my_pen);
  DeleteObject(brush);

}

void DrawCross(HDC hdc, int x, int y)
{
  int offset = 20;
  int start_x = BOARD_POZITION_X + x*GRID_SIZE + offset;
  int start_y = BOARD_POZITION_Y + y*GRID_SIZE + offset;
  int end_x = start_x + GRID_SIZE - 2 * offset;
  int end_y = start_y + GRID_SIZE - 2 * offset;
  HPEN h_my_pen = CreatePen(PS_SOLID, PEN_SIZE,CROSS_COLOR);

  int size = 20;
  SelectObject(hdc, h_my_pen);
  MoveToEx(hdc, start_x, start_y, NULL);
  LineTo(hdc, end_x, end_y);
  MoveToEx(hdc, end_x, start_y, NULL);
  LineTo(hdc, start_x, end_y);
  DeleteObject(h_my_pen);
}

void DrawEmptyBoard(HDC hdc)
{
  HPEN h_my_pen = CreatePen(PS_SOLID, BOARD_LINES_SIZE, BOARD_LINES_COLOR);
  SelectObject(hdc, h_my_pen);
  RECT rect;
  rect.left = BOARD_POZITION_X;
  rect.top = BOARD_POZITION_Y;
  rect.right = BOARD_POZITION_X + BOARD_SIZE;
  rect.bottom = BOARD_POZITION_Y + BOARD_SIZE;
  FillRect(hdc, &rect, CreateSolidBrush(BOARD_COLOR));

  for (int i = 1; i < 3; ++i)
  {
    MoveToEx(hdc, BOARD_POZITION_X + i * GRID_SIZE, BOARD_POZITION_Y, NULL);
    LineTo(hdc, BOARD_POZITION_X + i * GRID_SIZE, BOARD_POZITION_Y+BOARD_SIZE);
    MoveToEx(hdc, BOARD_POZITION_X, BOARD_POZITION_Y + i * GRID_SIZE, NULL);
    LineTo(hdc, BOARD_POZITION_X + BOARD_SIZE, BOARD_POZITION_Y + i*GRID_SIZE);

  }
  DeleteObject(h_my_pen);
}

void InitControlsHwnd(HWND hwndMainWindow)
{
  hwnd_checkbox_against_computer = GetDlgItem(hwndMainWindow, IDC_CHECK_AGAINST_COMPUTER);
  hwnd_static_game = GetDlgItem(hwndMainWindow, IDC_STATIC_GAME);
  hwnd_static_result = GetDlgItem(hwndMainWindow, IDC_STATIC_RESULT);
  hwnd_radio_circle = GetDlgItem(hwndMainWindow, IDC_RADIO_CIRCLE);
  hwnd_radio_cross = GetDlgItem(hwndMainWindow, IDC_RADIO_CROSS);
  hwnd_button_start = GetDlgItem(hwndMainWindow, IDC_BUTTON10);
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
  hwndMainWindow = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MAINVIEW), NULL, DialogProc);
  ShowWindow(hwndMainWindow, iCmdShow);
  HWND hwnd_static3 = GetDlgItem(hwndMainWindow, IDC_STATIC3);
  InitControlsHwnd(hwndMainWindow);
  InitGameControls(hwndMainWindow);
  ClearButtons();




	MSG msg = {};
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
int iTextLength = GetWindowTextLength(hwndEditBox);//Pobranie d³ugoœci tekstu
          GetWindowText(hwndEditBox, szText, iTextLength + 1)
          */