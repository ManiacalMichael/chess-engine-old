#include <iostream>
#include "boardrep.hxx"
#include "moves.hxx"
#include "movegen.hxx"

using namespace std;

void DisplayMoveList( MoveNode* );

void DisplayBoard( const Position& );

void WriteSquare( int );

int main() {
	Position game = START_POSITION;
	MoveNode* movelist = GenMoves( game );
	DisplayMoveList( movelist );
	return 0;
}

void DisplayMoveList( MoveNode* movelist ) {
	MoveNode* p = movelist;
	Position testpos = START_POSITION;
	while( p != NULL ) {
		MakeMove( testpos, p->move );
		DisplayBoard( testpos );
		testpos = START_POSITION;
		p = p->nxt;
	}
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
	if( pos.flags & WHITE_TO_MOVE )
		cout<< "White's move"<< endl;
	else
		cout<< "Black's move"<< endl;
}

void WriteSquare( int square ) {
	const char File[ 8 ] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H' };
	cout<< File[ ( square % 8 ) ]<< ( ( square / 8 ) + 1 );
}
