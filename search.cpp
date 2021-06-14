#pragma once
#include "board.h"
#include <map>
#include <unordered_map>

using namespace std;

#ifndef CALCULATE_LIMIT
#define CALCULATE_LIMIT 69
#endif
// implements the worst stochastic engine you'll ever see

double expected_delta(board, int, double, int, int*);
void best_moves(board&, int, double, int*, clock_t&, int, mv, int);
mv make_move(board, int);
void init_search();

// double expected_delta(board b, int t, double prob, int IDK, int* num_moves)
// {

// }

void best_moves(board& b, int team, double interest, int* evaluated, clock_t& start, int best, mv to_go, int current_val)
{
	// need a depth or number of searches probably
	// check if in check - then we just do every 1 depth move and do the 'best' one LOL LOL LOL LOL	

	if (*evaluated > CALCULATE_LIMIT || (double)(clock() - start) / CLOCKS_PER_SEC > 10) return;
	// go through
	for (int i=0;i<8;i++)
		for (int j=1;j<=8;j++)
		{
			if (b((COL)i,j).get_team() == team)
			{
				COL K = (COL) i;
				// 10% chance of moving LOL LOL LOL
				if ((rand() % 10) == 0)
				{
					// use this piece to make moves :(
					vector<int> available_moves = b(K,j).get_moves();
					for (int k : available_moves)
					{
						switch(k)
						{
							case 0:
							{
								// try all 8 king moves
								for (int i_ = -1; i_ <= 1; i_++)
									for (int j_ = -1; j_ <= 1; j_++)
									{
										if (b.is_legal(K,j,(COL)(K+i_),j+j_,team))
										{
											// make da move

										}
									}

							}
							case 1:
							{
								// straight moves
								pair<pair<int, int>, pair<int, int> > = b.count_straight(K,j);
							}
							case 2:
							{
								pair<pair<int, int>, pair<int, int> > = b.count_diagonal(K,j);
							}
							case 3:
							{

							}
							case 4:
							{
								// try forward 1/2, or take on both sides
							}
							default:
								break;
						}
					}
				}
			}
		}

}

// mv make_move(board b, int t)
// {
// 	return best_moves(b,t).back();
// }


void init_search()
{
	// love seeding pleas do good <3
	srand(500);
}