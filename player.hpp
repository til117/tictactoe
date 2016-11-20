#ifndef _TICTACTOE_PLAYER_HPP_
#define _TICTACTOE_PLAYER_HPP_

#include <iostream>
#include <stdlib.h>
#include <string>
#include <limits>
#include <algorithm>
#include <time.h>
#include "constants.hpp"
#include "deadline.hpp"
#include "move.hpp"
#include "gamestate.hpp"
#include <vector>

namespace TICTACTOE
{

double UtilityFunc(GameState,uint8_t);
double MiniMax_passdown (GameState currentState, uint8_t player, int depth, double alpha );
int MiniMax (GameState currentState, uint8_t player);
void CheckLine( int player_sum, int opponent_sum, double &player_sum_tot, double &opponent_sum_tot );

class Player
{
public:
    ///perform a move
    ///\param pState the current state of the board
    ///\param pDue time before which we must have returned
    ///\return the next state the board is in after our move
    GameState play(const GameState &pState, const Deadline &pDue);
};

/*namespace TICTACTOE*/ }

#endif
