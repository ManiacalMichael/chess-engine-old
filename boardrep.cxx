#include "boardrep.hxx"

void SetPiece( BoardRep& board, int square, int piece ) {
	board.layer0 &= ~( 1LL << square );
	board.layer1 &= ~( 1LL << square );
	board.layer2 &= ~( 1LL << square );
	board.layer3 &= ~( 1LL << square );
	if( piece & 1 )
		board.layer0 |= 1LL << square;
	if( piece & 2 )
		board.layer1 |= 1LL << square;
	if( piece & 4 )
		board.layer2 |= 1LL << square;
	if( piece & 8 )
		board.layer3 |= 1LL << square;
	return;
}

int GetPiece( const BoardRep& board, int square ) {
	int i = 0;
	if( board.layer0 & ( 1LL << square ) )
		i += 1;
	if( board.layer1 & ( 1LL << square ) )
		i += 2;
	if( board.layer2 & ( 1LL << square ) )
		i += 4;
	if( board.layer3 & ( 1LL << square ) )
		i += 8;
	return i;
}
