
const signed int INFINITE = 32767;

const signed int NEG_INFINITE = -32768;

signed int MiniMax( signed int alpha = NEG_INFINITE, signed int beta = INFINITE, int depth = 0, bool IsMaximiser, Position pos ) {
	int moves = 0;
	signed int eval, best;
	MoveNode* movelist = GenMoves( pos );
	MoveNode* p = movelist;
	Position testpos = pos;
	while( p != NULL ) {
		p = p->nxt;
		moves++;
	}
	if( ( depth == 0 ) || ( moves == 0 ) )
		return Evaluate( pos, moves );
	p = movelist;
	if( IsMaximiser ) {
		best = NEG_INFINITE;
		while( ( beta > alpha ) && ( moves > 0 ) ) {
			MakeMove( testpos, p->move );
			eval = MiniMax( alpha, beta, depth - 1, false, testpos );
			if( eval > best ) {
				best = eval;
				if( eval > alpha )
					alpha = eval;
			}
			p = p->nxt;
			moves--;
			testpos = pos;
		}
	}
	else {
		best = INFINITE;
		while( ( beta > alpha ) && ( moves > 0 ) ) {
			MakeMove( testpos, p->move );
			eval = MiniMax( alpha, beta, depth - 1, true, testpos );
			if( eval < best ) {
				best = eval;
				if( eval < beta )
					beta = eval;
			}
			p = p->nxt;
			moves--;
			testpos = pos;
		}
	}
	DeleteMoveList( movelist );
	return best;
}

MoveRep Search( Position& pos, int depth ) {
	MoveNode* movelist = GenMoves( pos );
	MoveNode* bestmove, p;
	int moves = 0;
	signed int eval, best;
	signed int alpha = NEG_INFINITY;
	signed int beta = INFINITY;
	Position testpos = pos;
	MoveRep ret;
	p = movelist;
	while( p != NULL ) {
		p = p->nxt;
		moves++;
	}
	p = movelist;
	if( pos.flags & WHITE_TO_MOVE ) {
		best = alpha;
		while( ( beta > alpha ) && ( moves > 0 ) ) {
			MakeMove( testpos, p->move );
			eval = MiniMax( alpha, beta, depth - 1, false, testpos );
			if( eval > best ) {
				best = eval;
				if( eval > alpha )
					alpha = eval;
			}
			p = p->nxt;
			moves--;
			testpos = pos;
		}
	}
	else {
		best = beta;
		while( ( beta > alpha ) && ( moves > 0 ) ) {
			MakeMove( testpos, p->move );
			eval = MiniMax( alpha, beta, depth - 1, true, testpos );
			if( eval < best ) {
				best = eval;
				if( eval < beta )
					beta = eval;
			}
			p = p->nxt;
			moves--;
			testpos = pos;
		}
	}
	ret = bestmove->move;
	DeleteMoveList( movelist );
	return ret;
}

