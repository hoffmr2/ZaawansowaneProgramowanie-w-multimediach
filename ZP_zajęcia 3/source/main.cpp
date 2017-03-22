#include <Windows.h>
#include "res.h"
#include <winuser.h>
#include <cstdio>

/*
 * gra w zgadywanie
 * komputer losuje cyfre z zakresu 1-40
 * wpisuje w text box
 * komputer odpowiada czy iwêksza czy mniejsza
 * 2 pola static box
 */

#define ATTEMPT_NUMBER "liczba prób %d"
#define IS_OK_DEFAULT "wpisz wartoœæ z zakresu 1 - 40"

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
        MessageBox(hwndDlg, TEXT("Test"), TEXT("Klikniêcie"), MB_OK);
        auto hwnd_edit_box = GetDlgItem(hwndDlg, IDC_EDIT1);
        INT text_length = GetWindowTextLength(hwnd_edit_box);
        CHAR text[500];
        GetWindowText(hwnd_edit_box, text, text_length+1);
        SetWindowText((HWND)lParam, text);
        return TRUE;
      }
    case EN_CHANGE:
      switch (LOWORD(wParam))
      {
      case IDC_EDIT1:
        auto hwnd_edit_box = GetDlgItem(hwndDlg, IDC_EDIT1);
        auto hwnd_button = GetDlgItem(hwndDlg, IDC_BUTTON_START);
        INT text_length = GetWindowTextLength(hwnd_edit_box);
        CHAR text[500];
        GetWindowText(hwnd_edit_box, text,text_length+1);
        SetWindowText(hwnd_button, text);
        return TRUE;
      }


    }
    return FALSE;
  case WM_CLOSE:
    DestroyWindow(hwndDlg);
    PostQuitMessage(0);
    return TRUE;
  case WM_LBUTTONDOWN:
    CHAR text[200];
    wsprintf(text, "Klikn¹³eœ myszk¹ x=%d, y=%d", LOWORD(lParam), HIWORD(lParam));
    MessageBox(hwndDlg, text, TEXT("Klikniêcie"), MB_OK);
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


