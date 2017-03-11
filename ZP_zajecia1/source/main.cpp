#include <Windows.h>
#include <stdio.h>


#define TEXT_BUFFOR_SIZE 200
#define MB_YESNOQUESTION MB_YESNO | MB_ICONQUESTION
#define APP_NAME "knur app"
#define CHOOSE_NUMBER "Wbyierz liczbê z zakresu 1-40"
#define IS_NUMBER_EQUAL "Czy liczba to %d ?"
#define IS_NUMBER_GREATER "Czy liczba jest wiêksz od %d"
#define UNCORRECT_NUMBER_CHOSSEN "Najwidoczniej wybra³eœ liczbê z poza zakresu" 


#define ASK_IF_NUMBER_IS_OK \
sprintf_s(text, TEXT_BUFFOR_SIZE, IS_NUMBER_EQUAL, number); \
return_message = MessageBox(0, text, APP_NAME, MB_YESNOQUESTION); \
if (return_message == IDYES) \
return 0;



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{

  auto down_bound = 1, up_bound = 40;
  int number = up_bound, return_message;
  char text[TEXT_BUFFOR_SIZE];
  //do nothing
  MessageBox(0,CHOOSE_NUMBER , APP_NAME, MB_OK);

  ASK_IF_NUMBER_IS_OK


  while (down_bound != up_bound)
  {
    //char* text =  spr"Wbyierz czy liczba jest wiêksz od" + std::to_string(up_bound / 2);
    if (number == (up_bound + down_bound) / 2)
      break;

    number = (up_bound + down_bound) / 2;
    ASK_IF_NUMBER_IS_OK

    sprintf_s(text, TEXT_BUFFOR_SIZE, IS_NUMBER_GREATER , number);
    return_message = MessageBox(0,  text , APP_NAME, MB_YESNO);

    if (return_message == IDYES)
      down_bound = number;
    else
      up_bound = number;
 
  }
 // sprintf_s(text, TEXT_BUFFOR_SIZE, UNCORRECT_NUMBER_CHOSSEN, (up_bound + down_bound)/2);
  MessageBox(0, UNCORRECT_NUMBER_CHOSSEN, APP_NAME, MB_OK | MB_ICONWARNING);
  return 0;
}