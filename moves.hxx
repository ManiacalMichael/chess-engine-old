#ifndef INCLUDE_MOVES_HXX
#define INCLUDE_MOVES_HXX

#include "boardrep.hxx"
#include <cstddef>

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
	MoveNode() : nxt( NULL ), move( 0 ) {}
	MoveRep move;
	MoveNode* nxt;
};

void MakeMove( Position&, MoveRep );

int LS1BIndice( Bitboard );

MoveNode* AddMoves( MoveNode*, Bitboard, int );

MoveNode* AddNode( MoveNode*, MoveRep );

MoveNode* RemoveNode( MoveNode* );

void DeleteMoveList( MoveNode* );

#endif
