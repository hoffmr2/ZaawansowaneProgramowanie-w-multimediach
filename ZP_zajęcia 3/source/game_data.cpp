#include "game_data.h"
#include <cstdlib>
#include <ctime>


GameData::GameData() : attempts_number_(0)
{
  srand(time(nullptr));
  DrawNumber();
}


GameData::~GameData()
{
}

void GameData::ResetData()
{
  attempts_number_ = 0;
  DrawNumber();
}

void GameData::DrawNumber()
{
  number_to_guess_ = (rand() % 40) + 1;
}

void GameData::IncreaseAttemptsNumber()
{
  ++attempts_number_;
}

int GameData::GetAttemptsNumber()
{
  return attempts_number_;
}

int GameData::GetNumberToGues()
{
  return number_to_guess_;
}

bool GameData::Compare(int value)
{
  return value == number_to_guess_;
}
