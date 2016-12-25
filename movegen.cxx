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
	Bitboard OccupiedSquares, EnemyKing, EnemyDiagonals, EnemyStraights, EnemyPawns, EnemyKnights, EnemyPieces, FriendlyPieces = 0x0000000000000000;
	OccupiedSquares = board.layer1 | board.layer2 | board.layer3;
	EnemyDiagonals |= board.layer1 & board.layer2 & ( ~board.layer3 ); // Bishops of both colors
	EnemyDiagonals |= board.layer1 & ( ~board.layer2 ) & board.layer3; // Queens
	EnemyStraights |= board.layer1 & ( ~board.layer2 ) & board.layer3;
	EnemyStraights |= ( ~board.layer1 ) & ( ~board.layer2 ) & board.layer3; //Rooks
	EnemyPawns |= board.layer1 & ( ~board.layer2 ) & ( ~board.layer3 );
	EnemyKnights |= ( ~board.layer1 ) & board.layer2 & ( ~board.layer3 );
	EnemyKing |= ( ~board.layer1 ) & board.layer2 & board.layer3;
	if( board.layer0 & ( 1 << square ) ) 
		EnemyPieces = ~board.layer0;
	else
		EnemyPieces = board.layer0;
	EnemyDiagonals &= EnemyPieces;
	EnemyStraights &= EnemyPieces;
	EnemyPawns &= EnemyPieces;
	EnemyKnights &= EnemyPieces;
	EnemyKing &= EnemyPieces;
	FriendlyPieces = ~EnemyPieces & OccupiedSquares;
	if( EnemyDiagonals & ( BishopMoves( EnemyPieces, FriendlyPieces, square ) | QueenMoves( EnemyPieces, FriendlyPieces, square ) ) )
		return true;
	if( EnemyStraights & ( BishopMoves( EnemyPieces, FriendlyPieces, square ) | QueenMoves( EnemyPieces, FriendlyPieces, square ) ) )
		return true;
	if( EnemyPawns & PawnMoves( EnemyPieces, FriendlyPieces, square, board.layer0 & ( 1 << square ), 0x0000 ) )
		return true;
	if( EnemyKnights & KnightMoves( EnemyPieces, FriendlyPieces, square ) )
		return true;
	if( EnemyKings & KingMoves( EnemyPieces, FriendlyPieces, square ) )
		return true;
	return false;
}

Bitboard CastleMoves( const BoardRep& board, signed int square, BitFlags posflags ) {
	Bitboard moves = 0x0000000000000000;
	Bitboard EmptySquares ~= board.layer0 | board.layer1 | board.layer2 | board.layer3;
	int color = board.layer0 & ( 1 << square );
	if( posflags & ( WHITE_CHECK | BLACK_CHECK ) )
		return moves;
	if( color ) {
		if( posflags & BLACK_KINGSIDE_CASTLE ) {
			if( ( EmptySquares & ( 1 << ( square + 1 ) ) ) && ( EmptySquares & ( 1 << ( square + 2 ) ) ) ) {
				if( ! IsChecked( testboard, square + 1 ) ) {
					if( ! IsChecked( testboard, square + 2 ) )
						moves |= ( 1 << square + 2 );
				}
			}
		}
		if( posflags & BLACK_QUEENSIDE_CASTLE ) {
			if( ( EmptySquares & ( 1 << ( square - 1 ) ) ) && ( EmptySquares & ( 1 << ( square - 2 ) ) ) && ( EmptySquares & ( 1 << ( square - 3 ) ) ) ) {
				if( ! IsChecked( testboard, square - 1 ) ) {
					if( ! IsChecked( testboard, square - 2 ) ) 
						moves |= ( 1 << square - 2 );
				}
			}
		}
	}
	else {
		if( posflags & WHITE_KINGSIDE_CASTLE ) {
			if( ( EmptySquares & ( 1 << ( square + 1 ) ) ) && ( EmptySquares & ( 1 << ( square + 2 ) ) ) ) {
				if( ! IsChecked( testboard, square + 1 ) ) {
					if( ! IsChecked( testboard, square + 2 ) )
						moves |= ( 1 << square + 2 );
				}
			}
		}
		if( posflags & WHITE_QUEENSIDE_CASTLE ) {
			if( ( EmptySquares & ( 1 << ( square - 1 ) ) ) && ( EmptySquares & ( 1 << ( square - 2 ) ) ) && ( EmptySquares & ( 1 << ( square - 3 ) ) ) ) {
				if( ! IsChecked( testboard, square - 1 ) ) {
					if( ! IsChecked( testboard, square - 2 ) ) 
						moves |= ( 1 << square - 2 );
				}
			}
		}
	}
	return moves;
}

int LS1BIndice( Bitboard ls1b ) {	/// Isolated LS1B only
	const int arr[ 67 ] = {
		-1, 0, 1, 39, 2, 15, 40, 23, 3, 12, 16,
		59, 41, 19, 24, 54, 4, -1, 13, 10, 17, 62,
		60, 28, 42, 30, 20, 51, 25, 44, 55, 47, 5, 
		32, -1, 38, 14, 22, 11, 58, 18, 53, 63, 9,
		61, 27, 29, 50, 43, 46, 31, 37, 21, 57, 52,
		8, 26, 49, 45, 36, 56, 7, 48, 35, 6, 34, 33
	};
	return arr[ ls1b % 67 ];
}

MoveNode* AddMoves( MoveNode* p, Bitboard moves, int square ) {
	MoveRep r = 0x0000;
	int end = 0;
	while( moves != 0x0000000000000000 ) {
		end = LS1BIndice( ( moves & ( moves - 1 ) ) ^ moves );
		p = AddNode( p, square | ( end << 6 ) );
		moves &= moves - 1;
	}
	return p;
}

MoveNode* AddNode( MoveNode* p, MoveRep move ) {
	p->move = move;
	p->nxt = new MoveNode;
	return p->nxt;
}

MoveNode* GenMoves( const Position& pos, int color ) {
	/*
	 * Notes:
	 * Generate moves
	 * Add special flags to special moves
	 * Test moves p->nxt ( don't test root node )
	 * Test root node
	 * return movelist
	 */
	MoveNode* movelist = new MoveNode;
	MoveNode* p = movelist;
	const BoardRep board = pos.board;
	const BitFlags posflags = pos.flags;
	int piece, type;
	Position testpos = pos;
	Bitboard EnemyKing = ( ~board.layer1 ) | board.layer2 | board.layer3;
	Bitboard EnemyPieces, FriendlyPieces;
	Bitboard OccupiedSquares = board.layer0 | board.layer1 | board.layer2 | board.layer3;
	if( color ) 
		EnemyPieces = OccupiedSquares & ( ~board.layer0 );
	else
		EnemyPieces = OccupiedSquares & board.layer0;
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
