#include <iostream>
#include "boardrep.hxx"
#include "moves.hxx"
#include "movegen.hxx"

using namespace std;

const Position kvw3Bb5 = { 	// Game position of Garry Kasparov vs. The World at 3.Bb5+
	{
		0xfff3080400000000,
		0x2cf308061000ef0c,
		0x7600000200200016,
		0x9900000000000099
	},
	0x1780,
	5,
	5
};

const Position kvw6Nf6 = {	// Position after 6... Nf6
	{
		0xb1fb2c0400000000,
		0x20fb08061400eb0c,
		0x3000240200240014,
		0x9108000000000099
	},
	0x8780,
	14,
	0
};

const Position kvw25Bd4 = {	// Position after 25... Bd4
	{
		0x001a0e2018000000,
		0x00320a6018806000,
		0x0008044008000040,
		0X0028000010000060
	},
	0x8000,
	50,
	3
};

void DisplayMoveList( Position&, MoveNode* );

void DisplayBoard( const Position& );

void WriteSquare( int );

int main() {
	Position game = START_POSITION;
	cout<< IsChecked( kvw3Bb5.board, 60 )<< endl;
	cout<< "Start Position: "<< endl;
	DisplayBoard( START_POSITION );
	cout<< endl<< "Possible moves: "<< endl;
	DisplayMoveList( game, GenMoves( game ) );
	cout<< endl<< "Garry Kasparov v. The World"<< endl;
	cout<< "Position after 3.Bb5+ :"<< endl;
	game = kvw3Bb5;
	DisplayBoard( game );
	cout<< endl<< "Possible moves: "<< endl;
	DisplayMoveList( game, GenMoves( game ) );
	game = kvw6Nf6;
	cout<< endl<< "Position after 6... Nf6 :"<< endl;
	DisplayBoard( game );
	cout<< endl<< "Possible moves :"<< endl;
	DisplayMoveList( game, GenMoves( game ) );
	game = kvw25Bd4;
	cout<< endl<< "Position after 25... Bd4 :"<< endl;
	DisplayBoard( game );
	cout<< endl<< "Possible moves:"<< endl;
	DisplayMoveList( game, GenMoves( game ) );
	return 0;
}

void DisplayMoveList( Position& pos, MoveNode* movelist ) {
	MoveNode* p = movelist;
	Position testpos = pos;
	while( p != NULL ) {
		MakeMove( testpos, p->move );
		DisplayBoard( testpos );
		testpos = pos;
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
