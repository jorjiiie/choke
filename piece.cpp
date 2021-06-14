/*
	Authors: Theo Kreuger, Ryan Zhu
	piece class that is base piece of chess game
	has some extra stats like moves for 2 tile pawn move and castling, kills for why not, last move for en passant, etc 
*/

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
		piece(const piece&);
		void set_team(int);
		void change_type(piece_type); // pawn promotion - could make a new one but who cares
		vector<int> get_moves();
		void test();		
		friend ostream& operator<<(ostream&, const piece&);		
		char get_name() const;
		int get_val() const;
		int get_team() const;
		void inc_move_count();
		int get_move_count() const; // for pawn 2x move & castling
		bool is_piece() const; // for empty tiles
		string to_col() const;
		piece_type get_type() const;

	private:
		int team, captures, move_count, last_move; // last move is for en passant :(
		piece_type type;
};

piece::piece(const piece& o)
{
	this->team = o.team;
	this->captures = o.captures;
	this->move_count = o.move_count;
	this->last_move = o.last_move;
	this->type = o.type;
}
int piece::get_val() const
{
	switch(this->type)
	{
		case KING:
			return 1<<31;
		case QUEEN:
			return 9;
		case ROOK:
			return 5;
		case BISHOP:
			return 3;
		case KNIGHT:
			return 3;
		case PAWN:
			return 1;
		default:
			return 0;
	}
}
int piece::get_team() const
{
	return this->team;
}

piece::piece()
{
	team = -1;
	captures = 0;
	type = piece_type::NONE;
	move_count = 0;
}

piece::piece(piece_type tp)
{
	team = -1;
	captures = 0;
	type = tp;
	move_count = 0;
}

piece::piece(piece_type tp, int t)
{
	team = t;
	captures = 0;
	type = tp;
	move_count = 0;
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
	stream << "[" << p.team << ", " << p.get_name() << "]";
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
void piece::inc_move_count()
{
	this->move_count++;
}
int piece::get_move_count() const
{
	return this->move_count;
}
bool piece::is_piece() const
{
	return this->type != NONE;
}
vector<int> piece::get_moves()
{
	vector<int> j;
	switch(this->type)
	{
		// computer won't make any special moves like castling or en passant
		case KING:
			// king move
			j.push_back(0);
			break;
		case QUEEN:
			// rook + bishop moves
			j.push_back(1);
			j.push_back(2);
			break;
		case ROOK:
			// rook moves
			j.push_back(1);
			break;
		case BISHOP:
			// bishop moves
			j.push_back(2);
			break;
		case KNIGHT:
			// cool knight move
			j.push_back(3);
			break;
		case PAWN:
			// pawn move
			j.push_back(4);
			break;
		default:
			break;

	}
	return j;
}

string piece::to_col() const
{
	string ret = "";
	// for now, BLUE for team 0, red for team 1
	if (this->team)
		ret+= "97";
	else 
		ret += "30";
	//	cerr << "TYPE IS " << p.type << "\n";
	ret+='m';
	ret+= string(1,this->get_name());
	ret +="\033[0m";
	return ret;
}
piece_type piece::get_type() const
{
	return this->type;
}
