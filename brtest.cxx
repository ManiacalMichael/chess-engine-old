#include <iostream>
#include "boardrep.hxx"

using namespace std;

void DisplayBoard( const Position& );

int main() {
	Position game = START_POSITION;
	DisplayBoard( game );
	SetPiece( game.board, 4, NO_PIECE );
	DisplayBoard( game );
	return 0;
}

void DisplayBoard( const Position& pos ) {
	BoardRep board = pos.board;
	cout<< "-----------------"<< endl;
	for( int i = 8; i > 0; i-- ) {
		for( int j = 0; j < 8; j++ ) {
			cout<< " ";
			switch( GetPiece( board, ( i * 8 ) + j ) ) {
				case NO_PIECE:
					cout<< ".";
				case WHITE_PAWN:
					cout<< "P";
				case BLACK_PAWN:
					cout<< "p";
				case WHITE_KNIGHT:
					cout<< "N";
				case BLACK_KNIGHT:
					cout<< "n";
				case WHITE_BISHOP:
					cout<< "B";
				case BLACK_BISHOP:
					cout<< "b";
				case WHITE_ROOK:
					cout<< "R";
				case BLACK_ROOK:
					cout<< "r";
				case WHITE_QUEEN:
					cout<< "Q";
				case BLACK_QUEEN:
					cout<< "q";
				case WHITE_KING:
					cout<< "K";
				case BLACK_KING:
					cout<< "k";
				default:
					cout<< "%";
			}
		}
		cout<< endl;
	}
	if( pos.flags & EN_PASSANT ) 
		cout<< "En Passant available"<< endl;
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
