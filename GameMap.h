#ifndef FPS_GAMEMAP_H
#define FPS_GAMEMAP_H

#include <string>

class GameMap {
public:
    GameMap();

    explicit GameMap(const std::string &map_file);

    int getHeight() const;

    int getWidth() const;

    char getAt(int x, int y) const;

    bool isOkay() const;

private:
    int height_;
    int width_;
    std::string map_;
    bool okay_;
};

#endif //FPS_GAMEMAP_H
