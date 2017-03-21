#include <Windows.h>
#include "res.h"
#include <winuser.h>

/*
 * gra w zgadywanie
 * komputer losuje cyfre z zakresu 1-40
 * wpisuje w text box
 * komputer odpowiada czy iwêksza czy mniejsza
 * 2 pola static box
 */

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
      case IDC_BUTTON1:
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
        auto hwnd_button = GetDlgItem(hwndDlg, IDC_BUTTON1);
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


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
  HWND hwnd_main_window = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MAINVIEW), NULL, DialogProc);
  ShowWindow(hwnd_main_window, iCmdShow);

  //pêtla obs³ugi komunikatów

  MSG msg = {};

  while(GetMessage(&msg,NULL,0,0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
 
  return 0;
}


