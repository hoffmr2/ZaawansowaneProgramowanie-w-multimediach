#include <Windows.h>
#include "res.h"
#include <winuser.h>
#include <math.h>

#include "note_player.h"

#pragma comment(lib,"winmm.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "advapi32.lib")
/*
 * gra w zgadywanie
 * komputer losuje cyfre z zakresu 1-40
 * wpisuje w text box
 * komputer odpowiada czy iwêksza czy mniejsza
 * 2 pola static box
 */
#define APP_NAME "Knur"
#define CONGRATULATIONS "Brawo zgad³eœ"
#define NOTES "podaj nuty w formacie A0;B2; ..."
#define RYTHM "podaj wartoœci rytmiczne np: 8;8;8;1;16 ..."
#define IS_OK_DEFAULT "wpisz wartoœæ z zakresu 1 - 40"
#define IS_OK_TOLOW "za ma³oooo"
#define IS_OK_TOHIGH "za du¿oooo"
#define ERROR_MESSAGE "nastepnym razem wpisz poprawne dane :)"
#define MIN_VALUE 1
#define MAX_VALUE 40

#define FC 261.626
#define FD 277.183
#define FE 329.628
#define FF 349.288
#define FG 391.995
#define FA 444.000
#define FB 493.883
NotePlayer note_player;
void InitControls(HWND hwnd_main_window);
std::string notes = { "G0;G0;G0;D#0;A#1;G0;D#0;A#1;G0;D1;D1;D1;D#1;A#1;F#0;D#0;A#1;G0;G1;G0;G0;G1;F#1;F1;E1;D#1;E1;G#0;C#1;C1;H1;A#1;A0;A#1;D#0;F#0;D#0;F#0;A#1;G0;A#1;D1" };
std::string rythms = { "4;4;4;6;16;4;6;16;2;4;4;4;6;16;4;6;16;2;4;6;16;4;6;16;16;16;4;8;4;6;16;16;16;4;8;4;6;16;4;6;16;2" };

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
        InitControls(hwndDlg);
        return TRUE;

      case IDC_BUTTON_PLAY:
        auto hwnd_edit_notes = GetDlgItem(hwndDlg, IDC_EDIT_NOTES);
        auto hwnd_edit_rythm = GetDlgItem(hwndDlg, IDC_EDIT_RYTHM);
      //  int value;
        INT text_length = GetWindowTextLength(hwnd_edit_notes);
     //   CHAR notes[500];
      //  CHAR rythm[500];
     //   GetWindowText(hwnd_edit_notes, notes, text_length + 1);
     //   text_length = GetWindowTextLength(hwnd_edit_rythm);
    //    GetWindowText(hwnd_edit_rythm, rythm, text_length + 1);
      //  note_player.PlayMelody(notes, rythm, 120);
        note_player.PlayMelody(notes,rythms , 100);
        return TRUE;
      }



    }
    return FALSE;
  case WM_INITDIALOG:
    InitControls(hwndDlg);
    return TRUE;
  case WM_CLOSE:
    DestroyWindow(hwndDlg);
    PostQuitMessage(0);
    return TRUE;
  }
  return FALSE;
}


void InitNotesText(HWND hwnd_main_window)
{
  auto hwnd_attempt_notes = GetDlgItem(hwnd_main_window, IDC_STATIC_NOTES);
  CHAR text[50];
  wsprintf(text, NOTES, 0);
  SetWindowText(hwnd_attempt_notes, text);
}

void InitRythmText(HWND hwnd_main_window)
{
  auto hwnd_tohigh_tolow = GetDlgItem(hwnd_main_window, IDC_STATIC_RYTHM);
  CHAR text[50];
  wsprintf(text, IS_OK_DEFAULT);
  SetWindowText(hwnd_tohigh_tolow, text);
}

void InitEditText(HWND hwnd_main_window)
{
  auto hwnd_edit = GetDlgItem(hwnd_main_window, IDC_EDIT_NOTES);
  SetWindowText(hwnd_edit, nullptr);
  hwnd_edit = GetDlgItem(hwnd_main_window, IDC_EDIT_RYTHM);
  SetWindowText(hwnd_edit, nullptr);
}

void InitControls(HWND hwnd_main_window)
{
  InitNotesText(hwnd_main_window);
  InitRythmText(hwnd_main_window);
  InitEditText(hwnd_main_window);

}



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{


  HWND hwnd_main_window = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MAINVIEW), NULL, DialogProc);
  ShowWindow(hwnd_main_window, iCmdShow);
  MSG msg = {};

  note_player = NotePlayer();
  note_player.InitNotePlayer(1, 44100L, 8);
 // note_player.Play(1, 440);
  //TODO :
  /*
   * char sNotes nuty
   * char sTimes
   * stworzyæ melodje przybnajmniej 36s
   */
  while(GetMessage(&msg,NULL,0,0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return 0;
}

