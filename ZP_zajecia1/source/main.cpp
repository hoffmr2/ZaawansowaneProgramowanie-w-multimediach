#include <Windows.h>


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
  char* app_name = "knur app";
  //do nothing
  int return_message = MessageBox(0, "Podoba ci sie palikacja", app_name, MB_YESNO);

  if (return_message == IDYES)
    MessageBox(0, "perfekcja", "knur app", MB_OK);
  else
    MessageBox(0, "A co jest nie tak??", app_name, MB_OK | MB_ICONQUESTION);

  return 0;
}