#include <iostream>
#include <vector>

using namespace std;


class piece
{
	public:
		enum piece_type { KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN};
		piece();
		piece(piece::piece_type);
		piece(piece::piece_type, int);
		void set_team(int);
		void change_type(piece::piece_type); // pawn promotion - could make a new one but who cares
		vector<int> get_moves();
		void test();		
	private:
		int team, captures;
		piece_type type;
};


piece::piece()
{
	team = 0;
	captures = 0;
	type = piece::PAWN;
}

piece::piece(piece::piece_type tp)
{
	team = 0;
	captures = 0;
	type = tp;
}

piece::piece(piece::piece_type tp, int t)
{
	team = t;
	captures = 0;
	type = tp;
}


void piece::test()
{
	
	cout << type << " " << team <<"\n";
}

int main()
{
	piece joe(piece::KING, 1);
	joe.test();
}


