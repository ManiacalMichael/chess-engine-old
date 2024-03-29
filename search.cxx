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
#include "movegen.hxx"
#include "eval.hxx"

signed int MiniMax( Position& pos, bool IsMaximiser, signed int alpha = NEG_INFINITE, signed int beta = INFINITE, int depth = 0 ) {
	int moves = 0;
	signed int eval, best;
	MoveNode* movelist = GenMoves( pos );
	MoveNode* p = movelist;
	Position testpos = pos;
	while( p != NULL ) {
		p = p->nxt;
		moves++;
	}
	if( ( depth == 0 ) || ( moves == 0 ) ) {
		DeleteMoveList( movelist );
		return Evaluate( pos );
	}
	p = movelist;
	if( IsMaximiser ) {
		best = NEG_INFINITE;
		while( ( beta > alpha ) && ( moves > 0 ) ) {
			MakeMove( testpos, p->move );
			eval = MiniMax( testpos, false, alpha, beta, depth - 1 );
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
			eval = MiniMax( testpos, true, alpha, beta, depth - 1 );
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
	MoveNode* bestmove;
	MoveNode* p;
	int moves = 0;
	signed int eval, best;
	signed int alpha = NEG_INFINITE;
	signed int beta = INFINITE;
	Position testpos = pos;
	MoveRep ret;
	p = movelist;
	if( movelist == NULL ) {
		return 0x0000;
	}
	while( p != NULL ) {
		p = p->nxt;
		moves++;
	}
	p = movelist;
	if( pos.flags & WHITE_TO_MOVE ) {
		best = alpha;
		while( ( beta > alpha ) && ( moves > 0 ) ) {
			MakeMove( testpos, p->move );
			eval = MiniMax( testpos, false, alpha, beta, depth - 1 );
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
			eval = MiniMax( testpos, true, alpha, beta, depth - 1 );
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

