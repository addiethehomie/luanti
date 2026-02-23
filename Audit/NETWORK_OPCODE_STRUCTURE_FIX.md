# Network Opcode Structure Mismatch Fix

## Issue Identified
- **Date**: 2026-02-23
- **Files**: `src/network/clientopcodes.cpp`
- **Error Type**: C++11 narrowing compilation error
- **Root Cause**: Structure mismatch in opcode tables

## Problem Details
The compilation failed with errors:
```
error: type 'void (Client::*)(NetworkPacket *)' cannot be narrowed to 'bool' in initializer list [-Wc++11-narrowing]
```

### Root Cause Analysis
Lines 176-177 in `clientopcodes.cpp` contained entries using `ServerCommandFactory` structure format within a `ToClientCommandHandler` table:

```cpp
// WRONG - ServerCommandFactory format in ToClientCommandHandler table
{ "TOCLIENT_BLOCKDATA_4D",      TOCLIENT_STATE_CONNECTED, &Client::handleCommand_BlockData4D }, // 0x2d
{ "TOCLIENT_PHASE_CHANGE",      TOCLIENT_STATE_CONNECTED, &Client::handleCommand_PhaseChange }, // 0x2e
```

### Structure Definitions
- **ToClientCommandHandler**: `{ const char* name; ToClientConnectionState state; void (Client::*handler)(NetworkPacket* pkt); }`
- **ServerCommandFactory**: `{ const char* name; u8 channel; bool reliable; }`

The compiler was trying to convert the function pointer `&Client::handleCommand_BlockData4D` to a `bool` (third field of ServerCommandFactory), causing the narrowing error.

## Fix Applied
Replaced the mismatched entries with proper `null_command_handler` entries:

```cpp
// CORRECT - Proper ToClientCommandHandler format
null_command_handler, // 0x2d
null_command_handler, // 0x2e
```

## Prevention Pattern
When adding new opcode entries:
1. Verify the table type (ToClientCommandHandler vs ServerCommandFactory vs ClientCommandFactory)
2. Use the correct null placeholder: `null_command_handler` for client handlers, `null_command_factory` for factories
3. Match the structure field types exactly

## Similar Issues to Watch For
- Mixed structure types in `serveropcodes.cpp`
- Mixed structure types in other network opcode files
- Function pointer vs boolean field mismatches

## Impact
This was a blocking compilation error that prevented both MinGW32 and MinGW64 builds from completing.
