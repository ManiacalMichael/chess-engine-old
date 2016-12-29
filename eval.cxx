/*
 * * * * * eval.cxx
 * Position evaluation function
 ** Considerations for position evaluation
 * Pawns are more valuable closer to the end of the board
 * Knights are more valuable the more pawns there are
 * Knights are more valuable in the center of the board
 * Bishops are each more valuable when they are in pairs
 * Rooks are more valuable the less pawns there are
 * Rooks are more valuable on empty ranks/files
 * Rooks are more valuable on ranks/files with other friendly rooks
 * In the endgame, kings are more valuable in the center
 * Queens are almost always more valuable than a rook pair
 * Castling has a bonus
 * Pawn Structure:
 * 	Connected pawns are good
 * 	Doubled pawns are very bad
 * 	Isolated pawns are bad
 * 	Backwards pawns are bad
 */

#include "eval.hxx"

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
	OccupiedSquares &= ~FriendlyRooks;
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

signed int Evaluate( Position& pos, int mobility ) {
	BoardRep& board = &pos.board;
	BoardRep notboard;
	Bitboard EnemyPawns, FriendlyPawns, EnemyKnights, FriendlyKnights, EnemyBishops, FriendlyBishops,
		 EnemyRooks, FriendlyRooks, EnemyQueens, FriendlyQueens, EnemyKing, FriendlyKing,
		 EnemyPieces, FriendlyPieces, OccupiedSquares;
	int pawns, fbishops, ebishops;
	signed int eval = 0;
	if( pos.flags & GAME_OVER ) {
		if( pos.flags & WHITE_CHECK ) 
			return -12288;
		if( pos.flags & BLACK_CHECK ) 
			return +12288;
	}
	else {
		if( pos.fiftymove >= 100 )
			pos.flags |= GAME_DRAWN;
	}
	if( pos.flags & GAME_DRAWN )
		return 0;
	notboard.layer0 ~= board.layer0;
	notboard.layer1 ~= board.layer1;
	notboard.layer2 ~= board.layer2;
	notboard.layer3 ~= board.layer3;
	EnemyPieces = board.layer0 | board.layer1 | board.layer2 | board.layer3;
	OccupiedSquares = EnemyPieces;
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
	eval += 1150 * PopCount( FriendlyQueens );
	eval -= 1150 * PopCount( EnemyQueens );
	eval += RookEval( FriendlyRooks, EnemyRooks, OccupiedSquares );
	eval += PawnStructure( FriendlyPawns );
	eval -= PawnStructure( EnemyPawns );
	if( pos.moves > 90 ) {
		eval += PopCount( FriendlyKing & CenterSquares ) * 50;
		eval -= PopCount( EnemyKing & CenterSquares ) * 50;
	}
	eval += 5 * mobility;
	if( pos.flags & ( BLACK_QUEENSIDE_CASTLE | BLACK_KINGSIDE_CASTLE ) )
		eval += 20;
	if( pos.flags & ( WHITE_QUEENSIDE_CASTLE | WHITE_KINGSIDE_CASTLE ) )
		eval -= 20;
	return eval;
}
