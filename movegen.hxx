#ifndef INCLUDE_MOVEGEN_HXX
#define INCLUDE_MOVEGEN_HXX

#include "moves.hxx"
#include "boardrep.hxx"

Bitboard PawnMoves( Bitboard, Bitboard, int, int, BitFlags );

Bitboard KnightMoves( Bitboard, Bitboard, int );

Bitboard RookMoves( Bitboard, Bitboard, int );

Bitboard BishopMoves( Bitboard, Bitboard, int );

Bitboard QueenMoves( Bitboard, Bitboard, int );

Bitboard KingMoves( Bitboard, Bitboard, int );

bool IsChecked( const BoardRep&, int );

Bitboard CastleMoves( BoardRep&, int, BitFlags );

MoveNode* GenMoves( Position& );

#endif
