# Fix for Missing m_sectors Member Variable

## Issue
Compilation errors in MinGW cross-compilation workflow for Luanti project due to missing `m_sectors` member variable.

## Root Cause
The `m_sectors` member variable was accidentally removed from the Map base class, but was still being used in:
- ClientMap::emergeSector() 
- ClientMap rendering loops
- ServerMap::createSector()
- DummyMap constructor

## Solution
1. **Added m_sectors to Map base class** (`src/map.h`):
   ```cpp
   // Sector storage - maps 2D coordinates to sectors
   std::map<v2s16, MapSector*> m_sectors;
   ```

2. **Implemented getSectorNoGenerate methods** (`src/map.cpp`):
   ```cpp
   MapSector *Map::getSectorNoGenerateNoLock(v2s16 p2d)
   MapSector *Map::getSectorNoGenerate(v2s16 p2d)
   ```

3. **Added proper cleanup in Map destructor** (`src/map.cpp`):
   ```cpp
   // Free all sectors
   for (auto &sector_entry : m_sectors) {
       MapSector *sector = sector_entry.second;
       delete sector;
   }
   m_sectors.clear();
   ```

## Files Modified
- `src/map.h`: Added m_sectors member variable
- `src/map.cpp`: Added getSectorNoGenerate implementations and destructor cleanup

## Impact
- Fixes compilation errors at lines 259, 443, 755, and 1559 in clientmap.cpp
- Restores proper sector storage functionality across all Map subclasses
- Maintains backward compatibility with existing code

## Verification
All necessary includes were already present:
- `<map>` included in map.h 
- `mapsector.h` included in map.cpp
