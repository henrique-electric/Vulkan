#include <utils.hpp>

namespace utils {
    bool strcmp(const char* str1, const char* str2) {
        int count = 0;
        
        if (str1 != nullptr && str2 != nullptr) {
            while (str1[count] != 0x00 && str2[count] != 0x00) {
                if (str1[count] != str2[count]) {
                    return false;
                }
                
                count++;
            }
            return true;
        }
    }
}
