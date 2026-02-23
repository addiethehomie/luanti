# Database SQLite3 Access Fix

## Issue
Compilation error in both MinGW32 and MinGW64 builds:
```
error: 'openDatabase' is a private member of 'Database_SQLite3'
```

## Root Cause
`MapDatabaseSQLite3` class inherits privately from `Database_SQLite3` and was trying to call the private method `openDatabase()` from the base class in its constructor.

## Solution
Changed `openDatabase()` method from `private` to `protected` in `Database_SQLite3` class to allow derived classes to access it.

## Files Modified
- `src/database/database-sqlite3.h`: Changed access specifier from private to protected for `openDatabase()` method

## Similar Issues to Watch For
1. **Private inheritance patterns**: Other classes using private inheritance may encounter similar access issues
2. **Method access levels**: Any future private methods in base classes that derived classes need to access
3. **Macro-generated calls**: The `PARENT_CLASS_FUNCS` macro generates calls to base class methods - ensure these remain accessible

## Prevention
- When adding new private methods to base classes, consider if derived classes might need access
- Use `protected` instead of `private` for methods that are part of the implementation contract but not public API
- Review inheritance hierarchies when changing access specifiers
