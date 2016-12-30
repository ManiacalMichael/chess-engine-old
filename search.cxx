/*
 * * * * * search.cxx
 * Search function
 ** MiniMax + Alpha Beta Pruning
 * White selects highest scoring moves
 * Black selects lowest scoring moves
 * Alpha is the minimum score accepted by white parent nodes
 * Beta is the maximum score accepted by black parent nodes
 * When Beta is less than Alpha no new nodes examined will be accepted by
 *   parent nodes and search terminates on that node
 *
 ** Search Function
 * The search function is a special case of MiniMax that keeps track of
 * which possible move scores the highest
 *
 */

#include "search.hxx"

signed int MiniMax( signed int alpha = NEG_INFINITE, signed int beta = INFINITE, int depth = 0, bool IsMaximiser, const Position& pos ) {
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

MoveRep Search( const Position& pos, int depth ) {
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
				bestmove = p;
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
				bestmove = p;
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

