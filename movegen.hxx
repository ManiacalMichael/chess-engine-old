#ifndef INCLUDE_MOVEGEN_HXX
#define INCLUDE_MOVEGEN_HXX

#include "moves.hxx"
#include "boardrep.hxx"

Bitboard PawnMoves( Bitboard, Bitboard, int, int BitFlags );

Bitboard KnightMoves( Bitboard, Bitboard, int );

Bitboard RookMoves( Bitboard, Bitboard, signed int );

Bitboard BishopMoves( Bitboard, Bitboard, signed int );

Bitboard QueenMoves( Bitboard, Bitboard, signed int );

bool IsChecked( BoardRep&, signed int );

Bitboard CastleMoves( BoardRep&, signed int, BitFlags );

MoveNode* GenMoves( Position& );

#endif
