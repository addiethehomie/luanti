# Luanti Windows Cross-Compilation Fixes

## Purpose & Intent
Document fixes applied to resolve compilation failures in the Luanti Windows cross-compilation build process.

## Issues Resolved

### 1. Ambiguous Function Call in dummymap.h (Critical)
**File:** `src/dummymap.h` (line 32)  
**Error:** Call to `getBlockNoCreateNoEx({x, y, z})` was ambiguous  
**Root Cause:** Initializer list `{x, y, z}` could be converted to both `v3s16` and `v4s16` types due to v4s16 constructor `v4s16(s16 x, s16 y, s16 z, s16 p = 0)`  
**Fix:** Explicit cast to `v3s16{x, y, z}` to resolve ambiguity  
**Impact:** Resolves build-blocking compilation error

### 2. Division by Zero Warning in s_security.cpp (Low Priority)
**File:** `src/script/cpp_api/s_security.cpp` (line 1123)  
**Warning:** Potential division by zero in clock resolution calculation  
**Root Cause:** Static analyzer false positive - `SSCSM_CLOCK_RESOLUTION_US` is defined as constant 20 in `constants.h`  
**Fix:** Added `static_assert(SSCSM_CLOCK_RESOLUTION_US > 0, "SSCSM_CLOCK_RESOLUTION_US must be positive");`  
**Impact:** Suppresses false positive warning with compile-time verification

### 3. Unused Private Fields in database-sqlite3.h (Low Priority)
**File:** `src/database/database-sqlite3.h` (lines 209-212)  
**Warning:** Unused 4D statement pointers (`m_stmt_read_4d`, `m_stmt_write_4d`, etc.)  
**Status:** Documented as non-critical, can be addressed in future cleanup  
**Impact:** No build impact, warnings only

## Technical Notes

### Function Overload Resolution
The ambiguity arose from the phase-aware block system introducing 4D coordinates (`v4s16`) alongside existing 3D coordinates (`v3s16`). The `v4s16` struct's constructor with default parameter allows implicit conversion from 3-element initializer lists, creating overload resolution conflicts.

### Clock Resolution Safety
The `SSCSM_CLOCK_RESOLUTION_US` constant (20 microseconds) is used for SSCSM (Server-Side Client-Side Mod) clock resolution as a security measure against timing attacks. The compile-time assertion ensures this safety invariant.

## Build Status
- Primary compilation blocker: **RESOLVED**
- Secondary warnings: **DOCUMENTED/ADDRESSED**
- Ready for build continuation
