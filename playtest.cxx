// Temporary interface for testing
#include "boardrep.hxx"
#include "movegen.hxx"
#include "moves.hxx"
#include "search.hxx"
#include <iostream>
#include <string>

using namespace std;

MoveRep ReadMove( Position& );

void DisplayBoard( const Position& );

void WriteSquare( int );

string WriteNotation( MoveRep, Position& );

MoveRep ParseNotation( Position& );

int main() {
	int depth, moves;
	Position game = START_POSITION;
	MoveRep ComputerMove;
	string c = "\0";
	bool PlayerWhite;
	bool ComputerPlays = false;
	cout<< "Depth = "<< flush;
	cin>> depth;
	cout<< "Depth set at "<< dec<< depth<< endl;
	moves = 1;
	while( c == "\0" ) {
		cout<< "Player takes (w/b/n): "<< flush;
		cin>> c;
		switch( c[0] ) {
			case 'w':
				PlayerWhite = true;
				break;
			case 'W':
				PlayerWhite = true;
				break;
			case 'b':
				PlayerWhite = false;
				break;
			case 'B':
				PlayerWhite = false;
				break;
			case 'n':
				ComputerPlays = true;
				break;
			case 'N':
				ComputerPlays = true;
				break;
			default:
				c = "\0";
		}
	}
	DisplayBoard( game );
	if( ComputerPlays ) {
		while( !( game.flags & GAME_OVER ) ) {
			cout<< moves<< ". ";
			ComputerMove = Search( game, depth );
			cout<< WriteNotation( ComputerMove, game );
			cout<< endl;
			MakeMove( game, ComputerMove );
			DisplayBoard( game );
			cout<< moves<< "... ";
			ComputerMove = Search( game, depth );
			cout<< WriteNotation( ComputerMove, game );
			cout<< endl;
			MakeMove( game, ComputerMove );
			DisplayBoard( game );
			moves++;
		}
	}
	while( !( game.flags & GAME_OVER ) ) {
		if( PlayerWhite ) {
			cout<< "Your move, "<< moves<< ". "<< flush;
			MakeMove( game, ParseNotation( game ) );
			DisplayBoard( game );
			cout<< "Computer's move, "<< moves<< "... ";
			ComputerMove = Search( game, depth );
			cout<< WriteNotation( ComputerMove, game );
			cout<< endl;
			MakeMove( game, ComputerMove );
			DisplayBoard( game );
			moves++;
		}
		else {
			cout<< "Computer's move, "<< moves<< ". ";
			ComputerMove = Search( game, depth );
			cout<< WriteNotation( ComputerMove, game );
			cout<< endl;
			MakeMove( game, ComputerMove );
			DisplayBoard( game );
			cout<< "Your move, "<< moves<< "... "<< flush;
			MakeMove( game, ParseNotation( game ) );
			DisplayBoard( game );
			moves++;
		}
	}
	cout<< "Game Over"<< endl;
	return 0;
}

void DisplayBoard( const Position& pos ) {
	BoardRep board = pos.board;
	cout<< "-----------------"<< endl;
	for( int i = 8; i > 0; i-- ) {
		for( int j = 0; j < 8; j++ ) {
			if( j == 0 )
				cout<< "|";
			else 
				cout<< " ";
			cout<< PieceDisplay[ GetPiece( board, ( ( i - 1 ) * 8 ) + j ) ];
			if( j == 7 )
				cout<< "|";
		}
		cout<< endl;
	}
	cout<< "----------------"<< endl;
	if( pos.flags & EN_PASSANT ) { 
		cout<< "En Passant available on: ";
		WriteSquare( ( pos.flags & EP_SQUARE_MASK ) >> 1 );
		cout<< endl;
	}
	if( pos.flags & 0x0780 ) {
		if( pos.flags & WHITE_KINGSIDE_CASTLE )
			cout<< "W";
		else
			cout<< "-";
		if( pos.flags & WHITE_QUEENSIDE_CASTLE )
			cout<< "w";
		else
			cout<< "-";
		if( pos.flags & BLACK_KINGSIDE_CASTLE )
			cout<< "B";
		else
			cout<< "-";
		if( pos.flags & BLACK_QUEENSIDE_CASTLE )
			cout<< "b";
		else
			cout<< "-";
		cout<< endl;
	}
	if( pos.flags & WHITE_CHECK ) {
		if( pos.flags & GAME_OVER )
			cout<< "White lost"<< endl;
		else
			cout<< "White is in check"<< endl;
	}
	else if( pos.flags & BLACK_CHECK ) {
		if( pos.flags & GAME_OVER )
			cout<< "Black lost"<< endl;
		else
			cout<< "Black is in check"<< endl;
	}
	if( pos.flags & GAME_DRAWN )
		cout<< "Game is drawn"<< endl;
}

void WriteSquare( int square ) {
	const char File[ 8 ] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H' };
	cout<< File[ ( square % 8 ) ]<< ( ( square / 8 ) + 1 );
}

MoveRep ReadMove( Position& pos ) {
	char srank = '\0';
	char sfile = '\0';
	char erank = '\0';
	char efile = '\0';
	char promo = '\0';
	MoveRep ret = 0x0000;
	while( ( srank == '\0' ) || ( sfile == '\0' ) || ( erank == '\0' ) || ( efile == '\0' ) ) {
		cin>> sfile;
		cin>> srank;
		cin>> efile;
		cin>> erank;
		switch( srank ) {
			case '1':
				break;
			case '2':
				ret |= 8;
				break;
			case '3':
				ret |= 16;
				break;
			case '4':
				ret |= 24;
				break;
			case '5':
				ret |= 32;
				break;
			case '6':
				ret |= 40;
				break;
			case '7':
				ret |= 48;
				break;
			case '8':
				ret |= 56;
				break;
			default:
				cout<< "ERROR"<< endl;
				srank = '\0';
				continue;
				break;
		}
		switch( sfile ) {
			case 'A':
				ret |= 0;
				break;
			case 'B':
				ret |= 1;
				break;
			case 'C':
				ret |= 2;
				break;
			case 'D':
				ret |= 3;
				break;
			case 'E':
				ret |= 4;
				break;
			case 'F':
				ret |= 5;
				break;
			case 'G':
				ret |= 6;
				break;
			case 'H':
				ret |= 7;
				break;
			default:
				cout<< "ERROR"<< endl;
				sfile = '\0';
				continue;
				break;
		}
		switch( erank ) {
			case '1':
				break;
			case '2':
				ret |= ( 8 << 6 );
				break;
			case '3':
				ret |= ( 16 << 6 );
				break;
			case '4':
				ret |= ( 24 << 6 );
				break;
			case '5':
				ret |= ( 32 << 6 );
				break;
			case '6':
				ret |= ( 40 << 6 );
				break;
			case '7':
				ret |= ( 48 << 6 );
				break;
			case '8':
				ret |= ( 56 << 6 );
				break;
			default:
				cout<< "ERROR"<< endl;
				erank = '\0';
				continue;
				break;
		}
		switch( efile ) {
			case 'A':
				break;
			case 'B':
				ret |= ( 1 << 6 );
				break;
			case 'C':
				ret |= ( 2 << 6 );
				break;
			case 'D':
				ret |= ( 3 << 6 );
				break;
			case 'E':
				ret |= ( 4 << 6 );
				break;
			case 'F':
				ret |= ( 5 << 6 );
				break;
			case 'G':
				ret |= ( 6 << 6 );
				break;
			case 'H':
				ret |= ( 7 << 6 );
				break;
			default:
				cout<< "ERROR"<< endl;
				efile = '\0';
				continue;
				break;
		}
	}
	if( ( GetPiece( pos.board, ( ret & START_SQUARE_MASK ) ) / 2 ) == 1 ) {	// If this is a pawn...
		if( !( pos.flags & WHITE_TO_MOVE ) ) {
			if( ( ( ( ret & END_SQUARE_MASK ) >> 6 ) / 8 ) == 0 ) { // If this is a black pawn moving to the 0th rank...
				while( promo == '\0' ) {
					cout<< "Promotion (q/n): "<< flush;
					cin>> promo;
					switch( promo ) {
						case 'q':
							ret |= PROMOTION_TO_QUEEN;
							break;
						case 'Q':
							ret |= PROMOTION_TO_QUEEN;
							break;
						case 'n':
							ret |= PROMOTION_TO_KNIGHT;
							break;
						case 'N':
							ret |= PROMOTION_TO_KNIGHT;
							break;
						default:
							cout<< "ERROR"<< endl;
							promo = '\0';
							break;
					}
				}
			}
		}
		else {
			if( ( ( ( ret & END_SQUARE_MASK ) >> 6 ) / 8 ) == 7 ) { // If this is a white pawn moving to the 7th rank...
				while( promo == '\0' ) {
					cout<< "Promotion (q/n): "<< flush;
					cin>> promo;
					switch( promo ) {
						case 'q':
							ret |= PROMOTION_TO_QUEEN;
							break;
						case 'Q':
							ret |= PROMOTION_TO_QUEEN;
							break;
						case 'n':
							ret |= PROMOTION_TO_KNIGHT;
							break;
						case 'N':
							ret |= PROMOTION_TO_KNIGHT;
							break;
						default:
							cout<< "ERROR"<< endl;
							promo = '\0';
							break;
					}
				}
			}
		}
		if( ( ( ret & END_SQUARE_MASK ) >> 6 ) == ( pos.flags & EP_SQUARE_MASK ) ) // If this an ep capture... 
			ret |= EP_CAPTURE;
	}
	else if( ( GetPiece( pos.board, ( ret & START_SQUARE_MASK ) ) / 2 ) == 6 ) { // If this is a king moving two spaces to either side...
		if( ( ( ret & END_SQUARE_MASK ) >> 6 ) == ( ret & START_SQUARE_MASK ) + 2 ) 
			ret |= CASTLE_MOVE;
		else if( ( ( ret & END_SQUARE_MASK ) >> 6 ) == ( ret & START_SQUARE_MASK ) - 2 )
			ret |= CASTLE_MOVE;
	}
	return ret;
}

string WriteNotation( MoveRep move, Position& pos ) {
	MoveNode* movelist = GenMoves( pos );
	MoveNode* p = movelist;
	MoveNode* conflicts = new MoveNode;
	MoveNode* q = conflicts;
	Position testpos = pos;
	BoardRep board = pos.board;
	int piece = GetPiece( board, ( move & START_SQUARE_MASK ) );
	int start = move & START_SQUARE_MASK;
	int end = ( move & END_SQUARE_MASK ) >> 6;
	int count = 0;
	const char File[ 8 ] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };
	string ret;
	switch( GetPiece( board, start ) / 2 ) {
		case 1:
			if( GetPiece( board, end ) )
				ret += File[ start % 8 ];
			break;
		case 2:
			ret += 'N';
			break;
		case 3:
			ret += 'B';
			break;
		case 4:
			ret += 'R';
			break;
		case 5:
			ret += 'Q';
			break;
		case 6:
			ret += 'K';
			break;
	}
	if( ( ( GetPiece( board, start ) / 2 ) == 1 ) && ( GetPiece( board, end ) ) )
		count = 0;
	else {
		while( p != NULL ) {
			if( GetPiece( board, p->move & START_SQUARE_MASK ) == piece ) {
				if( ( ( p->move & END_SQUARE_MASK ) >> 6 ) == end ) {
					q->nxt = new MoveNode;
					q->move = p->move;
					q = q->nxt;
					count++;
				}
			}
			p = p->nxt;
		}
		count--;
	}
	q = conflicts;
	while( count ) {	// For loop termination syntax
		while( q->nxt != NULL ) {
			if( ( ( q->move & START_SQUARE_MASK ) % 8 ) != ( start % 8 ) ) {
				count--;
			}
			q = q->nxt;
		}
		if( !count ) {	// Adding file fully disambaugates move
			ret += File[ start % 8 ];
			break;
		}
		q = conflicts;
		count = 0;
		while( q->nxt != NULL ) {
			count++;
			q = q->nxt;
		}
		count--;
		q = conflicts;
		while( q->nxt != NULL ) {
			if( ( ( q->move & END_SQUARE_MASK ) / 8 ) != ( start / 8 ) ) {
				count--;
			}
			q = q->nxt;
		}
		if( !count ) {
			ret += ( ( start / 8 ) + 49 );	// ASCII
			break;
		}
		else {
			ret += File[ start % 8 ];
			ret += ( ( start / 8 ) + 49 );
		}
		DeleteMoveList( conflicts );
	}
	if( GetPiece( board, end ) )
		ret += 'x';
	ret += File[ end % 8 ];
	ret += ( ( end / 8 ) + 49 );	// ASCII
	if( move & PROMOTION_TO_KNIGHT )
		ret += "=N";
	else if( move & PROMOTION_TO_QUEEN )
		ret += "=Q";
	if( move & CASTLE_MOVE ) {
		if( end > start )
			ret = "O-O";
		else
			ret = "O-O-O";
	}
	MakeMove( testpos, move );
	if( testpos.flags & WHITE_CHECK ) {
		if( testpos.flags & GAME_OVER ) {
			if( !( testpos.flags & GAME_DRAWN ) )
				ret += '#';
		}
		ret += '+';
	}
	if( testpos.flags & BLACK_CHECK ) {
		if( testpos.flags & GAME_OVER ) {
			if( !( testpos.flags & GAME_DRAWN ) )
				ret += '#';
		}
		ret += '+';
	}
	DeleteMoveList( movelist );
	return ret;
}

MoveRep ParseNotation( Position& pos ) {
	MoveNode* movelist = GenMoves( pos );
	MoveNode* p = movelist;
	string s;
	while( true ) {
		cin>> s;
		while( p != NULL ) {
			cout<< WriteNotation( p->move, pos )<< endl;
			if( s == WriteNotation( p->move, pos ) )
				return p->move;
			p = p->nxt;
		}
		cout<< "Error: move not found in list, possibly ambiguous"<< endl;
		cout<< "Try again: ";
		p = movelist;
	}
}
