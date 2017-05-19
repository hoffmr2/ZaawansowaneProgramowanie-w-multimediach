
#ifndef GAME_CORE_H_
#define GAME_CORE_H_

#define TABLE_SIZE 3
#define INT_CROSS 1
#define INT_CIRCLE -1
#define CIRCLE_WON 15
#define CROSS_WON 17
#define DRAW 16
#define NO_RESULT 18

//game variebles
__declspec(selectany) int game_table[TABLE_SIZE][TABLE_SIZE];
__declspec(selectany) bool is_game_on = false;
__declspec(selectany) bool is_player_cross = true;
__declspec(selectany) bool game_result = false;
__declspec(selectany) bool has_cross_started = false;

//Init
void InitGameTable();
void InitGameVars();

//Check game state functions
void SymbolsInRow(int row, int& empty, int& circles, int& crosses);
void SymbolsInColumn(int column, int& empty, int& circles, int& crosses);
void SymbolsInCrossUpDown(int& empty, int& circles, int& crosses);
void SymbolsInCrossDownUp(int& empty, int& circles, int& crosses);
bool IsMiddleFree();
bool IsFieldNotFree(int row,int column);
int CheckResults();
void CheckForDraw(int&result);

//Computer Turn functions
void EmplaceInUpLeftCorner();
void EmplaceInDownLeftCorner();
void EmplaceInUpRightCorner();
void EmplaceInDownRightCorner();
bool EmplaceInCorner();
bool EmplaceInTheMiddle();
bool EmplaceInRow(int row);
bool EmplaceInColumn(int column);
void EmplaceAnyWhere();
bool EmplaceInCrossUpDown();
bool EmplaceInCrossDownUp();
void EmplaceByComputer();

//Player Emplace Func
void EmplaceByPlayer(int row, int column);
void ChangePlayer();

#endif



