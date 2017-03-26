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
  HWND hwnd_checkbox_against_computer = nullptr;
  int game_table[TABLE_SIZE][TABLE_SIZE];




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

bool EmplaceInCorner()
{
  if(game_table[2][2] == 0)
  {
    game_table[2][2] = INT_CIRCLE;
    wsprintf(sz_text, CIRCLE);
    SetWindowText(buttons[8], sz_text);
    return true;
  }
  if (game_table[0][2] == 0)
  {
    game_table[0][2] = INT_CIRCLE;
    wsprintf(sz_text, CIRCLE);
    SetWindowText(buttons[2], sz_text);
    return true;
  }
  if (game_table[2][0] == 0)
  {
    game_table[2][0] = INT_CIRCLE;
    wsprintf(sz_text, CIRCLE);
    SetWindowText(buttons[6], sz_text);
    return true;
  }
  if (game_table[0][0] == 0)
  {
    game_table[0][0] = INT_CIRCLE;
    wsprintf(sz_text, CIRCLE);
    SetWindowText(buttons[0], sz_text);
    return true;
  }
  return false;
}

bool IsMiddleFree()
{
  return game_table[1][1] == 0;
}

bool EmplaceInTheMiddle()
{
  if (game_table[1][1] == 0)
  {
    game_table[1][1] = INT_CIRCLE;
    wsprintf(sz_text, CIRCLE);
    SetWindowText(buttons[4], sz_text);
    return true;
  }
  return false;
}

bool EmplaceInRow(int row)
{
  for(int i=0;i<TABLE_SIZE;++i)
  {
    if(game_table[row][i] ==0)
    {
      game_table[row][i] = INT_CIRCLE;
      wsprintf(sz_text, CIRCLE);
      SetWindowText(buttons[3*row+ i %3], sz_text);
      return true;
    }
  }
  return false;
}

bool EmplaceInColumn(int column)
{
  for (int i = 0; i<TABLE_SIZE; ++i)
  {
    if (game_table[i][column] == 0)
    {
      game_table[i][column] = INT_CIRCLE;
      wsprintf(sz_text, CIRCLE);
      SetWindowText(buttons[3 * i + column % 3], sz_text);
      return true;
    }
  }
  return false;
}

void EmplaceAnyWhere()
{
  for (int i = 0; i<TABLE_SIZE; ++i)
    for (int j = 0; j < TABLE_SIZE; ++j)
    {
      if (game_table[i][j] == 0)
      {
        game_table[i][j] = INT_CIRCLE;
        wsprintf(sz_text, CIRCLE);
        SetWindowText(buttons[3 * i + j % 3], sz_text);
      }
    }
}


void EmplaceByComputer()
{
  int empty, circles, crosses;

  for (int i = 0; i < TABLE_SIZE; ++i)
  {
    SymbolsInColumn(i, empty, circles, crosses);
    if (circles == 2 || crosses == 2)
    {
      if (EmplaceInColumn(i))return;
    }
    SymbolsInRow(i, empty, circles, crosses);
    if (circles == 2 || crosses == 2)
      if (EmplaceInRow(i))return;
  }
  if (EmplaceInTheMiddle())return;
  if (EmplaceInCorner())return;
  EmplaceAnyWhere();
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
}

void CrossWon()
{
  wsprintf(sz_text, GAME_STATUS_STOPED);
  SetWindowText(hwnd_static_game, sz_text);
  wsprintf(sz_text, GAME_RESULT_WINNER_CROSS);
  SetWindowText(hwnd_static_result, sz_text);
  wsprintf(sz_text, RESTART_GAME);
  is_game_on = false;
}

void CircleWon()
{
  wsprintf(sz_text, GAME_STATUS_STOPED);
  SetWindowText(hwnd_static_game, sz_text);
  wsprintf(sz_text, GAME_RESULT_WINNER_CIRCLE);
  SetWindowText(hwnd_static_result, sz_text);
  wsprintf(sz_text, RESTART_GAME);
  is_game_on = false;
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
                      EmplaceByComputer();
                      UpdateGameTable();
                      ChangeActivePlayer(hwndDlg);
                      CheckResults();
                      
                    }
                    return TRUE;
                  }
                  default:
                      if (is_game_on)
                      {
                        auto hwnd_button = reinterpret_cast<HWND>(lParam);//uchwyt na przycisk
                          if(GetWindowTextLength(hwnd_button) == BUTTON_EMPTY)
                          {
                              SetCrossOrCircleText();
                              SetWindowText(hwnd_button, sz_text);
                              UpdateGameTable();
                              ChangeActivePlayer(hwndDlg);
                              CheckResults();
                             
                              if(IsDlgButtonChecked(hwndDlg,IDC_CHECK_AGAINST_COMPUTER) == BST_CHECKED )
                              {
                                EmplaceByComputer();
                                UpdateGameTable();
                                ChangeActivePlayer(hwndDlg);
                                CheckResults();
                                
                              }
                            
                              
                          }
                      }
                      return TRUE;
              }
              return TRUE;
          }
          return TRUE;
      }
      return TRUE;
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
  HWND hwndMainWindow = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MAINVIEW), NULL, DialogProc);
  ShowWindow(hwndMainWindow, iCmdShow);
  HWND hwnd_static3 = GetDlgItem(hwndMainWindow, IDC_STATIC3);
  InitControlsHwnd(hwndMainWindow);
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