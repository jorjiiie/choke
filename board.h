#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "piece.cpp"

using namespace std;

enum COL { A, B, C, D, E, F, G, H};

struct move
{
	//UHHH so we need a move that has the basic movements setup + the board that has the actual ones like QxD6 or something instead of QD6
	pair<COL, int> from;
	pair<COL, int> to;
} move;
class board
{
	public:
		board(); // basic game
		void print_board();
		piece& operator()(COL, int);
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
			cout << "a ";
			cout << "\033[101;" <<  g_b[i][j] << " ";
		}
		cout << "\n";
	}
}
int main()
{
	cout << "hello\n";
	board joe;
	joe.print_board();
}
