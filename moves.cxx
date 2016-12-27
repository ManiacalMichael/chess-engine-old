typedef short unsigned int MoveRep;
/*
 * MoveRep bit values
 *      012345        6789AB        CD          E          F
 * | Start Square | End Square | Promotion | Castle | E.p. Capture |
 */

const MoveRep PROMOTION_TO_KNIGHT = 0x1000;

const MoveRep PROMOTION_TO_QUEEN = 0x2000;

const MoveRep CASTLE_MOVE = 0x4000;

const MoveRep EP_CAPTURE = 0x8000;

const MoveRep START_SQUARE_MASK = 0x003F;

const MoveRep END_SQUARE_MASK = 0x0FC0;

struct MoveNode {
	MoveNode() : nxt( NULL ) {}
	MoveRep move;
	Move* nxt;
};


void MakeMove( Position pos, MoveRep move ) {
	/*
	 * notes:
	 * 1. Set end square
	 * 2. Perform ep capture
	 * 3. Clear ep squares
	 * 4. Set ep square
	 * 5. Clear start square
	 * 6. Increment counters
	 * 7. Set check status
	 * 8. Change castle status
	 * 9. Toggle player
	 */
	BoardRep& board = &pos.board;
	int color, kingpos, king;
	int start = move & START_SQUARE_MASK;
	int end = ( move & END_SQUARE_MASK ) >> 6;
	if( pos.flags & WHITE_TO_MOVE )
		color = 0;
	else
		color = 1;
	if( ( GetPiece( board, start ) / 2 ) == 1 )
		pos.fiftymove = 0;
	else if( GetPiece( board, start ) != NO_PIECE )
		pos.fiftymove = 0;
	else
		pos.fiftymove++;
	if( move & PROMOTION_TO_KNIGHT ) 
		SetPiece( board, end, WHITE_KNIGHT + color );
	else if( move & PROMOTION_TO_QUEEN )
		SetPiece( board, end, WHITE_QUEEN + color );
	else
		SetPiece( board, end, GetPiece( board, start ) );
	if( move & EP_CAPTURE ) {
		if( color )	// Capturing white pawn
			SetPiece( board, end + 8, NO_PIECE );
		else
			SetPiece( board, end - 8, NO_PIECE );
	}
	pos.flags &= ~EN_PASSANT;
	if( ( GetPiece( board, start ) / 2 ) == 1 ) {
		if( end == ( start + 16 ) ) {	// Set white ep
			pos.flags |= EN_PASSANT;
			pos.flags |= ( ( end - 8 ) << 1 );
		}
		else if( end == ( start - 16 ) ) {
			pos.flags |= EN_PASSANT;
			pos.flags |= ( ( end + 8 ) << 1 );
		}
	}
	SetPiece( board, start, NO_PIECE );
	king = WHITE_KING + color;
	for( int i = 0; i < 64; i++ ) {
		if( GetPiece( board, i ) == king )
			kingpos = i;
	}
	pos.flags &= ~( WHITE_CHECK | BLACK_CHECK );
	if( IsChecked( board, king ) ) {
		if( color )
			pos.flags |= BLACK_CHECK;
		else
			pos.flags |= WHITE_CHECK;
	}
	if( pos.flags & 0x0780 ) {	// 0x0780 is all the castle status bits
		if( start == 4 )	// 4 = e1
			pos.flags &= ~( WHITE_KINGSIDE_CASTLE | WHITE_QUEENSIDE_CASTLE );
		else if( start == 0 )	// 0 = a1
			pos.flags &= ~WHITE_QUEENSIDE_CASTLE;
		else if( start == 7 )	// 7 = h1
			pos.flags &= ~WHITE_KINGSIDE_CASTLE;
		else if( start == 60 )	// 60 = e8
			pos.flags &= ~( BLACK_KINGSIDE_CASTLE | BLACK_QUEENSIDE_CASTLE );
		else if( start == 56 )	// 56 = a8
			pos.flags &= ~BLACK_QUEENSIDE_CASTLE;
		else if( start == 63 )	// 63 = h8
			pos.flags &= ~BLACK_KINGSIDE_CASTLE;
	}
	pos.moves++;
	pos.flags ^= WHITE_TO_MOVE;
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

void RemoveNextNode( MoveNode* p ) {
	MoveNode* q, r = p->nxt;
	if( q->nxt != NULL )
		q = q->nxt;
	delete r;
	p->nxt = q;
}

void DeleteMoveList( MoveNode* movelist ) {
	MoveNode* p = movelist;
	while( movelist != NULL ) {
		p = movelist->nxt;
		delete movelist;
		movelist = p;
	}
}
