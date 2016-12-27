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

MoveNode* GenMoves( Position& pos ) {
	/*
	 * Notes:
	 * Generate moves
	 * Add special flags to special moves
	 * Test moves p->nxt ( don't test root node )
	 * Test root node
	 * Check for checkmate or draw for eval function
	 * return movelist
	 */
	MoveNode* movelist = new MoveNode;
	MoveNode* p, q = movelist;
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
		if( ( GetPiece( board, ( p->move ) & START_SQUARE_MASK ) / 2 ) == 1 ) {
			if( color ) {
				if( ( ( ( p->move & END_SQUARE_MASK ) >> 6 ) / 8 ) == 0 ) {
					q = new MoveNode;
					q->move = p->move | PROMOTION_TO_KNIGHT;
					p->move |= PROMOTION_TO_QUEEN;
					q->nxt = p->nxt;
					p->nxt = q;
				}
			}
			else {
				if( ( ( ( p->move & END_SQUARE_MASK ) >> 6 ) / 8 ) == 7 ) {
					q = new MoveNode;
					q->move = p->move | PROMOTION_TO_KNIGHT;
					p->move |= PROMOTION_TO_QUEEN;
					q->nxt = p->nxt;
					p->nxt = q;
				}
			}
			if( ( ( p->move & END_SQUARE_MASK ) >> 6 ) == ( posflags & EP_SQUARE_MASK ) ) 
				p->move |= EP_CAPTURE;
		}
		else if( ( GetPiece( board, p->move & START_SQUARE_MASK ) / 2 ) == 6 ) {
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
			DeleteNextNode( p );
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
	if( !( pos.flags & GAME_OVER ) ) {
		if( pos.fiftymove >= 100 ) {
			pos.flags |= GAME_DRAWN;
		}
	}
	return movelist;
}
