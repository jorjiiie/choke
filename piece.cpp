#include <iostream>
#include <vector>

using namespace std;

enum piece_type {KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN, NONE};

class piece
{
	public:
		piece();
		piece(piece_type);
		piece(piece_type, int);
		void set_team(int);
		void change_type(piece_type); // pawn promotion - could make a new one but who cares
		vector<int> get_moves();
		void test();		
		friend ostream& operator<<(ostream&, const piece&);		
		char get_name() const;
	private:
		int team, captures;
		piece_type type;
};


piece::piece()
{
	team = 0;
	captures = 0;
	type = piece_type::NONE;
}

piece::piece(piece_type tp)
{
	team = 0;
	captures = 0;
	type = tp;
}

piece::piece(piece_type tp, int t)
{
	team = t;
	captures = 0;
	type = tp;
}


void piece::test()
{
	
	cout << "AHSJDASD\n";
	cout << type << " " << team <<"\n";
}
/*
int main()
{
	piece joe(piece::KING, 1);
	joe.test();
}
*/
char piece::get_name() const
{
//	cerr << this->type << " TYPE\n";
	switch(this->type)
	{
		case KING:
			return 'K';
		case QUEEN:
			return 'Q';
		case ROOK:
			return 'R';
		case KNIGHT:
			return 'N';
		case BISHOP:
			return 'B';
		case PAWN:
			return 'P';
		default:
			return ' ';
	}

}
ostream& operator<<(ostream& stream, const piece& p)
{
	// for now, BLUE for team 0, red for team 1
	if (p.team)
		stream << "97";
	else 
		stream << "30";
//	cerr << "TYPE IS " << p.type << "\n";
	stream << 'm';
	stream << string(1,p.get_name());
	stream << "\033[0m";
	return stream;
}
void piece::set_team(int j)
{
	team = j;
}
void piece::change_type(piece_type tp)
{
	type = tp;
}
