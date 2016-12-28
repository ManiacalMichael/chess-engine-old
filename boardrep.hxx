#ifndef INCLUDE_BOARDREP_HXX
#define INCLUDE_BOARDREP_HXX

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

typedef long long unsigned int Bitboard;

struct BoardRep;

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

typedef short unsigned int BitFlags;
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

struct Position;

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

int GetPiece( BoardRep&, int );

#endif
