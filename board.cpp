#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "piece.cpp"

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
		void print_board();
		piece& operator()(COL, int);
		void manual(COL, int);
		int eval() const;
		bool verify_straight(COL, int, COL, int);
		bool verify_pawn(COL, int, COL, int);
		bool verify_diagonal(COL, int, COL, int);
		bool in_check(int);
		bool legal_move(COL, int, COL, int, int);
		bool move(COL, int, COL, int);
		bool make_move(COL, int, COL, int);
		
	private:
		// 0, 0 is A8
		// 1, 0 is A7
		// 7, 0 is A1
		// 7, 7 is H1
		piece g_b[8][8];
		vector<mv> past_moves;
};
board::board()
{
	// there's actually nothing to do... love jesus
}
piece& board::operator()(COL c, int row)
{
	// A -> [][0];
	// XJ -> [8-J][X]
	// A1 -> [8][0]
	return g_b[8-row][c];
}
void board::print_board()
{
	cout << "hahsn" << endl;
	for (int i=0;i<8;i++) 
	{
		cout << (8-i) << " ";
		for (int j=0;j<8;j++)
		{
			//cout << "a ";
			if ((i^j)&1) cout << "\033[101;";
			else cout << "\033[102;";
			cout << "1;";
			cout <<  g_b[i][j].to_col();
		}
		cout << "\n";
	}
	cout << "  ABCDEFGH\n";
}
void board::manual(COL c, int r)
{
	g_b[7-r][c].set_team(1);
}
int board::eval() const
{
	// lame evaluation that does 1,3,3,5,9 values and king is pretty much infinity
	int scores[2];
	scores[0]=scores[1]=0;
	for (int i=0;i<8;i++)
	{
		for (int j=0;j<8;j++)
		{
			scores[g_b[i][j].get_team()]+=g_b[i][j].get_val();
		}
	}
	return scores[1]-scores[0]; // positive is in favor of computer, negative in favor of player
	
	
}
bool board::verify_straight(COL f_c, int f_r, COL t_c, int t_r)
{
	if (f_r == t_r)
	{
		COL min_c = min(f_c, t_c);
		COL max_c = max(f_c, t_c);
		for (int j = min_c + 1; j < max_c; j++)
			if ((*this)((COL)j,f_r).is_piece()) return false;
		return true;
	} 
	else 
	{
		int min_r = min(f_r,t_r);
		int max_r = max(f_r,t_r);
		for (int j = min_r + 1; j < max_r; j++)
			if ((*this)(f_c,j).is_piece()) return false;
		return true;
	}

}

bool board::verify_pawn(COL f_c, int f_r, COL t_c, int t_r)
{
	cerr << "PAWN MOVE\n";
	// has to be +/-2
	// team 1 (white) can only be neg 
	// team 0 (black) should be positive
	cerr << (*this)(f_c,f_r).get_move_count() << "\n";
	if ((*this)(t_c,t_r).is_piece()) return false;
	if ((*this)(f_c,f_r).get_team()==1)
	{
		if (f_r - t_r == -1) return true;
		if (f_r - t_r == -2) 
			return (*this)(f_c,f_r).get_move_count() == 0 && !(*this)(f_c,f_r+1).is_piece();
	} 
	else
	{
		if (f_r - t_r == 1) return true;
		if (f_r - t_r == 2) 
			return (*this)(f_c,f_r).get_move_count() == 0 && !(*this)(f_c,f_r-1).is_piece();
	}
	return false;

}
bool board::verify_diagonal(COL f_c, int f_r, COL t_c, int t_r)
{
	int col_mod, row_mod;
	// going up
	if (f_c < t_c) col_mod = 1;
	else col_mod = -1; // going down

	if (f_r < t_r) row_mod = 1;
	else row_mod = -1;
	
	cerr << "diagonal " << col_mod << " " << row_mod << "\n";
	for (int i = f_c + col_mod, j = f_r + row_mod; (COL)i != t_c && j != t_r; i+=col_mod, j+=row_mod)
	{
		cerr << "COLUMN " << i << " ROW" << j << "\n";
		if ((*this)((COL)i,j).is_piece()) return false;
	}
	return true;
}
bool board::in_check(int team)
{
	return false;
	// find position of king, then look in all move types

}
bool board::legal_move(COL f_c, int f_r, COL t_c, int t_r, int team)
{
	if (move(f_c,f_r,t_c,t_r) && (!in_check(team)))
	{
		past_moves.push_back(mv(make_pair(f_c,f_r),make_pair(t_c,t_r),(*this)(f_c,f_r),(*this)(t_c,t_r)));
		(*this)(f_c,f_r).inc_move_count();
		(*this)(t_c,t_r)=(*this)(f_c,f_r);
		(*this)(f_c,f_r) = piece();
		return true;
	}
	cerr << "BAD\n";
	return false;
}
bool board::move(COL f_c, int f_r, COL t_c, int t_r)
{
	//oob
	if (f_c < A || f_c > H || t_c < A || t_c > H) return false;
	if (f_r < 1 || f_r > 8 || t_r < 1 || t_r > 8) return false;

	if ((*this)(t_c,t_r).get_team() == (*this)(f_c,f_r).get_team()) return false;

	if (f_c == t_c || f_r == t_r)
	{
		// vertical
		switch((*this)(f_c,f_r).get_type())
		{
			case QUEEN:
				return verify_straight(f_c,f_r,t_c,t_r);
			case ROOK:
				return verify_straight(f_c,f_r,t_c,t_r);
			case PAWN:
				return f_c == t_c && verify_pawn(f_c,f_r,t_c,t_r);
			case KING:
				{
					// check castling too :((
					if (f_c == t_c)
						return abs(f_r-t_r) == 1;
					else
						return abs(f_c-t_c) == 1;

				}
			default:
				return false; // bushop and kngiht canont mooov straight up or duun
		}
	}
	// move has to be diagonal or knight
	cerr << "NOT STRAIGHT\n";
	if (abs(f_c - t_c) == abs(f_r - t_r) && ((*this)(f_c,f_r).get_type() == BISHOP || (*this)(f_c,f_r).get_type() == QUEEN) )
	{
		// digaonal
		return verify_diagonal(f_c,f_r,t_c,t_r);
	}
	// has to be kngith i think

	if ((*this)(f_c,f_r).get_type() != KNIGHT) return false;
	cerr << "KNIGHT MOVE\n";
	int dx = abs(f_c - t_c);
	int dy = abs(f_r - t_r);
	return (dx == 2 && dy ==1) || (dx == 1 && dy == 2);
}	



int main()
{
	cout << "hello\n";
	board joe;
	for (int i=0; i<8; i++)
	{
		COL j = (COL)i;
		joe(j, 2).change_type(PAWN);
		joe(j, 2).set_team(1);
		joe(j, 7).change_type(PAWN);
		joe(j, 7).set_team(0);

	}
	// add uncool pieces
	for (int i=1; i<=8; i+=7)
	{
		joe(A, i).change_type(ROOK);
		joe(A, i).set_team(i&1);
		joe(B, i).change_type(KNIGHT);
		joe(B, i).set_team(i&1);
		joe(C, i).change_type(BISHOP);
		joe(C, i).set_team(i&1);
		joe(D, i).change_type(QUEEN);
		joe(D, i).set_team(i&1);
		joe(E, i).change_type(KING);
		joe(E, i).set_team(i&1);
		joe(F, i).change_type(BISHOP);
		joe(F, i).set_team(i&1);
		joe(G, i).change_type(KNIGHT);
		joe(G, i).set_team(i&1);
		joe(H, i).change_type(ROOK);
		joe(H, i).set_team(i&1);
	}

	joe.print_board();
	//cout << joe.eval() << "\n";

	//cout << "\n\njoe";
	//cout << "\n" << joe(A,1) << "\n";
	// vector<int> m = joe(A,2).get_moves();
	// for (int i=0;i<m.size();i++) cout << i << " " << m[i] << "\n";
	while(true)
	{
		string a,b;
		cin >> a >> b;
		joe.legal_move((COL)(a[0]-'A'),a[1]-'0',(COL)(b[0]-'A'),b[1]-'0',1);
		joe.print_board();
	}
}
