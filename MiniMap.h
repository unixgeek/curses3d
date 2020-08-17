#ifndef FPS_MINIMAP_H
#define FPS_MINIMAP_H

#include "GameMap.h"
#include "GameWindow.h"
#include "Player.h"

class MiniMap {
public:
    MiniMap(int window_start_x, int window_start_y, int size, GameMap game_map);

    void renderView(GameWindow &window, const Player &player);

private:
    GameMap game_map_;
    double points_[16]{};
    int size_;
    int window_start_x_;
    int window_start_y_;
    double circumference_;
};

#endif //FPS_MINIMAP_H
