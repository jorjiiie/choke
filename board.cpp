/*
	Authors: Theo Kreuger, Ryan Zhu
	board class 
	defined operator () for access (lazy LOL)
	8x8 board but probably can make this custom but at the same time i'm too lazy

*/

#pragma once

#include "board.h"
#include "search.cpp"


board::board()
{
	// there's actually nothing to do... love jesus
}
board::board(const board& b)
{
	for (int i=0;i<8;i++)
		for (int j=0;j<8;j++)
			this->g_b[i][j]=b.g_b[i][j];
	this->past_moves = b.past_moves;
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
	// cout << "hahsn" << endl;
	cout << "  ABCDEFGH\n";
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
void board::print_double()
{
	cout << "  ABCDEFGH     ABCDEFGH\n";
	for (int i=0;i<8;i++)
	{
		cout << (8-i) << " ";
		for (int j=0;j<8;j++)
		{
			if ((i^j)&1) cout << "\033[101;";
			else cout << "\033[102;";
			cout << "1;";
			cout <<  g_b[i][j].to_col();

		}
		cout << "   ";
		cout << (i+1) << " ";
		for (int j=0;j<8;j++)
		{
			if (((7-i)^j)&1) cout << "\033[101;";
			else cout << "\033[102;";
			cout << "1;";
			cout <<  g_b[7-i][j].to_col();
		}
		cout << "\n";
	}
	cout << "  ABCDEFGH     ABCDEFGH\n";
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
		cerr << "row move\n";
		COL min_c = min(f_c, t_c);
		COL max_c = max(f_c, t_c);
		for (int j = min_c + 1; j < max_c; j++)
		{
			cerr << j << " " << f_c << endl;
			if ((*this)((COL)j,f_r).is_piece()) return false;
		}
		cerr << "good move\n";
		return true;
	} 
	else 
	{
		cerr << "col move\n";
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
	// find position of king, then look in all move types
	COL c;
	int r = 0;
	for (int i=0;i<8;i++)
	{
		for (int j=1;j<=8;j++)
		{
			if ((*this)((COL)i,j).get_team() == team && (*this)((COL)i,j).get_type() == KING)
			{
				c = (COL) i;
				r = j;
				break;
			}	
		}
		if (r) break;
	}
	// now go in every direction
	// im actually crying
	if (r == 0) 
	{
		cerr << "somethign is wrong and king wasnt found\n";
		return true;
	}
	// IN PAIN
	// straight moves, diagonal moves, knight moves
	{ 
		
		// so i can call all my arrays dx/dy
		// megamind 9009
		int dx[] = {0,0,1,-1};
		int dy[] = {1,-1,0,0};
		// straight
		for (int i=0;i<4;i++)
		{
			int tmp_c = c+dx[i];
			int tmp_r = r+dy[i];
			while (tmp_c >= 0 && tmp_c < 8 && tmp_r > 0 && tmp_r <= 8)
			{
				if ((*this)((COL)tmp_c,tmp_r).get_team() != team && (*this)((COL)tmp_c,tmp_r).get_type() == QUEEN || (*this)((COL)tmp_c,tmp_r).get_type() == ROOK)
				{
					cerr << "straight check\n";
					return true;
				}
				if ((*this)((COL)tmp_c,tmp_r).is_piece()) break;
				tmp_c+=dx[i];
				tmp_r+=dy[i];
			}		
		}
	}
	{
		// diagonal
		int dx[] = {1,1,-1,-1};
		int dy[] = {1,-1,1,-1};
		for (int i=0;i<4;i++)
		{
			int tmp_c = c+dx[i];
			int tmp_r = r+dy[i];
			while (tmp_c >= 0 && tmp_c < 8 && tmp_r > 0 && tmp_r <= 8)
			{
				if ((*this)((COL)tmp_c,tmp_r).get_team() != team && (*this)((COL)tmp_c,tmp_r).get_type() == QUEEN || (*this)((COL)tmp_c,tmp_r).get_type() == BISHOP)
				{
					cerr << "diag check\n";
					return true;
				}
				if ((*this)((COL)tmp_c,tmp_r).is_piece()) break;
				tmp_c+=dx[i];
				tmp_r+=dy[i];
			}		
		}
	}
	{
		// knight
		int dx[] = {-2,-2,-1,-1,1,1,2,2};
		int dy[] = {1,-1,2,-2,2,-2,1,-1};
		for (int i=0;i<8;i++)
		{
			int tmp_c = c+dx[i];
			int tmp_r = r+dy[i];
			if (tmp_c < 0 || tmp_c >= 8 || tmp_r <= 0 || tmp_r > 8) continue;
			if ((*this)((COL)tmp_c,tmp_r).get_team() != team && (*this)((COL)tmp_c,tmp_r).get_type() == KNIGHT)
			{
				cerr << "knoight check\n";
				return true;
			}
		}

	}	
	{
		// pawns
		int dx[] = {1,-1};
		int dy;
		if (team == 0)
		{
			// black
			// white pawns go up so its (-1,-1)(1,-1)
			dy = -1;
		} else {
			// white
			// black pawns go down so its (-1,+1)(+1,+1)
			dy = 1;
		}
		for (int i=0;i<2;i++) {

			int tmp_c = c+dx[i];
			int tmp_r = r+dy;
			if (tmp_c < 0 || tmp_c >= 8 || tmp_r <= 0 || tmp_r > 8) continue;
			if ((*this)((COL)tmp_c,tmp_r).get_team() != team && (*this)((COL)tmp_c,tmp_r).get_type() == PAWN)
			{
				cerr << "pawn check\n";
				return true;
			}
		}
	}
	return false;
}
bool board::legal_move(COL f_c, int f_r, COL t_c, int t_r)
{

	if (move(f_c,f_r,t_c,t_r))
	{
		
		if ((*this)(t_c,t_r).get_type() == KING) return false;


		// see if makin gmove would put into check
		board j = (*this);
		j.move_nc(f_c,f_r,t_c,t_r);
		if (j.in_check((*this)((COL)f_c,f_r).get_team())) return false;



		past_moves.push_back(mv(make_pair(f_c,f_r),make_pair(t_c,t_r),(*this)(f_c,f_r),(*this)(t_c,t_r)));
		if ((*this)(f_c,f_r).get_type() == PAWN)
		{
			// check for promotion
			if ((*this)(f_c,f_r).get_team() == 0)
			{
				// black, so if row == 1
				if (t_r == 1)
					(*this)(f_c,f_r).change_type(QUEEN);

			} else
			{
				if (t_r == 8)
					(*this)(f_c,f_r).change_type(QUEEN);
			}
		}
		(*this)(f_c,f_r).inc_move_count();
		(*this)(t_c,t_r)=(*this)(f_c,f_r);
		(*this)(f_c,f_r) = piece();
		return true;
	}
	cerr << "BAD\n";
	return false;
}
bool board::is_legal(COL f_c, int f_r, COL t_c, int t_r, int team)
{
	// crazy
	// absolutely crazy
	// should be other way around
	// but o well this is what im dealing with
	board LOL = (*this);
	return LOL.legal_move(f_c,f_r,t_c,t_r);
}
void board::move_nc(COL f_c, int f_r, COL t_c, int t_r)
{
	(*this)(f_c,f_r).inc_move_count();
	(*this)(t_c,t_r)=(*this)(f_c,f_r);
	(*this)(f_c,f_r) = piece();
}
bool board::move(COL f_c, int f_r, COL t_c, int t_r)
{
	//oob
	if (f_c < A || f_c > H || t_c < A || t_c > H) return false;
	if (f_r < 1 || f_r > 8 || t_r < 1 || t_r > 8) return false;

	if ((*this)(t_c,t_r).get_team() == (*this)(f_c,f_r).get_team()) return false;

	cerr << "moving!\n";

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
			default:
				return false; // bushop and kngiht canont mooov straight up or duun
		}
	}
	// move has to be diagonal or knight
	cerr << "NOT STRAIGHT\n";
	// king diag
	if ((*this)(f_c,f_r).get_type() == KING)
	{
		cerr << " " << f_c << " " << f_r << " " << t_c << " " << t_r << "\n";
		return (abs(f_r-t_r) <= 1) && (abs(f_c-t_c) <= 1);
	}

	// fix this to accommodate for pawn capturing
	if (abs(f_c - t_c) == abs(f_r - t_r) && ((*this)(f_c,f_r).get_type() == BISHOP || (*this)(f_c,f_r).get_type() == QUEEN) )
	{
		// digaonal
		return verify_diagonal(f_c,f_r,t_c,t_r);
	}
	if ((*this)(f_c,f_r).get_type() == PAWN)
	{
		cerr << "pawn capture\n";
		int dy;
		if ((*this)(f_c,f_r).get_team() == 0) dy = 1;
		else dy = -1;
		if (abs(f_c-t_c) == 1 && (f_r-t_r) == dy) return (((*this)(t_c,t_r).get_team()) != ((*this)(f_c,f_r).get_team())) && ((*this)(t_c,t_r).get_team() != -1); // opposite team
		return false;
	}
	// has to be kngith i think

	if ((*this)(f_c,f_r).get_type() != KNIGHT) return false;
	cerr << "KNIGHT MOVE\n";
	int dx = abs(f_c - t_c);
	int dy = abs(f_r - t_r);
	return (dx == 2 && dy ==1) || (dx == 1 && dy == 2);
}	
pair<pair<int, int>, pair<int, int> > board::count_straight(COL c, int r)
{
	// assume that it's a rook or queen
	vector<int> vec(4,0);
	int dx[] = {0,0,1,-1};
	int dy[] = {1,-1,0,0};
	for (int i=0;i<4;i++)
	{
		int cnt = 1;
		int tmp_c = c+dx[i];
		int tmp_r = r+dy[i];
		while (tmp_c >= 0 && tmp_c < 8 && tmp_r > 0 && tmp_c <= 8)
		{
			if ((*this)((COL)tmp_c,tmp_r).is_piece())
			{
				vec[i]=cnt;
				break;
			}
			cnt++;
			tmp_c+=dx[i];
			tmp_r+=dy[i];
		}
	}
	pair<int, int> horiz,vert;
	horiz = make_pair(vec[2],vec[3]);
	vert = make_pair(vec[1],vec[0]);

	return make_pair(horiz,vert);
}
pair<pair<int, int>, pair<int, int> > board::count_diagonal(COL c, int r)
{
	vector<int> vec(4,0);
	int dx[] = {1,1,-1,-1};
	int dy[] = {1,-1,1,-1};
	for (int i=0;i<4;i++)
	{
		int cnt = 1;
		int tmp_c = c+dx[i];
		int tmp_r = r+dy[i];
		while (tmp_c >= 0 && tmp_c < 8 && tmp_r > 0 && tmp_r <= 8)
		{
			if ((*this)((COL)tmp_c,tmp_r).is_piece())
			{
				vec[i]=cnt;
				break;
			}
			cnt++;
			tmp_c+=dx[i];
			tmp_r+=dy[i];
		}
	}
	pair<int, int> left,right;
	left = make_pair(vec[2],vec[3]);
	right = make_pair(vec[0],vec[1]);

	return make_pair(left,right);

}
bool board::in_checkmate(int team)
{
	// make every goddamn move for team and see if its still checkmate
	// absoulte crying death pain 
	for (int i=0;i<8;i++)
	{
		for (int j=1;j<=8;j++)
		{
			if ((*this)((COL)i,j).get_team() == team)
			{
				// make all the moves it can 
				// im gonna be really dumb and make this move around every square LMAO LMAO lMAO
				// thats 64 * 16 -> 1024 operations
				// honestly that might be the most efficient
				//hold up
				for (int k = 0;k<8;k++)
					for (int l=1;l<=8;l++)
					{
						board tmp = (*this);
						if(tmp.legal_move((COL)i,j,(COL)k,l)) 
						{
							cerr << " move " << i << "  " << j << " -> " << k << " " << l << " TYPE: " << (*this)((COL)i,j).get_name() << " " << "\n";
							return false;
						}
					}
			}
		}
	}
	return true;
}

void test_board(board& b)
{
	b(A,5).change_type(QUEEN);
	b(A,5).set_team(1);
	b(A,1).change_type(KING);
	b(A,1).set_team(1);
	b(A,7).change_type(KING);
	b(A,7).set_team(0);
	b(D,8).change_type(KNIGHT);
	b(D,8).set_team(1);
	b(B,2).change_type(PAWN);
	b(B,2).set_team(0);
	b(B,7).change_type(QUEEN);
	b(B,7).set_team(1);
}
void init_game(board& b)
{
	for (int i=0; i<8; i++)
	{
		COL j = (COL)i;
		b(j, 2).change_type(PAWN);
		b(j, 2).set_team(1);
		b(j, 7).change_type(PAWN);
		b(j, 7).set_team(0);

	}
	// add uncool pieces
	for (int i=1; i<=8; i+=7)
	{
		b(A, i).change_type(ROOK);
		b(A, i).set_team(i&1);
		b(B, i).change_type(KNIGHT);
		b(B, i).set_team(i&1);
		b(C, i).change_type(BISHOP);
		b(C, i).set_team(i&1);
		b(D, i).change_type(QUEEN);
		b(D, i).set_team(i&1);
		b(E, i).change_type(KING);
		b(E, i).set_team(i&1);
		b(F, i).change_type(BISHOP);
		b(F, i).set_team(i&1);
		b(G, i).change_type(KNIGHT);
		b(G, i).set_team(i&1);
		b(H, i).change_type(ROOK);
		b(H, i).set_team(i&1);
	}
}
int main()
{
	if (DEBUG < 1)
	{
		// disables cerr stream
		cerr.rdbuf(NULL);
	}

	board boe;
	init_game(boe);
	boe.print_double();
	//cout << joe.eval() << "\n";
	boe.in_check(0);
	//cout << "\n\njoe";
	//cout << "\n" << joe(A,1) << "\n";
	// vector<int> m = joe(A,2).get_moves();
	// for (int i=0;i<m.size();i++) cout << i << " " << m[i] << "\n";
	int turn_num = 1;
	while(true)
	{
		if ((turn_num++) & 1)
		{
			cout << "WHITE's TURN: ";
		}
		else {
			cout << "BLACK's TURN: ";
		}
		string a,b;
		cin >> a >> b;

		boe.legal_move((COL)(a[0]-'A'),a[1]-'0',(COL)(b[0]-'A'),b[1]-'0');
		boe.print_double();
		if (boe.in_checkmate(0)) 
		{
			cout << "WHITE WINS!!!\n";
			break;
		}
		else if (boe.in_checkmate(1))
		{
			cout << "BLACK WINS!!!\n";
			break;
		}
			}
}
