#include <Windows.h>
#include <cmath>
#include <string>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
  char* app_name = "knur app";
  int number = 40;
  int down_bound = 0, up_bound = 40;
  char text[200];
  //do nothing
  int return_message; 
  MessageBox(0, "Wbyierz liczbê z zakresu 1-40", app_name, MB_OK);
  for (int i = 0; i < std::log2(number) - 1; ++i)
  {
    //char* text =  spr"Wbyierz czy liczba jest wiêksz od" + std::to_string(up_bound / 2);
    
    sprintf_s(text, 200, "Wbyierz czy liczba jest wiêksz od %d", (up_bound + down_bound) / 2);
    return_message = MessageBox(0,  text , app_name, MB_YESNO);
    if (return_message == IDYES)
    {
     // MessageBox(0, "perfekcja", "knur app", MB_OK);
      down_bound += up_bound/2 ;
    }
    else
    {
      up_bound -= down_bound /2;
    }
 
  }
  sprintf_s(text, 200, "Liczba to %d", (up_bound + down_bound)/2);
  MessageBox(0, text, app_name, MB_OK | MB_ICONQUESTION);
  return 0;
}