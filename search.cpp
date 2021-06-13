#pragma once
#include "board.cpp"
#include <map>
#include <unordered_map>

using namespace std;


double expected_delta(board, int, double, int, int*, map<piece, pair<COL, int>>&);
vector<mv> best_moves(board, int);



vector<mv> best_moves(board b, int team)
{
	// need a depth or number of searches probably

}

