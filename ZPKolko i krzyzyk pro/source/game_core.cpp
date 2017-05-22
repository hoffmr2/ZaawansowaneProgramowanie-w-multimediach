#include "game_core.h"


void InitGameVars()
{
  is_game_on = false;
  is_player_cross = true;
  game_result = false;
  has_cross_started = false;
}

void InitGameTable()
{
  for (int i = 0; i < TABLE_SIZE; ++i)
    for (int j = 0; j < TABLE_SIZE; ++j)
      game_table[i][j] = 0;
}

void SymbolsInRow(int row, int& empty, int& circles, int& crosses)
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

void SymbolsInColumn(int column, int& empty, int& circles, int& crosses)
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

void SymbolsInCrossUpDown(int& empty, int& circles, int& crosses)
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

void SymbolsInCrossDownUp(int& empty, int& circles, int& crosses)
{
  empty = circles = crosses = 0;
  for (int i = 0, j = 2; i < TABLE_SIZE; ++i, --j)
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

void EmplaceInUpLeftCorner()
{
  game_table[0][0] = INT_CIRCLE;
}

void EmplaceInDownLeftCorner()
{
  game_table[2][0] = INT_CIRCLE;
}

void EmplaceInUpRightCorner()
{
  game_table[0][2] = INT_CIRCLE;
}

void EmplaceInDownRightCorner()
{
  game_table[2][2] = INT_CIRCLE;
}

bool EmplaceInCorner()
{
  int empty, circles, crosses, first_row = 0, last_row = 2;
  SymbolsInRow(first_row, empty, circles, crosses);
  if (circles == first_row)
  {
    if (game_table[0][0] == 0)
    {
      EmplaceInUpLeftCorner();
      return true;
    }
    if (game_table[0][2] == 0)
    {
      EmplaceInUpRightCorner();
      return true;
    }
  }

  SymbolsInRow(last_row, empty, circles, crosses);
  if (circles == first_row)
  {
    if (game_table[2][0] == 0)
    {
      EmplaceInDownLeftCorner();
      return true;
    }
    if (game_table[0][2] == 0)
    {
      EmplaceInDownRightCorner();
      return true;
    }
  }
  if (game_table[2][2] == 0)
  {
    EmplaceInDownRightCorner();
    return true;
  }
  if (game_table[0][2] == 0)
  {
    EmplaceInUpRightCorner();
    return true;
  }
  if (game_table[2][0] == 0)
  {
    EmplaceInDownLeftCorner();
    return true;
  }
  if (game_table[0][0] == 0)
  {
    EmplaceInUpLeftCorner();
    return true;
  }
  return false;
}

bool IsMiddleFree()
{
  return game_table[1][1] == 0;
}

bool IsFieldNotFree(int raw,int column)
{
  return game_table[raw][column] != 0;
}
bool EmplaceInTheMiddle()
{
  if (game_table[1][1] == 0)
  {
    game_table[1][1] = INT_CIRCLE;
    return true;
  }
  return false;
}

bool EmplaceInRow(int row)
{
  for (int i = 0; i < TABLE_SIZE; ++i)
  {
    if (game_table[row][i] == 0)
    {
      game_table[row][i] = INT_CIRCLE;
      return true;
    }
  }
  return false;
}

bool EmplaceInColumn(int column)
{
  for (int i = 0; i < TABLE_SIZE; ++i)
  {
    if (game_table[i][column] == 0)
    {
      game_table[i][column] = INT_CIRCLE;
      return true;
    }
  }
  return false;
}

void EmplaceAnyWhere()
{
  for (int i = 0; i < TABLE_SIZE; ++i)
    for (int j = 0; j < TABLE_SIZE; ++j)
    {
      if (game_table[i][j] == 0)
      {
        game_table[i][j] = INT_CIRCLE;
      }
    }
}

bool EmplaceInCrossUpDown()
{
  for (int i = 0; i < TABLE_SIZE; ++i)
  {
    if (game_table[i][i] == 0)
    {
      game_table[i][i] = INT_CIRCLE;
      return true;
    }
  }
  return false;
}

bool EmplaceInCrossDownUp()
{
  for (int i = 0, j = 2; i < TABLE_SIZE; ++i, --j)
  {
    if (game_table[i][j] == 0)
    {
      game_table[i][j] = INT_CIRCLE;

      return true;
    }
  }
  return false;
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
  SymbolsInCrossDownUp(empty, circles, crosses);
  if (circles == 2 || crosses == 2)
  {
    if (EmplaceInCrossDownUp())return;
  }

  SymbolsInCrossUpDown(empty, circles, crosses);
  if (circles == 2 || crosses == 2)
  {
    if (EmplaceInCrossUpDown())return;
  }
  if (EmplaceInTheMiddle())return;
  if (EmplaceInCorner())return;
  EmplaceAnyWhere();
}

void EmplaceByPlayer(int row, int column)
{
  if (is_player_cross)
  {
    game_table[row][column] = INT_CROSS;
  }
  else
  {
    game_table[row][column] = INT_CIRCLE;
  }
}

void ChangePlayer()
{
  is_player_cross = !is_player_cross;
}

int CheckResults()
{
  int circles, crosses, empty,result = NO_RESULT;
  for (int i = 0; i < TABLE_SIZE; ++i)
  {
    SymbolsInColumn(i, empty, circles, crosses);
    if (circles == TABLE_SIZE)
    {
      result = CIRCLE_WON;
    }
    if (crosses == TABLE_SIZE)
    {
      result = CROSS_WON;
    }
    SymbolsInRow(i, empty, circles, crosses);
    if (circles == TABLE_SIZE)
    {
      result = CIRCLE_WON;
    }
    if (crosses == TABLE_SIZE)
    {
      result = CROSS_WON;
    }
  }
  SymbolsInCrossDownUp(empty, circles, crosses);
  if (circles == TABLE_SIZE)
  {
    result = CIRCLE_WON;
  }
  if (crosses == TABLE_SIZE)
  {
    result = CROSS_WON;
  }
  SymbolsInCrossUpDown(empty, circles, crosses);
  if (circles == TABLE_SIZE)
  {
    result = CIRCLE_WON;
  }
  if (crosses == TABLE_SIZE)
  {
    result = CROSS_WON;
  }
  if (result == NO_RESULT)
  {
    CheckForDraw(result);
  }
  return result;
}

void CheckForDraw(int&result)
{
  int empty = 0;
  for (int i = 0; i < TABLE_SIZE; ++i)
    for (int j = 0; j < TABLE_SIZE; ++j)
      if (game_table[i][j] == 0)
        ++empty;
  if (empty == 0)
    result = DRAW;

}