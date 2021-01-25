#include <vector>

namespace ascii {
    // Ascii art data struct
    class Ascii {
    public:
        int height, length, numframes;
        std::vector<std::vector<std::string>> frames; 
    };

    // Add a c_str to the screen
    int ascii_mvaddstr(int y, int x, const char* str);

    // Draw an Ascii with passed flags
    int draw(const Ascii& art, int x, bool FLY = false);

    // Create Ascii struct by reading file
    Ascii createascii(std::string filename);
}
