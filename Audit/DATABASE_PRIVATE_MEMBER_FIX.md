# Database Private Member Access Fix

## Issue Description
Compilation errors in both MinGW32 and MinGW64 builds due to `MapDatabaseSQLite3::openDatabase()` attempting to access private members of `Database_SQLite3` base class.

## Root Cause
The `MapDatabaseSQLite3` class uses private inheritance from `Database_SQLite3` and was trying to directly access:
- `m_savedir` (private member)
- `m_dbname` (private member) 
- `busyHandler` (private static method)
- `m_busy_handler_data` (private member)

## Solution Implemented

### 1. Added Protected Getter Methods
Added to `Database_SQLite3` class in `database-sqlite3.h`:
```cpp
// Getter methods for derived classes
const std::string& getSavedir() const { return m_savedir; }
const std::string& getDbname() const { return m_dbname; }
```

### 2. Fixed MapDatabaseSQLite3::openDatabase()
Replaced the duplicate implementation that accessed private members with:
```cpp
void MapDatabaseSQLite3::openDatabase()
{
    if (m_database) return;

    // Construct database path to check if it exists
    std::string dbp = getSavedir() + DIR_DELIM + getDbname() + ".sqlite";
    m_was_newly_created = !fs::PathExists(dbp);

    // Call base class method to handle database opening
    Database_SQLite3::openDatabase();
}
```

## Intent & Purpose
- **Why**: The derived class needed to track whether the database was newly created (`m_was_newly_created`) but was duplicating base class functionality
- **What**: Provide clean access to necessary data while maintaining encapsulation
- **How**: Use protected getter methods and call base class implementation instead of duplicating code

## Other Classes Checked
- `PlayerDatabaseSQLite3`: No override of `openDatabase()` - uses base class directly
- `AuthDatabaseSQLite3`: No override of `openDatabase()` - uses base class directly  
- `ModStorageDatabaseSQLite3`: No override of `openDatabase()` - uses base class directly

## Files Modified
- `src/database/database-sqlite3.h`: Added protected getter methods
- `src/database/database-sqlite3.cpp`: Fixed MapDatabaseSQLite3::openDatabase() implementation

## Testing Status
Fix addresses the specific compilation errors found in both MinGW32 and MinGW64 builds related to private member access.
