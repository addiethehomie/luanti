// Luanti
// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright (C) 2010-2013 celeron55, Perttu Ahola <celeron55@gmail.com>

#pragma once

#include "irrlichttypes.h"

#include <vector3d.h>

typedef core::vector3df v3f;
typedef core::vector3d<double> v3d;
typedef core::vector3d<s16> v3s16;
typedef core::vector3d<u16> v3u16;
typedef core::vector3d<s32> v3s32;

// Phase Dimension System: 4D coordinate structure (X, Y, Z, Phase)
struct v4s16 {
	s16 X, Y, Z, P = 0;  // Default to Phase 0 for backwards compatibility
	
	// Constructor from v3s16 (defaults to Phase 0)
	v4s16(s16 x, s16 y, s16 z, s16 p = 0) : X(x), Y(y), Z(z), P(p) {}
	
	// Constructor from v3s16 struct
	v4s16(const v3s16& pos, s16 p = 0) : X(pos.X), Y(pos.Y), Z(pos.Z), P(p) {}
	
	// Default constructor
	v4s16() = default;
	
	// Hash function for unordered_map
	struct Hash {
		size_t operator()(const v4s16& pos) const {
			// Convert signed s16 to unsigned to avoid sign extension issues
			u16 x = (u16)pos.X;
			u16 y = (u16)pos.Y;
			u16 z = (u16)pos.Z;
			u16 p = (u16)pos.P;
			
			// Use proper bit separation with multiplication for better distribution
			return ((size_t)x << 48) ^ 
				   ((size_t)y << 32) ^ 
				   ((size_t)z << 16) ^ 
				   (size_t)p;
		}
	};
	
	// Equality operator
	bool operator==(const v4s16& other) const {
		return X == other.X && Y == other.Y && 
			   Z == other.Z && P == other.P;
	}
	
	// Inequality operator
	bool operator!=(const v4s16& other) const {
		return !(*this == other);
	}
	
	// Conversion to v3s16 (for backwards compatibility)
	v3s16 toV3s16() const {
		return v3s16(X, Y, Z);
	}
};
