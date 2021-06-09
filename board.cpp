#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "piece.cpp"

using namespace std;

enum COL { A, B, C, D, E, F, G, H};

struct
{
	//UHHH so we need a move that has the basic movements setup + the board that has the actual ones like QxD6 or something instead of QD6
	pair<COL, int> from;
	pair<COL, int> to;
}move;
class board
{
	public:
		board(); // basic game
		void print_board();
		piece& operator()(COL, int);
		void manual(COL, int);
	private:
		// 0, 0 is A8
		// 1, 0 is A7
		// 7, 0 is A1
		// 7, 7 is H1
		piece g_b[8][8];
		
};
board::board()
{
	// there's actually nothing to do... love jesus
}
piece& board::operator()(COL c, int row)
{
	// A -> [][0];
	// XJ -> [7-J][X]
	// A1 -> [7][X]
	return g_b[7-row][c];
}
void board::print_board()
{
	cout << "hahsn" << endl;
	for (int i=0;i<8;i++) 
	{
		for (int j=0;j<8;j++)
		{
			//cout << "a ";
			if ((i^j)&1) cout << "\033[101;";
			else cout << "\033[102;";
			cout << "1;";
			cout <<  g_b[i][j];
		}
		cout << "\n";
	}
}
void board::manual(COL c, int r)
{
	g_b[7-r][c].set_team(1);
}
int main()
{
	cout << "hello\n";
	board joe;
	for (int i=0; i<8; i++)
	{
		COL j = (COL)i;
		joe(j, 1).change_type(PAWN);
		joe(j, 6).change_type(PAWN);
	}
	// add uncool pieces
	for (int i=0; i<8; i+=7)
	{
		joe(A, i).change_type(ROOK);
		joe(B, i).change_type(KNIGHT);
		joe(C, i).change_type(BISHOP);
		joe(D, i).change_type(QUEEN);
		joe(E, i).change_type(KING);
		joe(F, i).change_type(BISHOP);
		joe(G, i).change_type(KNIGHT);
		joe(H, i).change_type(ROOK);
	}
	// joe.manual(COL::A,5);
	// (joe(COL::A,5)).set_team(1);
	// joe(A,5).change_type(ROOK);
	// for (int i=0;i<4;i++)
	// {
	// 	joe(A,i+2).change_type(NONE);
	// 	joe(B,i+2).change_type(NONE);
	// 	joe(C,i+2).change_type(NONE);
	// 	joe(D,i+2).change_type(NONE);
	// 	joe(E,i+2).change_type(NONE);
	// 	joe(F,i+2).change_type(NONE);
	// 	joe(G,i+2).change_type(NONE);
	// 	joe(H,i+2).change_type(NONE);
	// }
	joe.print_board();
}
