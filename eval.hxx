#ifndef INCLUDE_EVAL_HXX
#define INCLUDE_EVAL_HXX

#include "boardrep.hxx"

const signed int PawnRankTable[ 8 ] = { -1, 100, 105, 110, 115, 120, 125, -1 };

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

int PopCount( Bitboard );

signed int EvalPawns( Bitboard, Bitboard );

signed int EvalRooks( Bitboard, Bitboard, Bitboard );

signed int PawnStructure( Bitboard );

signed int Evaluate( Position& );

#endif
