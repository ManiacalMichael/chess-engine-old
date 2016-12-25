/*
 * notes for evaluation:
 * Pawns
 * 	More valuable closer to end of board
 * Knights
 * 	More valuable the more pawns there are
 * 	More valuable in center
 * Bishops
 * 	More valuable in pairs
 * Penalties for undefended minor pieces
 * 	Not sure how to implement this w/o practically redoing movegen
 * Rooks
 * 	More valuable the less pawns there are
 * 	More valuable if on open rank/file
 * Queen
 * 	Important
 * King
 * 	King centralization in endgame
 * Castling
 * 	Penalty for uncastled king
 * Mobility
 * Pawn Structure
 * 	Connected pawns good
 * 	Doubled pawns bad
 * 	Isolated pawns bad
 * 	Backward pawns bad
 */

const signed int PawnRankValue[ 8 ] = { -1, 100, 105, 110, 115, 120, 125, -1 };

const signed int KnightPawnTable[ 17 ] = { 300, 306, 312, 318, 324, 330, 336, 342,
				348, 354, 360, 366, 378, 384, 390, 396, 402 };

const Bitboard CenterSquares = 0x00003C3C3C3C0000;

const signed int RookPawnTable[ 17 ] = { 636, 628, 620, 612, 604, 596, 588, 580,
				572, 564, 556, 548, 540, 532, 524, 516, 508 };

const signed int BishopTable[ 11 ] = { 0, 325, 500, 825, 1150, 1475, 1800, 2125, 2450, 2775, 3100 };

//const signed int UndefendedMinors[ 19 ] = { 0, -20, -40, -60, -80, -100, -120, -140, -160,
//		-180, -200, -220, -240, -260, -280, -300, -320, -340, -360 };

const Bitboard OpenRank[ 8 ] = {
	0x00000000000000FF,
	0x000000000000FF00,
	0x0000000000FF0000,
	0x00000000FF000000,
	0x000000FF00000000,
	0x0000FF0000000000,
	0x00FF000000000000,
	0xFF00000000000000 };

const Bitboard OpenFile[ 8 ] = {
	0x0101010101010101,
	0x0202020202020202,
	0x0404040404040404,
	0x0808080808080808,
	0x1010101010101010,
	0x2020202020202020,
	0x4040404040404040,
	0x8080808080808080 };

int PopCount( Bitboard bb ) {
	int x = 0;
	while( bb != 0x000000000000000 ) {
		bb &= ( bb - 1 );
		x++;
	}
	return x;
}

signed int EvalPawns( Bitboard FriendlyPawns, Bitboard EnemyPawns, int color ) {
	signed int eval = 0;
	if( color ) {
		while( FriendlyPawns != 0x0000000000000000 ) {
			eval += PawnRankTable[ 7 - ( LS1BIndice( ( FriendlyPawns & ( FriendlyPawns - 1 ) ) ^ FriendlyPawns ) / 8 )];
			FreindlyPawns &= FriendlyPawns - 1;
		}
		while( EnemyPawns != 0x0000000000000000 ) {
			eval -= PawnRankTable[ LS1BIndice( ( EnemyPawns & ( EnemyPawns - 1 ) ) ^ EnemyPawns ) / 8 ];
			EnemyPawns &= EnemyPawns - 1;
		}
	}
	else {
		while( FriendlyPawns != 0x0000000000000000 ) {
			eval += PawnRankTable[ LS1BIndice( ( FriendlyPawns & ( FriendlyPawns - 1 ) ) ^ FriendlyPawns ) / 8 ];
			FreindlyPawns &= FriendlyPawns - 1;
		}
		while( EnemyPawns != 0x0000000000000000 ) {
			eval -= PawnRankTable[ 7 - ( LS1BIndice( ( EnemyPawns & ( EnemyPawns - 1 ) ) ^ EnemyPawns ) / 8 )];
			EnemyPawns &= EnemyPawns - 1;
		}
	}
	return eval;
}

signed int EvalRooks( Bitboard FriendlyRooks, Bitboard EnemyRooks, Bitboard OccupiedSquares ) {
	signed int eval = 0;
	int square;
	while( FriendlyRooks != 0x0000000000000000 ) {
		square = LS1BIndice( ( FriendlyRooks & ( FriendlyRooks - 1 ) ) ^ FriendlyRooks );
		if( !( OccupiedSquares & EmptyRank[ square / 8 ] ) ) 
			eval += 30;
		if( !( OccupiedSquares & EmptyFile[ square % 8 ] ) )
			eval += 30;
		FriendlyRooks &= FriendlyRooks - 1;
	}
	while( EnemyRooks != 0x0000000000000000 ) {
		square = LS1BIndice( ( EnemyRooks & ( EnemyRooks - 1 ) ) ^ EnemyRooks );
		if( !( OccupiedSquares & EmptyRank[ square / 8 ] ) )
			eval -= 30;
		if( !( OccupiedSquares & EmptyRank[ square % 8 ] ) )
			eval -= 30;
		EnemyRooks &= EnemyRooks - 1;
	}
	return eval;
}

signed int PawnStructure( Bitboard skeleton ) {
	/*
	 * Isolated pawn evaluated once( -10 ) : -10
	 * Backwards pawn evaluated once( -12 ) : -12
	 * Connected pawn evaluated twice( +15 ) : +30
	 * Doubled pawn evaluated once( -45 ) : -45
	 */
	signed int eval = 0;
	int square, file;
	Bitboard test = skeleton;
	while( test != 0x0000000000000000 ) {
		square = LS1BIndice( ( test & ( test - 1 ) ) ^ test );
		file = square % 8;
		if( file == 0 ) {
			if( !( skeleton & EmptyFile[ 1 ] ) )	// Isolated pawn 
				eval -= 10;
			else {
				if( skeleton & ( 1 << ( square + 9 ) ) ) // Backwards pawn
					eval -= 12;
				if( skeleton & ( 1 << ( square + 1 ) ) ) // Connected pawn
					eval += 15;
			}
		}
		else if( file == 7 ) {
			if( !( skeleton & EmptyFile[ 6 ] ) )
				eval -= 10;
			else {
				if( skeleton & ( 1 << ( square + 7 ) ) )
					eval -= 12;
				if( skeleton & ( 1 << ( square - 1 ) ) )
					eval += 15;
			}
		}
		else {
			if( !( ( skeleton & EmptyFile[ file + 1 ] ) || ( skeleton & EmptyFile[ file - 1 ] ) ) )
				eval -= 10;
			else {
				if( skeleton & ( 1 << ( square + 9 ) ) )
					eval -= 12;
				if( skeleton & ( 1 << ( square + 7 ) ) )
					eval -= 12;
				if( skeleton & ( 1 << ( square + 1 ) ) )
					eval += 15;
				if( skeleton & ( 1 << ( square - 1 ) ) )
					eval += 15;
			}
		}
		if( skeleton & ( 1 << ( square + 8 ) ) ) // Doubled pawn
			eval -= 45;
		test &= test - 1;
	}
	return eval;
}

signed int Evaluate( const Position& pos, int color, int mobility ) {
	BoardRep& board = &pos.board;
	BoardRep notboard;
	Bitboard EnemyPawns, FriendlyPawns, EnemyKnights, FriendlyKnights, EnemyBishops, FriendlyBishops,
		 EnemyRooks, FriendlyRooks, EnemyQueens, FriendlyQueens, EnemyKing, FriendlyKing,
		 EnemyPieces, FriendlyPieces, OccupiedSquares;
	int pawns, fbishops, ebishops;
	signed int eval = 0;
	if( color ) {
		if( ( pos.flags & BLACK_CHECK ) && ( pos.flags & GAME_OVER ) )	// Checkmate
			return -12288;
		if( ( pos.flags & WHITE_CHECK ) && ( pos.flags & GAME_OVER ) )
			return +12288;
	}
	else {
		if( ( pos.flags & WHITE_CHECK ) && ( pos.flags & GAME_OVER ) )
			return -12288;
		if( ( pos.flags & BLACK_CHECK ) && ( pos.flags & GAME_OVER ) )
			return +12288;
	}
	if( pos.flags & GAME_DRAWN )
		return 0;
	notboard.layer0 ~= board.layer0;
	notboard.layer1 ~= board.layer1;
	notboard.layer2 ~= board.layer2;
	notboard.layer3 ~= board.layer3;
	EnemyPieces = board.layer0 | board.layer1 | board.layer2 | board.layer3;
	OccupiedSquares = EnemyPieces;
	if( color )
		EnemyPieces &= notboard.layer0;
	else
		EnemyPieces &= board.layer0;
	FriendlyPieces = OccupiedSquare & ( ~EnemyPieces );
	EnemyPawns = board.layer1 & notboard.layer2 & notboard.layer3;
	EnemyKnights = notboard.layer1 & board.layer2 & notboard.layer3;
	EnemyBishops = board.layer1 & board.layer2 & notboard.layer3;
	EnemyRooks = notboard.layer1 & notboard.layer2 & board.layer3;
	EnemyQueens = board.layer1 & notboard.layer2 & board.layer3;
	EnemyKing = notboard.layer1 & board.layer2 & board.layer3;
	FriendlyPawns = EnemyPawns & FriendlyPieces;
	FriendlyKnights = EnemyKnights & FriendlyPieces;
	FriendlyBishops = EnemyBishops & FriendlyPieces;
	FriendlyRooks = EnemyRooks & FriendlyPieces;
	FriendlyQueens = EnemyQueens & FriendlyPieces;
	FriendlyKing = EnemyKing & FriendlyPieces;
	EnemyPawns &= EnemyPieces;
	EnemyKnights &= EnemyPieces;
	EnemyBishops &= EnemyPieces;
	EnemyRooks &= EnemyPieces;
	EnemyQueens &= EnemyPieces;
	EnemyKing &= EnemyPieces;
	pawns = PopCount( EnemyPawns | FriendlyPawns );
	fbishops = PopCount( FriendlyBishops );
	ebishops = PopCount( EnemyBishops );
	eval += EvalPawns( FriendlyPawns, EnemyPawns, color );
	eval += KnightPawnTable[ pawns ] * PopCount( FriendlyKnights );
	eval -= KnightPawnTable[ pawns ] * PopCount( EnemyKnights );
	eval += 30 * PopCount( FriendlyKnights & CenterSquares );
	eval -= 30 * PopCount( EnemyKnights & CenterSquares );
	eval += BishopTable[ fbishops ];
	eval -= BishopTable[ ebishops ];
	eval += RookPawnTable[ pawns ] * PopCount( FriendlyRooks );
	eval -= RookPawnTable[ pawns ] * PopCount( EnemyRooks );
	eval += 1150 * PopCount( FreindlyQueens );
	eval -= 1150 * PopCount( EnemyQueens );
	eval += RookEval( FriendlyRooks, EnemyRooks, OccupiedSquares );
	eval += PawnStructure( FriendlyPawns );
	eval -= PawnStructure( EnemyPawns );
	if( pos.moves > 90 ) {
		eval += PopCount( FriendlyKing & CenterSquares ) * 50;
		eval -= PopCount( EnemyKing & CenterSquares ) * 50;
	}
	eval += 5 * mobility;
	if( color ) {
		if( pos.flags & ( BLACK_QUEENSIDE_CASTLE | BLACK_KINGSIDE_CASTLE ) )
			eval -= 20;
	}
	else {
		if( pos.flags & ( WHITE_QUEENSIDE_CASTLE | WHITE_KINGSIDE_CASTLE ) )
			eval -= 20;
	}
	return eval;
}

