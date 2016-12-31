/*
 * * * * * movegen.cxx
 * Move generation
 ** Pawn Moves
 * White pawns move positive
 * Black pawns move negative
 * White pawns on rank 1 move two ranks up
 * Black pawns on rank 6 move two ranks down
 * Pawns on file 0 cannot capture left
 * Pawns on file 7 cannot capture right
 * Pawns can capture on the en passant square
 * Move generation function will handle promotions seperately
 *
 ** Knight moves
 * Knights can move:
 *  Up two ranks (+16)
 *    Right one file (+17)
 *    Left one file (+15)
 *  Down two ranks (-16)
 *    Right one file (-15)
 *    Left one file (-17)
 *  Right two files (+2)
 *    Up one rank (+10)
 *    Down one rank (-6)
 *  Left two files (-2)
 *    Up one rank (+6)
 *    Down one rank (-10)
 * Limits on knight movement:
 *  Rank 7: Cannot move up
 *  Rank 6: Cannot move two ranks up
 *  Rank 1: Cannot move two ranks down
 *  Rank 0: Cannot move down
 *  File 7: Cannot move right
 *  File 6: Cannot move two ranks right
 *  File 1: Cannot move two ranks left
 *  File 0: Cannot move left
 *
 ** General limitations on piece movement
 * Up: Destination square cannot exceed 63
 * Down: Destination square cannot exceed 0
 * Right: Destination file cannot be 0
 * Left: Destination file cannot be 7
 * Loops terminate on encountering a non-empty square
 * If the square contains an capturable piece the move is added before termination
 *
 ** Bishop moves
 * Loops until limits are exceeded or occupied square is reached
 * Move Up-Right: +9
 * Move Up-Left: +7
 * Move Down-Right: -7
 * Move Down-Left: -9
 *
 ** Rook moves
 * Loops until limits are exceeded or occupied square is reached
 * Move Up: +8
 * Move Down: -8
 * Move Right: +1
 * Move Left: -1
 *
 ** Queen moves
 * Combines moves of rook and bishop from same square
 *
 ** King moves
 * King moves to the squares in it's immediate vicinity:
 * +7  +8  +9
 * -1      +1
 * -9  -8  -7
 *
 ** Castling
 * A king and rook can castle 
 * If the relevant flags are set on the position
 * If the king is not in check on it's current square
 * If the king would not be in check on the intervening square
 * If the king would not be in check on the destination square
 *
 ** Check status
 * A king is not in check if:
 * There is no queen or bishop on the diagonals from the king
 * There is no queen or rook on the files and ranks from the king
 * There is no knight a knight's move away from the king
 * There is no pawn on the immediate diagonals from the king
 * Move generation for individual pieces can be repurposed for getting the check status
 * But extra checks have to done to make sure that only pawn captures are counted
 *  - Pass an empty BitFlags to stop e.p. captures being counted as threats
 *  - Exclude the squares one and two ranks ahead/behind the king
 *
 ** Move Generation
 * Loop and generate all the available moves for each piece of the same color
 * Invalidate any move that ends in a check for the current player
 * Generate castle moves for the player
 *  - Assume that the king is on the proper square, if it isn't the flags will not allow
 *     a castle from that player anyway
 * Loop through all moves to add special conditions to e.p. and castle,
 *  and to add another move with both special flags set for promotions
 * Loop and remove any non-root node that results in check for the player
 * Check the root node and remove if it results in check for the player
 * If there are no moves the game is over
 * If the game is over and there is no check the game is drawn (stalemate)
 */
 
#include "movegen.hxx"
#include "moves.hxx"
#include "boardrep.hxx"

Bitboard PawnMoves( Bitboard EnemyPieces, Bitboard FriendlyPieces, int square, int color, BitFlags posflags ) {
	Bitboard moves = 0x0000000000000000;
	Bitboard EmptySquares = ~( EnemyPieces | FriendlyPieces );
	int file = square % 8;
	int rank = square / 8;
	int epsquare = ( posflags & EN_PASSANT ) >> 1;
	if( color ) {
		if( ( rank == 6 ) && ( ( 1LL << ( square - 8 ) ) & EmptySquares ) )
			moves |= ( 1LL << ( square - 16 ) ) & EmptySquares;
		if( ( 63 >= ( square - 9 ) >= 0 ) && ( file != 0 ) )
			moves |= ( 1LL << ( square - 9 ) ) & EnemyPieces;
		if( ( 63 >= ( square - 7 ) >= 0 ) && ( file != 7 ) )
			moves |= ( 1LL << ( square - 7 ) ) & EnemyPieces;
		if( 63 >= ( square - 8 ) >= 0 )
			moves |= ( 1LL << ( square - 8 ) ) & EmptySquares;
		if( posflags & EN_PASSANT ) {
			if( ( ( square - 9 ) == epsquare ) && ( file != 0 ) )
				moves |= ( 1LL << ( square - 9 ) );
			if( ( ( square - 7 ) == epsquare ) && ( file != 7 ) )
				moves |= ( 1LL << ( square - 7 ) );
		}
	}
	else {
		if( ( rank == 1 ) && ( ( 1LL << ( square + 8 ) ) & EmptySquares ) )
			moves |= ( 1LL << ( square + 16 ) ) & EmptySquares;
		if( ( 63 >= ( square + 9 ) >= 0 ) && ( file != 7 ) )
			moves |= ( 1LL << ( square + 9 ) ) & EnemyPieces;
		if( ( 63 >= ( square + 7 ) >= 0 ) && ( file != 0 ) )
			moves |= ( 1LL << ( square + 7 ) ) & EnemyPieces;
		if( 63 >= ( square + 8 ) >= 0 )
			moves |= ( 1LL << ( square + 8 ) ) & EmptySquares;
		if( posflags & EN_PASSANT ) {
			if( ( ( square + 9 ) == epsquare ) && ( file != 7 ) )
				moves |= ( 1LL << ( square + 9 ) );
			if( ( ( square + 7 ) == epsquare ) && ( file != 0 ) )
				moves |= ( 1LL << ( square + 7 ) );
		}
	}
	return moves;
}

Bitboard KnightMoves( Bitboard EnemyPieces, Bitboard FriendlyPieces, int square ) {
	Bitboard moves = 0x0000000000000000;
	Bitboard Squares = ~FriendlyPieces;
	int rank = square / 8;
	int file = square % 8;
	if( rank < 6 ) { // Go up
		if( file != 7 ) // Go right
			moves |= ( 1LL << ( square + 17 ) ) & Squares;
		if( file != 0 )
			moves |= ( 1LL << ( square + 15 ) ) & Squares;
	}
	if( rank > 1 ) { // Go Down
		if( file != 7 )
			moves |= ( 1LL << ( square - 15 ) ) & Squares;
		if( file != 0 )
			moves |= ( 1LL << ( square - 17 ) ) & Squares;
	}
	if( file < 6 ) { // Go Right
		if( rank != 7 ) // Go Up
			moves |= ( 1LL << ( square + 10 ) ) & Squares;
		if( rank != 0 )
			moves |= ( 1LL << ( square - 6 ) ) & Squares;
	}
	if( file > 1 ) { // Go Left
		if( rank != 7 ) // Go Up
			moves |= ( 1LL << ( square + 6 ) ) & Squares;
		if( rank != 0 )
			moves |= ( 1LL << ( square - 10 ) ) & Squares;
	}
	return moves;
}

Bitboard RookMoves( Bitboard EnemyPieces, Bitboard FriendlyPieces, signed int square ) {
	signed int x = square + 8;
	Bitboard moves = 0x0000000000000000;
	Bitboard EmptySquares = ~( FriendlyPieces | EnemyPieces );
	Bitboard OccupiedSquares = ~EmptySquares;
	while( 63 >= x ) {
		if( OccupiedSquares & ( 1LL << x ) ) {
			if( FriendlyPieces & ( 1LL << x ) ) 
				break;
			else {
				moves |= ( 1LL << x );
				break;
			}
		}
		moves |= ( 1LL << x );
		x += 8;
	}
	x = square - 8;
	while( x >= 0 ) {
		if( OccupiedSquares & ( 1LL << x ) ) {
			if( FriendlyPieces & ( 1LL << x ) )
				break;
			else {
				moves |= ( 1LL << x );
				break;
			}
		}
		moves |= ( 1LL << x );
		x -= 8;
	}
	x = square + 1;
	while( ( x % 8 ) != 0 ) {
		if( OccupiedSquares & ( 1LL << x ) ) {
			if( FriendlyPieces & ( 1LL << x ) )
				break;
			else {
				moves |= ( 1LL << x );
				break;
			}
		}
		moves |= ( 1LL << x );
		x++;
	}
	x = square - 1;
	while( ( x % 8 ) != 7 ) {
		if( OccupiedSquares & ( 1LL << x ) ) {
			if( FriendlyPieces & ( 1LL << x ) )
				break;
			else {
				moves |= ( 1LL << x );
				break;
			}
		}
		moves |= ( 1LL << x );
		x--;
	}
	return moves;
}

Bitboard BishopMoves( Bitboard FriendlyPieces, Bitboard EnemyPieces, signed int square ) {
	Bitboard moves = 0x0000000000000000;
	Bitboard OccupiedSquares = FriendlyPieces | EnemyPieces;
	signed int x = square + 9;
	while( ( 63 >= x ) && ( x % 8 ) != 0 ) {
		if( OccupiedSquares & ( 1LL << x ) ) {
			if( FriendlyPieces & ( 1LL << x ) )
				break;
			else {
				moves |= ( 1LL << x );
				break;
			}
		}
		moves |= ( 1LL << x );
		x += 9;
	}
	x = square - 7;
	while( ( x >= 0 ) && ( x % 8 ) != 0 ) {
		if( OccupiedSquares & ( 1LL << x ) ) {
			if( FriendlyPieces & ( 1LL << x ) )
				break;
			else {
				moves |= ( 1LL << x );
				break;
			}
		}
		moves |= ( 1LL << x );
		x -= 7;
	}
	x = square + 7;
	while( ( 63 >= x ) && ( x % 8 ) != 7 ) {
		if( OccupiedSquares & ( 1LL << x ) ) {
			if( FriendlyPieces & ( 1LL << x ) )
				break;
			else {
				moves |= ( 1LL << x );
				break;
			}
		}
		moves |= ( 1LL << x );
		x += 7;
	}
	x = square - 9;
	while( ( x >= 0 ) && ( x % 8 ) != 7 ) {
		if( OccupiedSquares & ( 1LL << x ) ) {
			if( FriendlyPieces & ( 1LL << x ) )
				break;
			else {
				moves |= ( 1LL << x );
				break;
			}
		}
		moves |= ( 1LL << x );
		x -= 9;
	}
	return moves;
}

Bitboard QueenMoves( Bitboard FriendlyPieces, Bitboard EnemyPieces, signed int square ) {
	Bitboard moves = 0x0000000000000000;
	moves |= RookMoves( FriendlyPieces, EnemyPieces, square );
	moves |= BishopMoves( FriendlyPieces, EnemyPieces, square );
	return moves;
}

Bitboard KingMoves( Bitboard FriendlyPieces, Bitboard EnemyPieces, signed int square ) {
	Bitboard moves = 0x0000000000000000;
	Bitboard OccupiedSquares = FriendlyPieces | EnemyPieces;
	signed int x = square + 1;
	if( ( 63 >= x ) && ( ( x % 8 ) != 0 ) ) {
		if( OccupiedSquares & ( 1LL << x ) ) {
			if( EnemyPieces & ( 1LL << x ) )
				moves |= ( 1LL << x );
		}
		else 
			moves |= ( 1LL << x );
	}
	x = square - 1;
	if( ( x >= 0 ) && ( ( x % 8 ) != 7 ) ) {
		if( OccupiedSquares & ( 1LL << x ) ) {
			if( EnemyPieces & ( 1LL << x ) )
				moves |= ( 1LL << x );
		}
		else 
			moves |= ( 1LL << x );
	}
	x = square + 7;
	if( ( 63 >= x ) && ( ( x % 8 ) != 7 ) ) {
		if( OccupiedSquares & ( 1LL << x ) ) {
			if( EnemyPieces & ( 1LL << x ) )
				moves |= ( 1LL << x );
		}
		else 
			moves |= ( 1LL << x );
	}
	x = square + 8;
	if( 63 >= x ) {
		if( OccupiedSquares & ( 1LL << x ) ) {
			if( EnemyPieces & ( 1LL << x ) )
				moves |= ( 1LL << x );
		}
		else
			moves |= ( 1LL << x );
	}
	x = square + 9;
	if( ( 63 >= x ) && ( ( x % 8 ) != 0 ) ) {
		if( OccupiedSquares & ( 1LL << x ) ) {
			if( EnemyPieces & ( 1LL << x ) )
				moves |= ( 1LL << x );
		}
		else
			moves |= ( 1LL << x );
	}
	x = square - 7;
	if( ( x >= 0 ) && ( ( x % 8 ) != 0 ) ) {
		if( OccupiedSquares & ( 1LL << x ) ) {
			if( EnemyPieces & ( 1LL << x ) )
				moves |= ( 1LL << x );
		}
		else
			moves |= ( 1LL << x );
	}
	x = square - 8;
	if( x >= 0 ) {
		if( OccupiedSquares & ( 1LL << x ) ) {
			if( EnemyPieces & ( 1LL << x ) )
				moves |= ( 1LL << x );
		}
		else
			moves |= ( 1LL << x );
	}
	x = square - 9;
	if( ( x >= 0 ) && ( ( x % 8 ) != 7 ) ) {
		if( OccupiedSquares & ( 1LL << x ) ) {
			if( EnemyPieces & ( 1LL << x ) )
				moves |= ( 1LL << x );
		}
		else
			moves |= ( 1LL << x );
	}
	return moves;
}

bool IsChecked( const BoardRep& board, signed int square ) {
	Bitboard OccupiedSquares, EnemyKing, EnemyDiagonals, EnemyStraights, EnemyPawns, EnemyKnights, EnemyPieces, PawnExclusions, FriendlyPieces = 0x0000000000000000;
	OccupiedSquares = board.layer1 | board.layer2 | board.layer3;
	EnemyDiagonals |= board.layer1 & board.layer2 & ( ~board.layer3 ); // Bishops of both colors
	EnemyDiagonals |= board.layer1 & ( ~board.layer2 ) & board.layer3; // Queens
	EnemyStraights |= board.layer1 & ( ~board.layer2 ) & board.layer3;
	EnemyStraights |= ( ~board.layer1 ) & ( ~board.layer2 ) & board.layer3; //Rooks
	EnemyPawns |= board.layer1 & ( ~board.layer2 ) & ( ~board.layer3 );
	EnemyKnights |= ( ~board.layer1 ) & board.layer2 & ( ~board.layer3 );
	EnemyKing |= ( ~board.layer1 ) & board.layer2 & board.layer3;
	if( board.layer0 & ( 1LL << square ) ) {
		EnemyPieces = ( ~board.layer0 ) & OccupiedSquares;
		PawnExclusions = ~( 1LL << ( square - 8 ) ) & ( 1LL << ( square - 16 ) );
	}
	else {
		EnemyPieces = board.layer0;
		PawnExclusions = ~( 1LL << ( square + 8 ) ) & ( 1LL << ( square + 16 ) );
	}
	EnemyDiagonals &= EnemyPieces;
	EnemyStraights &= EnemyPieces;
	EnemyPawns &= EnemyPieces;
	EnemyKnights &= EnemyPieces;
	EnemyKing &= EnemyPieces;
	FriendlyPieces = ( ~EnemyPieces ) & OccupiedSquares;
	if( EnemyDiagonals & ( BishopMoves( EnemyPieces, FriendlyPieces, square ) | QueenMoves( EnemyPieces, FriendlyPieces, square ) ) )
		return true;
	if( EnemyStraights & ( BishopMoves( EnemyPieces, FriendlyPieces, square ) | QueenMoves( EnemyPieces, FriendlyPieces, square ) ) )
		return true;
	if( EnemyPawns & PawnMoves( EnemyPieces, FriendlyPieces, square, board.layer0 & ( 1LL << square ), 0x0000 ) & PawnExclusions )
		return true;
	if( EnemyKnights & KnightMoves( EnemyPieces, FriendlyPieces, square ) )
		return true;
	if( EnemyKing & KingMoves( EnemyPieces, FriendlyPieces, square ) )
		return true;
	return false;
}

Bitboard CastleMoves( const BoardRep& board, signed int square, BitFlags posflags ) {
	Bitboard moves = 0x0000000000000000;
	Bitboard EmptySquares = ~board.layer0 | board.layer1 | board.layer2 | board.layer3;
	int color = board.layer0 & ( 1LL << square );
	if( posflags & ( WHITE_CHECK | BLACK_CHECK ) )
		return moves;
	if( color ) {
		if( posflags & BLACK_KINGSIDE_CASTLE ) {
			if( ( EmptySquares & ( 1LL << ( square + 1 ) ) ) && ( EmptySquares & ( 1LL << ( square + 2 ) ) ) ) {
				if( ! IsChecked( board, square + 1 ) ) {
					if( ! IsChecked( board, square + 2 ) )
						moves |= ( 1LL << square + 2 );
				}
			}
		}
		if( posflags & BLACK_QUEENSIDE_CASTLE ) {
			if( ( EmptySquares & ( 1LL << ( square - 1 ) ) ) && ( EmptySquares & ( 1LL << ( square - 2 ) ) ) && ( EmptySquares & ( 1LL << ( square - 3 ) ) ) ) {
				if( ! IsChecked( board, square - 1 ) ) {
					if( ! IsChecked( board, square - 2 ) ) 
						moves |= ( 1LL << square - 2 );
				}
			}
		}
	}
	else {
		if( posflags & WHITE_KINGSIDE_CASTLE ) {
			if( ( EmptySquares & ( 1LL << ( square + 1 ) ) ) && ( EmptySquares & ( 1LL << ( square + 2 ) ) ) ) {
				if( ! IsChecked( board, square + 1 ) ) {
					if( ! IsChecked( board, square + 2 ) )
						moves |= ( 1LL << square + 2 );
				}
			}
		}
		if( posflags & WHITE_QUEENSIDE_CASTLE ) {
			if( ( EmptySquares & ( 1LL << ( square - 1 ) ) ) && ( EmptySquares & ( 1LL << ( square - 2 ) ) ) && ( EmptySquares & ( 1LL << ( square - 3 ) ) ) ) {
				if( ! IsChecked( board, square - 1 ) ) {
					if( ! IsChecked( board, square - 2 ) ) 
						moves |= ( 1LL << square - 2 );
				}
			}
		}
	}
	return moves;
}

MoveNode* GenMoves( Position& pos ) {
	MoveNode* movelist = new MoveNode;
	MoveNode* p = movelist;
	MoveNode* q = p;
	const BoardRep board = pos.board;
	const BitFlags posflags = pos.flags;
	int piece, type, color;
	Position testpos = pos;
	Bitboard EnemyKing = ( ~board.layer1 ) | board.layer2 | board.layer3;
	Bitboard EnemyPieces, FriendlyPieces;
	Bitboard OccupiedSquares = board.layer0 | board.layer1 | board.layer2 | board.layer3;
	BitFlags FriendlyCheck;
	if( pos.flags & WHITE_TO_MOVE )
		color = 0;
	else
		color = 1;
	if( color ) {
		EnemyPieces = OccupiedSquares & ( ~board.layer0 );
		FriendlyCheck = BLACK_CHECK;
	}
	else {
		EnemyPieces = OccupiedSquares & board.layer0;
		FriendlyCheck = WHITE_CHECK;
	}
	EnemyKing &= EnemyPieces;
	FriendlyPieces = OccupiedSquares & ( ~EnemyPieces );
	FriendlyPieces |= EnemyKing;	// Prevents pieces from capturing a king
	for( int i = 0; i < 64; i++ ) {
		piece = GetPiece( board, i );
		type = piece / 2;
		if( ( piece % 2 ) == color ) {
			if( type == 1 )
				AddMoves( p, PawnMoves( EnemyPieces, FriendlyPieces, i, color, posflags ), i );
			else if( type == 2 )
				AddMoves( p, KnightMoves( EnemyPieces, FriendlyPieces, i ), i );
			else if( type == 3 )
				AddMoves( p, BishopMoves( EnemyPieces, FriendlyPieces, i ), i );
			else if( type == 4 )
				AddMoves( p, RookMoves( EnemyPieces, FriendlyPieces, i ), i );
			else if( type == 5 )
				AddMoves( p, QueenMoves( EnemyPieces, FriendlyPieces, i ), i );
			else if( type == 6 )
				AddMoves( p, KingMoves( EnemyPieces, FriendlyPieces, i ), i );
		}
	}
	if( color ) 
		AddMoves( p, CastleMoves( board, 60, posflags ), 60 );
	else
		AddMoves( p, CastleMoves( board, 4, posflags ), 4 );
	p = movelist;
	while( p != NULL ) {
		if( ( GetPiece( board, ( p->move ) & START_SQUARE_MASK ) / 2 ) == 1 ) {	// If this is a pawn...
			if( color ) {
				if( ( ( ( p->move & END_SQUARE_MASK ) >> 6 ) / 8 ) == 0 ) { // If this is a black pawn moving to the 0th rank...
					q = new MoveNode;
					q->move = p->move | PROMOTION_TO_KNIGHT;
					p->move |= PROMOTION_TO_QUEEN;
					q->nxt = p->nxt;
					p->nxt = q;
				}
			}
			else {
				if( ( ( ( p->move & END_SQUARE_MASK ) >> 6 ) / 8 ) == 7 ) { // If this is a white pawn moving to the 7th rank...
					q = new MoveNode;
					q->move = p->move | PROMOTION_TO_KNIGHT;
					p->move |= PROMOTION_TO_QUEEN;
					q->nxt = p->nxt;
					p->nxt = q;
				}
			}
			if( ( ( p->move & END_SQUARE_MASK ) >> 6 ) == ( posflags & EP_SQUARE_MASK ) ) // If this an ep capture... 
				p->move |= EP_CAPTURE;
		}
		else if( ( GetPiece( board, p->move & START_SQUARE_MASK ) / 2 ) == 6 ) { // If this is a king moving two spaces to either side...
			if( ( ( p->move & END_SQUARE_MASK ) >> 6 ) == ( p->move & START_SQUARE_MASK ) + 2 ) 
				p->move |= CASTLE_MOVE;
			else if( ( ( p->move & END_SQUARE_MASK ) >> 6 ) == ( p->move & START_SQUARE_MASK ) - 2 )
				p->move |= CASTLE_MOVE;
		}
		p = p->nxt;
	}
	p = movelist;
	while( p->nxt != NULL ) {
		q = p->nxt;
		MakeMove( testpos, q->move );
		if( testpos.flags & FriendlyCheck )
			RemoveNextNode( p );
		testpos = pos;
		p = p->nxt;
	}
	testpos = pos;
	p = movelist;
	MakeMove( testpos, movelist->move );
	if( testpos.flags & FriendlyCheck ) {
		movelist = movelist->nxt;
		delete p;
	}
	if( movelist == NULL ) {
		pos.flags |= GAME_OVER;
		if( !( pos.flags & ( WHITE_CHECK | BLACK_CHECK ) ) )
			pos.flags |= GAME_DRAWN;
	}
	return movelist;
}
