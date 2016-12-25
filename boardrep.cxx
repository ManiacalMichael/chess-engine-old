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

struct BoardRep {
	Bitboard layer0;
	Bitboard layer1;
	Bitboard layer2;
	Bitboard layer3;
};
/* Piece values for each layer:
 * Bits	0123 
 * 	0000 Empty square
 * 	0001 Unused
 * 	0010 White Pawn
 * 	0011 Black Pawn
 * 	0100 White Knight
 * 	0101 Black Knight
 * 	0110 White Bishop
 * 	0111 Black Bishop
 * 	1000 White Rook
 * 	1001 Black Rook
 * 	1010 White Queen
 * 	1011 Black Queen
 * 	1100 White King
 * 	1101 Black King
 */

const BoardRep START_BOARD = {
	0xFFFF000000000000,
	0x2CFF00000000FF2C,
	0x7600000000000076,
	0x9900000000000099
};

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

void SetPiece( BoardRep& board, int square, int piece ) {
	if( piece & 1 )
		board.layer0 |= 1 << square;
	if( piece & 2 )
		board.layer1 |= 1 << square;
	if( piece & 4 )
		board.layer2 |= 1 << square;
	if( piece & 8 )
		board.layer3 |= 1 << square;
	return;
}

int GetPiece( const BoardRep& board, int square ) {
	int i = 0;
	if( board.layer0 & ( 1 << square ) )
		i += 1;
	if( board.layer1 & ( 1 << square ) )
		i += 2;
	if( board.layer2 & ( 1 << square ) )
		i += 4;
	if( board.layer3 & ( 1 << square ) )
		i += 8;
	return i;
}

