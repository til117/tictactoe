/*
Compile with command: g++ *.cpp -Wall -o TTT
While in folder TTT
*/

#include "player.hpp"
#include <cstdlib>
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

#define search_depth 3
#define inf std::numeric_limits<double>::infinity()

namespace TICTACTOE
{
  /*-------------------------------------------*/
  /*      ---  Function Declarations  ---      */
  int MiniMax (GameState currentState, uint8_t player);
  double MiniMax_passdown(GameState currentState, uint8_t player, int depth, double alpha);
  double UtilityFunc(GameState state, uint8_t player);
  void CheckLine( int player_sum, int opponent_sum, double &player_sum_tot, double &opponent_sum_tot);

  /*--------------------------*/
  /*      ---  Play  ---      */
  GameState Player::play(const GameState &pState,const Deadline &pDue)
  {
      std::vector<GameState> lNextStates;
      pState.findPossibleMoves(lNextStates);
      int i;

      // If no possible moves
      if (lNextStates.size() == 0)
      {
        return GameState(pState, Move());
      }

      if (pState.getNextPlayer()&CELL_O)
      {
        i = MiniMax(pState,CELL_X);
      }

      if (pState.getNextPlayer()&CELL_X)
      {
        i = MiniMax(pState,CELL_O);
      }
      return lNextStates[i];
  }

  /*-----------------------------*/
  /*      ---  MiniMax  ---      */
  int MiniMax (GameState currentState, uint8_t player)
  {
  	std::vector<GameState> lPossibleMoves;
  	currentState.findPossibleMoves(lPossibleMoves);

  	int best_possible_index;
  	double best_possible, v;

    // Check if there is a possible move
  	if (lPossibleMoves.empty())
  	{
  		std::cout << "No Possible Moves!!!" << std::endl;
  		return -1;
  	}
    // If there are moves => Continue..
  	else
  	{
  		best_possible = -inf;
      for(uint8_t i = 0; i < lPossibleMoves.size(); i++)
      {
        // PLAYER
  			if( player & CELL_X)
  			{
  				v = MiniMax_passdown(lPossibleMoves[i], CELL_O, search_depth, -best_possible);
  				std::cerr << v << " ";
  			}
        // PLAYER
  			if( player & CELL_O)
  			{
  				v = MiniMax_passdown(lPossibleMoves[i], CELL_X, search_depth, -best_possible);
  				std::cerr << v << " ";
  			}
  			if (v > best_possible) {
  				best_possible_index = i;
  				best_possible = v;
  			}
  		}
  		std::cerr << " => " << best_possible << std::endl;
  		return best_possible_index;
  	}
  }

  /*--------------------------------------*/
  /*      ---  MiniMax_passdown  ---      */
  double MiniMax_passdown(GameState currentState, uint8_t player, int depth, double alpha)
  {
  	std::vector<GameState> lPossibleMoves;
  	currentState.findPossibleMoves(lPossibleMoves);

  	depth--;
  	double v;

    // Check if there is a possible move
  	if (lPossibleMoves.empty() || depth <= 0) {
  		return -UtilityFunc(currentState, player);
  	}
    // For every possible move => do the minimax
  	else {
  		double best_possible= -inf;

      for(uint8_t i = 0; i < lPossibleMoves.size(); i++)
      {
  			if( player & CELL_X )
  			{
  				v = MiniMax_passdown(lPossibleMoves[i], CELL_O, depth, -best_possible);
  			}
  			if( player & CELL_O )
  			{
  				v = MiniMax_passdown(lPossibleMoves[i], CELL_X, depth, -best_possible);
  			}
  			best_possible = std::max(best_possible,v);
        // Pruning
   			if(best_possible >= alpha)
  			{
  				return -best_possible;
  			}
  		}
  		return -best_possible;
  	}
  }

  /*--------------------------------------*/
  /*      ---  Utility Function  ---      */
  double UtilityFunc(GameState state, uint8_t player)
  {
  	double opponent_sum_tot = 0;
  	double player_sum_tot = 0;
  	int player_sum = 0;
  	int opponent_sum = 0;
    // Assign the opponent
  	uint8_t opponent;
  	if( player & CELL_X )
  	{
  		opponent = CELL_O;
  	}
  	else
  	{
  		opponent = CELL_X;
  	}

  	for( int i = 0; i < 4; i++)
  	{
      // Check row marks
  		player_sum = 0;
  		opponent_sum = 0;
  		for( int j =0; j < 4; j++ )
  		{
  			if( state.at(i*4+j)&opponent )
  			{
  				opponent_sum++;
  			}
  			if( state.at(i*4+j)&player )
  			{
  				player_sum++;
  			}
  		}
  		CheckLine( player_sum, opponent_sum, player_sum_tot, opponent_sum_tot );

  		// Check column marks
  		player_sum = 0;
  		opponent_sum = 0;
  		for( int j =0; j < 4; j++ )
  		{
  			if( state.at(j*4+i)&opponent )
  			{
  				opponent_sum++;
  			}
  			if( state.at(j*4+i)&player )
  			{
  				player_sum++;
  			}
  		}
  		CheckLine( player_sum, opponent_sum, player_sum_tot, opponent_sum_tot );

  	}

  	// Check diagonal marks (top left => bottom right)
  	player_sum = 0;
  	opponent_sum = 0;
  	for( int i = 0; i < 4; i++)
  	{
  		if( state.at(i+i*4)&opponent )
  		{
  			opponent_sum++;
  		}
  		if( state.at(i+i*4)&player )
  		{
  			player_sum++;
  		}
  	}
  	CheckLine( player_sum, opponent_sum, player_sum_tot, opponent_sum_tot );


	  // Check diagonal marks (top right => bottom left)
  	player_sum = 0;
  	opponent_sum = 0;
  	for( int i = 0; i < 4; i++)
  	{
  		if( state.at(3*i+3)&opponent )
  		{
  			opponent_sum++;
  		}
  		if( state.at(3*i+3)&player )
  		{
  			player_sum++;
  		}
  	}
  	CheckLine( player_sum, opponent_sum, player_sum_tot, opponent_sum_tot );

    // Secure victory/defeat
  	if( player_sum_tot == inf )
  	{
  		return inf;
  	}
  	if( opponent_sum_tot == inf )
  	{
  		return -inf;
  	}
    // Utility
  	return player_sum_tot-opponent_sum_tot;
  }


  /*-------------------------------*/
  /*      ---  CheckLine  ---      */
  void CheckLine( int player_sum, int opponent_sum, double &player_sum_tot, double &opponent_sum_tot )
  {
    // A winning state
  	if( player_sum == 4 )
  	{
  		player_sum_tot = inf;
  	}
    // A losing state
  	if (opponent_sum == 4 && player_sum == 0)
    {
  		opponent_sum_tot = inf;
  	}
    // No advantage if both players have marked the row/column/diagonal
  	if (opponent_sum>0 && player_sum>0)
  	{
  		opponent_sum = player_sum = 0; // return utility = 0
  	}
  	player_sum++;
  	opponent_sum++;
    // Total marks
  	player_sum_tot += player_sum*player_sum;
  	opponent_sum_tot += opponent_sum*opponent_sum;
  }
}
