#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "piece.cpp"
// #include "search.cpp"

#define DEBUG 0

using namespace std;

enum COL { A, B, C, D, E, F, G, H};
// crying idk how to do stuff :((((

struct mv
{
	//UHHH so we need a move that has the basic movements setup + the board that has the actual ones like QxD6 or something instead of QD6
	pair<COL, int> from;
	pair<COL, int> to;
	piece p1, p2;
	mv(pair<COL, int> f, pair<COL, int> t, piece& a, piece& b)
	{
		from = f;
		to = t;
		p1 = a;
		p2 = b;
	}
};
class board
{
	public:
		board(); // basic game
		board(const board& );
		void print_board();
		piece& operator()(COL, int);
		void manual(COL, int);
		int eval() const;
		bool verify_straight(COL, int, COL, int);
		bool verify_pawn(COL, int, COL, int);
		bool verify_diagonal(COL, int, COL, int);
		bool in_check(int);
		bool legal_move(COL, int, COL, int);
		bool move(COL, int, COL, int);
		bool make_move(COL, int, COL, int);
		bool is_legal(COL, int, COL, int, int);
		void move_nc(COL, int, COL, int);
		bool in_checkmate(int);
		// l r, u d
		pair<pair<int, int>, pair<int, int> > count_straight(COL, int);
		// lu ld, ru rd
		pair<pair<int, int>, pair<int, int> > count_diagonal(COL, int);
	private:
		// 0, 0 is A8
		// 1, 0 is A7
		// 7, 0 is A1
		// 7, 7 is H1
		piece g_b[8][8];
		vector<mv> past_moves;
};