/*
 * * * * * boardrep.cxx
 * Board and piece representation
 *
 ** Bitboards
 * Bitboards are a 64-bit integer representing an 8x8 chessboard
 * Each bit on a bitboard gives information about the square it represents
 * Indice % 8 -    0   1   2   3   4   5   6   7  
 *             7 A8  B8  C8  D8  E8  F8  G8  H8
 *             6 A7  B7  C7  D7  E7  F7  G7  H7
 *             5 A6  B6  C6  D6  E6  F6  G6  H6
 *             4 A5  B5  C5  D5  E5  F5  G5  H5
 *             3 A4  B4  C4  D4  E4  F4  G4  H4
 *             2 A3  B3  C3  D3  E3  F3  G3  H3
 *             1 A2  B2  C2  D2  E2  F2  G2  H2
 *           | 0 A1  B1  C1  D1  E1  F1  G1  H1
 *        Indice / 8
 * The bit for a specific square can be manipulated by creating a bitboard with only
 * that specific square set by shifting a 1 <square> spaces ( assuming 64-bit int )
 * Set bit x : bb |= 1 << x
 * Clear bit x : bb &= ~( 1 << x )
 * Check if bit x is set : bb & ( 1 << x )
 *
 ** Pieces:
 * Pieces values can be stored in a 4-bit integer
 * White pieces have bit 0 unset ( even-numbered )
 * Black pieces have bit 0 set ( odd-numbered )
 * Piece type can be determined by performing floor division by two:
 * Empty / 2 : 0
 * Pawn / 2 : 1
 * Knight / 2 : 2
 * Bishop / 2 : 3
 * Rook / 2 : 4
 * Queen / 2 : 5
 * King / 2 : 6
 *
 ** Board Representation
 * A full chessboard can be represented with four bitboards
 * The first bitboard representing the 0 bit for each piece value on each square
 * The second bitboard representing the 1 bit for each piece value on each square
 * etc.
 *
 ** Position Representation
 * A full position requires the following to be documented:
 * - The position of all the pieces
 * - The square where an en passant capture can be made
 * - Castling status
 * - Check status
 * - If the game is over/drawn
 * - The number of moves that have been made
 * - A counter to implement the fifty move rule
 * If the game ends in checkmate one player will be in check and the game will be
 * over at the same time
 */

#ifndef INCLUDE_BOARDREP_HXX
#define INCLUDE_BOARDREP_HXX

#include <cstdint>

const int NO_PIECE = 0;
const int INVALID = 1;
const int WHITE_PAWN = 2;
const int BLACK_PAWN = 3;
const int WHITE_KNIGHT = 4;
const int BLACK_KNIGHT = 5;
const int WHITE_BISHOP = 6;
const int BLACK_BISHOP = 7;
const int WHITE_ROOK = 8;
const int BLACK_ROOK = 9;
const int WHITE_QUEEN = 10;
const int BLACK_QUEEN = 11;
const int WHITE_KING = 12;
const int BLACK_KING = 13;
/* Piece values for each layer:
 * Bits	  3210 
 * 	0 0000 Empty square
 * 	1 0001 Unused
 * 	2 0010 White Pawn
 * 	3 0011 Black Pawn
 * 	4 0100 White Knight
 * 	5 0101 Black Knight
 * 	6 0110 White Bishop
 * 	7 0111 Black Bishop
 * 	8 1000 White Rook
 * 	9 1001 Black Rook
 * 	A 1010 White Queen
 * 	B 1011 Black Queen
 * 	C 1100 White King
 * 	D 1101 Black King
 */

typedef uint64_t Bitboard;

struct BoardRep {
	Bitboard layer0;
	Bitboard layer1;
	Bitboard layer2;
	Bitboard layer3;
};

const BoardRep START_BOARD = {
	0xFFFF000000000000,
	0x2CFF00000000FF2C,
	0x7600000000000076,
	0x9900000000000099
};
/*
 *   Layer 0:
 *   1111111111111111000000000000000000000000000000000000000000000000
 *   Layer 1:
 *   0010110011111111000000000000000000000000000000001111111100101100
 *   Layer 2:
 *   0111011000000000000000000000000000000000000000000000000001110110
 *   Layer 3:
 *   1001100100000000000000000000000000000000000000000000000010011001
 * 
 * 0x957DB759333333330000000000000000000000000000000022222222846CA648
 */

typedef uint16_t BitFlags;
//	Position Flags:
//       0         123456      7        8       9       A       B        C          D           E              F
//| Ep on board | Ep square | WO-O | WO-O-O | BO-O | BO-O-O | Wcheck | Bcheck | Game over | Game Drawn | White to move |

const BitFlags EN_PASSANT = 0x0001;

const BitFlags EP_SQUARE_MASK = 0x007E;

const BitFlags WHITE_KINGSIDE_CASTLE = 0x0080;

const BitFlags WHITE_QUEENSIDE_CASTLE = 0x0100;

const BitFlags BLACK_KINGSIDE_CASTLE = 0x0200;

const BitFlags BLACK_QUEENSIDE_CASTLE = 0x0400;

const BitFlags WHITE_CHECK = 0x0800;

const BitFlags BLACK_CHECK = 0x1000;

const BitFlags GAME_OVER = 0x2000;

const BitFlags GAME_DRAWN = 0x4000;

const BitFlags WHITE_TO_MOVE = 0x8000;

struct Position {
	BoardRep board;
	BitFlags flags;
	short unsigned moves;
	short unsigned fiftymove;
};

const Position START_POSITION = {
	{
		0xFFFF000000000000,
		0X2CFF00000000FF2C,
		0x7600000000000076,
		0X9900000000000099
	},
	0x8780,
	0,
	0
};

void SetPiece( BoardRep&, int, int );

int GetPiece( const BoardRep&, int );

#endif
