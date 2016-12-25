enum piece_t { EMPTY, INVALID,
	WHITE_PAWN, BLACK_PAWN,
	WHITE_KNIGHT, BLACK_KNIGHT,
	WHITE_BISHOP, BLACK_BISHOP,
	WHITE_ROOK, BLACK_ROOK,
	WHITE_QUEEN, BLACK_QUEEN,
	WHITE_KING, BLACK_KING
};

typedef long long unsigned int Bitboard;

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

typedef short unsigned int BitFlags;
//	Position Flags:
//       0         123456      7        8       9       A       B        C          D           E              F
//| Ep on board | Ep square | WO-O | WO-O-O | BO-O | BO-O-O | Wcheck | Bcheck | Game over | Game Drawn | White to move |

const BitFlags EN_PASSANT = 0x0001;

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
	int moves;
	int fiftymove;
};

