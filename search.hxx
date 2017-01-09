#ifndef INCLUDE_SEARCH_HXX
#define INCLUDE_SEARCH_HXX

#include "boardrep.hxx"
#include "moves.hxx"

const signed int INFINITE = 32767;

const signed int NEG_INFINITE = -32768;

signed int MiniMax( Position&, bool, signed int, signed int, int );

MoveRep Search( Position&, int );

#endif
