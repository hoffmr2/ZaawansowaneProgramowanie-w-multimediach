
#ifndef GAME_DATA_H_
#define GAME_DATA_H_


class GameData
{
public:
  GameData();
  ~GameData();
  void ResetData();
  
  void IncreaseAttemptsNumber();
  int GetAttemptsNumber();
  int GetNumberToGues();
  bool Compare(int value);
private:
  void DrawNumber();

  int attempts_number_;
  int number_to_guess_;
};

#endif