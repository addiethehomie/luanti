// Luanti
// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright (C) 2013 celeron55, Perttu Ahola <celeron55@gmail.com>

#include "database.h"
#include "irrlichttypes.h"


/****************
 * The position encoding is a bit messed up because negative
 * values were not taken into account.
 * But this also maps 0,0,0 to 0, which is nice, and we mostly
 * need forward encoding in Luanti.
 */
s64 MapDatabase::getBlockAsInteger(const v3s16 &pos)
{
	return ((s64) pos.Z << 24) + ((s64) pos.Y << 12) + pos.X;
}


v3s16 MapDatabase::getIntegerAsBlock(s64 i)
{
	// Offset so that all negative coordinates become non-negative
	i = i + 0x800800800;
	// Which is now easier to decode using simple bit masks:
	return { (s16)( (i        & 0xFFF) - 0x800),
	         (s16)(((i >> 12) & 0xFFF) - 0x800),
	         (s16)(((i >> 24) & 0xFFF) - 0x800) };
}

// Phase-aware 4D coordinate encoding
s64 MapDatabase::getBlockAsInteger(const v4s16 &pos)
{
	// Convert to unsigned to avoid sign extension issues
	u16 x = (u16)pos.X;
	u16 y = (u16)pos.Y;
	u16 z = (u16)pos.Z;
	u16 p = (u16)pos.P;
	
	// Extend encoding to include phase in the upper bits
	// Phase gets 16 bits, X,Y,Z get 16 bits each
	return ((s64) p << 48) + ((s64) z << 32) + 
		   ((s64) y << 16) + x;
}

v4s16 MapDatabase::getIntegerAsBlock(s64 i)
{
	// Decode 4D coordinates: P(16) | Z(16) | Y(16) | X(16)
	// All values are unsigned, convert back to signed
	return { (s16)( i        & 0xFFFF),
	         (s16)(((i >> 16) & 0xFFFF)),
	         (s16)(((i >> 32) & 0xFFFF)),
	         (s16)(((i >> 48) & 0xFFFF)) };
}
