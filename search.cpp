#pragma once
#include "board.h"
#include <map>
#include <unordered_map>

using namespace std;

#ifndef CALCULATE_LIMIT
#define CALCULATE_LIMIT 69
#endif

#ifndef INITIAL_INTEREST
#define INITIAL_INTEREST 5
#endif

// implements the worst stochastic engine you'll ever see

double expected_delta(board, int, double, int, int*);
void best_moves(board&, int, double, int&, clock_t&, int&, mv&, mv&, int);
mv make_move(board, int);
void init_search();

// double expected_delta(board b, int t, double prob, int IDK, int* num_moves)
// {

// }


void best_moves(board& b, int team, double interest, int& evaluated, clock_t& start, int& best, mv& to_go, mv& best_move)
{
	// need a depth or number of searches probably
	// check if in check - then we just do every 1 depth move and do the 'best' one LOL LOL LOL LOL	

	if (evaluated > CALCULATE_LIMIT || (double)(clock() - start) / CLOCKS_PER_SEC > 10) return;
	

	// if interest is < 1, then eval and check out
	if (interest < 1)
	{
		int val = b.eval();
		evaluated++;
		if (val > best)
		{
			best = val;
			best_move = to_go;
			return;
		}
	}
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
										if (i_ == 0 && j_ ==0) continue;
										if (b.is_legal(K,j,(COL)(K+i_),j+j_,team))
										{
											// make da move
											board nxt = b;
											nxt.move_nc(K,j,(COL)(K+i_),j+j_);
											if (interest == INITIAL_INTEREST)
											{
												// construct move, since it will be first
												to_go = mv(make_pair(K,j),make_pair((COL)(K+i_),j+j_),b(K,j),b((COL)(K+i_),j+j_));
											}
											best_moves(nxt,team^1,interest-1,evaluated, start, best, to_go, best_move);
										}
									}

							}
							case 1:
							{
								// straight moves
								pair<pair<int, int>, pair<int, int> > space = b.count_straight(K,j);


							}
							case 2:
							{
								pair<pair<int, int>, pair<int, int> > space = b.count_diagonal(K,j);

							}
							case 3:
							{
								// try all 8 knight moves
								int dx[] = {-2,-2,-1,-1,1,1,2,2};
								int dy[] = {1,-1,2,-2,2,-2,1,-1};
								for (int i_=0;i_<8;i_++)
								{
									if (!b.is_legal(K,j,(COL)(i+dx[i_]),j+dy[i_],team)) continue;
									
								}
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
