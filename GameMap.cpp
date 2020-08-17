#include <fstream>
#include "GameMap.h"

GameMap::GameMap() : height_(16), width_(16), okay_(true) {
    map_.append("################");
    map_.append("#              #");
    map_.append("#  ###         #");
    map_.append("#  # #         #");
    map_.append("#  # #         #");
    map_.append("#  #           #");
    map_.append("#  # #         #");
    map_.append("#  # #         #");
    map_.append("#  # #         #");
    map_.append("#  # #         #");
    map_.append("#  # #         #");
    map_.append("#  # #         #");
    map_.append("#  # #         #");
    map_.append("#  # #         #");
    map_.append("#    #         #");
    map_.append("################");
}

GameMap::GameMap(const std::string &map_file) : width_(0), height_(0), okay_(false) {
    std::ifstream in(map_file, std::ios::in);

    if (!in.is_open()) {
        okay_ = false;
    } else {
        std::string line;
        while (getline(in, line)) {
            // Remove a new line character if it exists.
            if (line.at(line.length() - 1) == '\n') {
                line.erase(line.length() - 1);
            }

            if (width_ == 0) {
                // First line determines the width. Not sure what happens if the lines vary in length...
                width_ = line.length();
            }

            map_.append(line);
            height_++;
        }
        in.close();

        if (width_ >= 16 and height_ >= 16) {
            okay_ = true;
        } else {
            okay_ = false;
        }
    }
}

int GameMap::getHeight() const {
    return height_;
}

int GameMap::getWidth() const {
    return width_;
}

char GameMap::getAt(int x, int y) const {
    return map_[y * width_ + x];
}

bool GameMap::isOkay() const {
    return okay_;
}
