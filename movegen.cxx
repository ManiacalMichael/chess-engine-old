Bitboard PawnMoves( Bitboard EnemyPieces, Bitboard FriendlyPieces, int square, int color, BitFlags posflags ) {
	Bitboard moves = 0x0000000000000000;
	Bitboard EmptySquares = ~( EnemyPieces | FreindlyPieces );
	int epsquare = ( posflags & EP_MASK ) >> 1;
	if( color ) {
		if( ( ( square / 8 ) == 6 ) && ( ( 1 << ( square - 8 ) ) & EmptySquares ) )
			moves |= ( 1 << ( square - 16 ) ) & EmptySquares;
		if( ( 63 >= ( square - 9 ) >= 0 ) && ( ( square % 8 ) != 0 ) )
			moves |= ( 1 << ( square - 9 ) ) & EnemyPieces;
		if( ( 63 >= ( square - 7 ) >= 0 ) && ( ( square % 8 ) != 7 ) )
			moves |= ( 1 << ( square - 7 ) ) & EnemyPieces;
		if( 63 >= ( square - 8 ) >= 0 )
			moves |= ( 1 << ( square - 8 ) ) & EmptySquares;
		if( posflags & EN_PASSANT ) {
			if( ( ( square - 9 ) == epsquare ) && ( ( square % 8 ) != 0 ) )
				moves |= ( 1 << ( square - 9 ) );
			if( ( ( square - 7 ) == epsquare ) && ( ( square % 8 ) != 7 ) )
				moves |= ( 1 << ( square - 7 ) );
		}
	}
	else {
		if( ( ( square / 8 ) == 1 ) && ( ( 1 << ( square + 8 ) ) & EmptySquares ) )
			moves |= ( 1 << ( square + 16 ) ) & EmptySquares;
		if( ( 63 >= ( square + 9 ) >= 0 ) && ( ( square % 8 ) != 7 ) )
			moves |= ( 1 << ( square + 9 ) ) & EnemyPieces;
		if( ( 63 >= ( square + 7 ) >= 0 ) && ( ( square % 8 ) != 0 ) )
			moves |= ( 1 << ( square + 7 ) ) & EnemyPieces;
		if( 63 >= ( square + 8 ) >= 0 )
			moves |= ( 1 << ( square + 8 ) ) & EmptySquares;
		if( posflags & EN_PASSANT ) {
			if( ( ( square + 9 ) == epsquare ) && ( ( square % 8 ) != 7 ) )
				moves |= ( 1 << ( square + 9 ) );
			if( ( ( square + 7 ) == epsquare ) && ( ( square % 8 ) != 0 ) )
				moves |= ( 1 << ( square + 7 ) );
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
			moves |= ( 1 << ( square + 17 ) ) & Squares;
		if( file != 0 )
			moves |= ( 1 << ( square + 15 ) ) & Squares;
	}
	if( rank > 1 ) { // Go Down
		if( file != 7 )
			moves |= ( 1 << ( square - 15 ) ) & Squares;
		if( file != 0 )
			moves |= ( 1 << ( square - 17 ) ) & Squares;
	}
	if( file < 6 ) { // Go Right
		if( rank != 7 ) // Go Up
			moves |= ( 1 << ( square + 10 ) ) & Squares;
		if( rank != 0 )
			moves |= ( 1 << ( square - 6 ) ) & Squares;
	}
	if( file > 1 ) { // Go Left
		if( rank != 7 ) // Go Up
			moves |= ( 1 << ( square + 6 ) ) & Squares;
		if( rank != 0 )
			moves |= ( 1 << ( square - 10 ) ) & Squares;
	}
	return moves;
}

Bitboard RookMoves( Bitboard EnemyPieces, Bitboard FriendlyPieces, signed int square ) {
	signed int x = square + 8;
	Bitboard moves = 0x0000000000000000;
	Bitboard EmptySquares = ~( FriendlyPieces | EnemyPieces );
	Bitboard OccupiedSquares = ~EmptySquares;
	while( 63 >= x ) {
		if( OccupiedSquares & ( 1 << x ) ) {
			if( FriendlyPieces & ( 1 << x ) ) 
				break;
			else {
				moves |= ( 1 << x );
				break;
			}
		}
		moves |= ( 1 << x );
		x += 8;
	}
	x = square - 8;
	while( x >= 0 ) {
		if( OccupiedSquares & ( 1 << x ) ) {
			if( FriendlyPieces & ( 1 << x ) )
				break;
			else {
				moves |= ( 1 << x );
				break;
			}
		}
		moves |= ( 1 << x );
		x -= 8;
	}
	x = square + 1;
	while( ( x % 8 ) != 0 ) {
		if( OccupiedSquares & ( 1 << x ) ) {
			if( FriendlyPieces & ( 1 << x ) )
				break;
			else {
				moves |= ( 1 << x );
				break;
			}
		}
		moves |= ( 1 << x );
		x++;
	}
	x = square - 1;
	while( ( x % 8 ) != 7 ) {
		if( OccupiedSquares & ( 1 << x ) ) {
			if( FriendlyPieces & ( 1 << x ) )
				break;
			else {
				moves |= ( 1 << x );
				break;
			}
		}
		moves |= ( 1 << x );
		x--;
	}
	return moves;
}

Bitboard BishopMoves( Bitboard FriendlyPieces, Bitboard EnemyPieces, signed int square ) {
	Bitboard moves = 0x0000000000000000;
	Bitboard OccupiedSquares = FriendlyPieces | EnemyPieces;
	signed int x = square + 9;
	while( ( 63 >= x ) && ( x % 8 ) != 0 ) {
		if( OccupiedSquares & ( 1 << x ) ) {
			if( FriendlyPieces & ( 1 << x ) )
				break;
			else {
				moves |= ( 1 << x );
				break;
			}
		}
		moves |= ( 1 << x );
		x += 9;
	}
	x = square - 7;
	while( ( x >= 0 ) && ( x % 8 ) != 0 ) {
		if( OccupiedSquares & ( 1 << x ) ) {
			if( FriendlyPieces & ( 1 << x ) )
				break;
			else {
				moves |= ( 1 << x );
				break;
			}
		}
		moves |= ( 1 << x );
		x -= 7;
	}
	x = square + 7;
	while( ( 63 >= x ) && ( x % 8 ) != 7 ) {
		if( OccupiedSquares & ( 1 << x ) ) {
			if( FriendlyPieces & ( 1 << x ) )
				break;
			else {
				moves |= ( 1 << x );
				break;
			}
		}
		moves |= ( 1 << x );
		x += 7;
	}
	x = square - 9;
	while( ( x >= 0 ) && ( x % 8 ) != 7 ) {
		if( OccupiedSquares & ( 1 << x ) ) {
			if( FriendlyPieces & ( 1 << x ) )
				break;
			else {
				moves |= ( 1 << x );
				break;
			}
		}
		moves |= ( 1 << x );
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
		if( OccupiedSquares & ( 1 << x ) ) {
			if( EnemyPieces & ( 1 << x ) )
				moves |= ( 1 << x );
		}
		else 
			moves |= ( 1 << x );
	}
	x = square - 1;
	if( ( x >= 0 ) && ( ( x % 8 ) != 7 ) ) {
		if( OccupiedSquares & ( 1 << x ) ) {
			if( EnemyPieces & ( 1 << x ) )
				moves |= ( 1 << x );
		}
		else 
			moves |= ( 1 << x );
	}
	x = square + 7;
	if( ( 63 >= x ) && ( ( x % 8 ) != 7 ) ) {
		if( OccupiedSquares & ( 1 << x ) ) {
			if( EnemyPieces & ( 1 << x ) )
				moves |= ( 1 << x );
		}
		else 
			moves |= ( 1 << x );
	}
	x = square + 8;
	if( 63 >= x ) {
		if( OccupiedSquares & ( 1 << x ) ) {
			if( EnemyPieces & ( 1 << x ) )
				moves |= ( 1 << x );
		}
		else
			moves |= ( 1 << x );
	}
	x = square + 9;
	if( ( 63 >= x ) && ( ( x % 8 ) != 0 ) ) {
		if( OccupiedSquares & ( 1 << x ) ) {
			if( EnemyPieces & ( 1 << x ) )
				moves |= ( 1 << x );
		}
		else
			moves |= ( 1 << x );
	}
	x = square - 7;
	if( ( x >= 0 ) && ( ( x % 8 ) != 0 ) ) {
		if( OccupiedSquares & ( 1 << x ) ) {
			if( EnemyPieces & ( 1 << x ) )
				moves |= ( 1 << x );
		}
		else
			moves |= ( 1 << x );
	}
	x = square - 8;
	if( x >= 0 ) {
		if( OccupiedSquares & ( 1 << x ) ) {
			if( EnemyPieces & ( 1 << x ) )
				moves |= ( 1 << x );
		}
		else
			moves |= ( 1 << x );
	}
	x = square - 9;
	if( ( x >= 0 ) && ( ( x % 8 ) != 7 ) ) {
		if( OccupiedSquares & ( 1 << x ) ) {
			if( EnemyPieces & ( 1 << x ) )
				moves |= ( 1 << x );
		}
		else
			moves |= ( 1 << x );
	}
	return moves;
}

bool IsChecked( const BoardRep& board, signed int square ) {
	Boardrep OccupiedSquares, EnemyDiagonals, EnemyStraights, EnemyPawns, EnemyKnights, EnemyPieces, FriendlyPieces = 0x0000000000000000;
	OccupiedSquares = board.layer1 | board.layer2 | board.layer3;
	EnemyDiagonals |= board.layer1 & board.layer2 & ( ~board.layer3 ); // Bishops of both colors
	EnemyDiagonals |= board.layer1 & ( ~board.layer2 ) & board.layer3; // Queens
	EnemyStraights |= board.layer1 & ( ~board.layer2 ) & board.layer3;
	EnemyStraights |= ( ~board.layer1 ) & ( ~board.layer2 ) & board.layer3; //Rooks
	EnemyPawns |= board.layer1 & ( ~board.layer2 ) & ( ~board.layer3 );
	EnemyKnights |= ( ~board.layer1 ) & board.layer2 & ( ~board.layer3 );
	if( board.layer0 & ( 1 << square ) ) 
		EnemyPieces = ~board.layer0;
	else
		EnemyPieces = board.layer0;
	EnemyDiagonals &= EnemyPieces;
	EnemyStraights &= EnemyPieces;
	EnemyPawns &= EnemyPieces;
	EnemyKnights &= EnemyPieces;
	FriendlyPieces = ~EnemyPieces & OccupiedSquares;
	if( EnemyDiagonals & ( BishopMoves( EnemyPieces, FriendlyPieces, square ) | QueenMoves( EnemyPieces, FriendlyPieces, square ) ) )
		return true;
	if( EnemyStraights & ( BishopMoves( EnemyPieces, FriendlyPieces, square ) | QueenMoves( EnemyPieces, FriendlyPieces, square ) ) )
		return true;
	if( EnemyPawns & PawnMoves( EnemyPieces, FriendlyPieces, square, board.layer0 & ( 1 << square ), 0x0000 ) )
		return true;
	if( EnemyKnights & KnightMoves( EnemyPieces, FriendlyPieces, square ) )
		return true;
	return false;
}

Bitboard CastleMoves( const BoardRep& board, signed int square, BitFlags posflags ) {
	Bitboard moves = 0x0000000000000000;
	int color = board.layer0 & ( 1 << square );
	if( posflags & ( WHITE_CHECK | BLACK_CHECK ) )
		return moves;
	if( color ) {
		if( ~posflags & ( BLACK_KINGSIDE_CASTLE | BLACK_QUEENSIDE_CASTLE ) )
			return moves;
	}
	else {
		if( ~posflags & ( WHITE_KINGSIDE_CASTLE | WHITE_QUEENSIDE_CASTLE ) )
			return moves;
	}
	if( 
