#include <cmath>
#include <utility>
#include "MiniMap.h"

MiniMap::MiniMap(int window_start_x, int window_start_y, int size, GameMap game_map) : window_start_x_(window_start_x),
                                                                                       window_start_y_(window_start_y),
                                                                                       size_(size),
                                                                                       circumference_(2 * M_PI),
                                                                                       game_map_(std::move(game_map)) {
    // Sadly, I had to refer to high school trigonometry.
    // https://en.wikibooks.org/wiki/High_School_Trigonometry/Radian_Measure
    for (int i = 0; i < 16; i++) {
        points_[i] = (M_PI * i) / 8;
    }
}

void MiniMap::renderView(GameWindow &window, const Player &player) {
    // Center map on player.
    int x = (int) player.getX() - (size_ / 2);
    int y = (int) player.getY() - (size_ / 2);

    int window_location_x = window_start_x_;
    for (int nx = x; nx < (x + size_); nx++) {
        int window_location_y = window_start_y_;
        for (int ny = y; ny < (y + size_); ny++) {
            wchar_t pixel = ' ';
            // Check if this point is inside the game map.
            if (nx > -1 && nx < game_map_.getWidth() && ny > -1 && ny < game_map_.getHeight()) {
                pixel = game_map_.getAt(nx, ny);
            }
            window.setAt(window_location_x, window_location_y, pixel);
            window_location_y++;
        }
        window_location_x++;
    }

    wchar_t pointer = '?';

    // ⇐     ⇑     ⇒     ⇓     ⇖     ⇗     ⇘     ⇙
    // 21d0  21d1  21d2  21d3  21d6  21d7  21d8  21d9

    // todo hack. Angle moves in opposite direction than I'm expecting.
    double a = circumference_ - player.getAngle();
    if (a >= points_[15] || a <= points_[1]) {
        pointer = 0x2192;//d2;
    } else if (a >= points_[1] && a <= points_[3]) {
        pointer = 0x2197;//d7;
    } else if (a >= points_[3] && a <= points_[5]) {
        pointer = 0x2191;//d1;
    } else if (a >= points_[5] && a <= points_[7]) {
        pointer = 0x2196;//d6;
    } else if (a >= points_[7] && a <= points_[9]) {
        pointer = 0x2190;//d0;
    } else if (a >= points_[9] && a <= points_[11]) {
        pointer = 0x2199;//d9;
    } else if (a >= points_[11] && a <= points_[13]) {
        pointer = 0x2193;//d3;
    } else if (a >= points_[13] && a <= points_[15]) {
        pointer = 0x2198;//d8;
    }

    // Draw player pointer in center of map.
    window.setAt(window_start_x_ + (size_ / 2), window_start_y_ + (size_ / 2), pointer);
}
