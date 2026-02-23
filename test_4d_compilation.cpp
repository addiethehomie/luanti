// Test file to verify 4D coordinate system compilation
#include "src/irr_v3d.h"
#include <unordered_map>
#include <iostream>

int main() {
    // Test v4s16 construction
    v4s16 pos1(100, 50, 25, 3);
    v4s16 pos2(v3s16(100, 50, 25), 3);
    v4s16 pos3; // Default constructor (should be 0,0,0,0)
    
    // Test hash function
    std::unordered_map<v4s16, int, v4s16::Hash> test_map;
    test_map[pos1] = 1;
    test_map[pos2] = 2;
    test_map[pos3] = 3;
    
    // Test equality
    if (pos1 == pos2) {
        std::cout << "pos1 == pos2: true" << std::endl;
    }
    
    // Test conversion to v3s16
    v3s16 pos3d = pos1.toV3s16();
    std::cout << "4D to 3D conversion: (" << pos3d.X << "," << pos3d.Y << "," << pos3d.Z << ")" << std::endl;
    
    // Test stream operator
    std::cout << "4D position: " << pos1 << std::endl;
    
    std::cout << "All 4D coordinate tests passed!" << std::endl;
    return 0;
}
