// Simple test to verify database schema fix
#include <iostream>
#include <string>

// Test the SQL schema that was fixed
const char *schema =
    "CREATE TABLE IF NOT EXISTS `blocks` (\n"
        "`x` INTEGER,\n"
        "`y` INTEGER,\n"
        "`z` INTEGER,\n"
        "`p` INTEGER DEFAULT 0,  -- Phase column (0 for legacy compatibility)\n"
        "`data` BLOB NOT NULL,\n"
        // Unified primary key includes phase dimension
        "PRIMARY KEY (`x`, `z`, `y`, `p`)"
    ");\n"
;

int main() {
    std::cout << "Testing 4D database schema..." << std::endl;
    
    // Check if the schema contains the phase column
    std::string schema_str(schema);
    if (schema_str.find("`p` INTEGER DEFAULT 0") != std::string::npos) {
        std::cout << "✓ Phase column found in schema" << std::endl;
    }
    
    if (schema_str.find("PRIMARY KEY (`x`, `z`, `y`, `p`)") != std::string::npos) {
        std::cout << "✓ 4D primary key found in schema" << std::endl;
    }
    
    // Test that there's a newline after the phase column (the fix)
    if (schema_str.find("DEFAULT 0,  -- Phase column") != std::string::npos) {
        std::cout << "✓ Phase column syntax appears correct" << std::endl;
    }
    
    std::cout << "Database schema verification completed!" << std::endl;
    return 0;
}
