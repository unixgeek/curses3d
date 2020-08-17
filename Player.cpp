#include "Player.h"
#include <algorithm>
#include <cmath>
#include <vector>

Player::Player(int x, int y) : position_x_((double) x), position_y_((double) y), angle_(0.0), speed_(1.0),
                               turn_speed_(speed_ * 0.85), fov_(M_PI / 4.0),
                               depth_(16.0), circumference_(2 * M_PI) {

}

void Player::moveForward(const GameMap &game_map, double elapsed_time) {
    double delta_x = position_x_ + cos(angle_) * speed_ * fudgeFactor(elapsed_time);
    double delta_y = position_y_ + sin(angle_) * speed_ * fudgeFactor(elapsed_time);

    if (game_map.getAt((int) delta_x, (int) delta_y) != '#') {
        position_x_ = delta_x;
        position_y_ = delta_y;
    }
}

void Player::moveBackward(const GameMap &game_map, double elapsed_time) {
    double delta_x = position_x_ - cos(angle_) * speed_ * fudgeFactor(elapsed_time);
    double delta_y = position_y_ - sin(angle_) * speed_ * fudgeFactor(elapsed_time);

    if (game_map.getAt((int) delta_x, (int) delta_y) != '#') {
        position_x_ = delta_x;
        position_y_ = delta_y;
    }
}

void Player::turnLeft(double elapsed_time) {
    angle_ -= turn_speed_ * fudgeFactor(elapsed_time);
    if (angle_ < 0) {
        angle_ = angle_ + circumference_;
    }
}

void Player::turnRight(double elapsed_time) {
    angle_ += turn_speed_ * fudgeFactor(elapsed_time);
    if (angle_ > circumference_) {
        angle_ = angle_ - circumference_;
    }
}

double Player::getX() const {
    return position_x_;
}

double Player::getY() const {
    return position_y_;
}

double Player::getAngle() const {
    return angle_;
}

void Player::renderView(GameWindow &window, const GameMap &map) {
    for (int x = 0; x < window.getWidth(); x++) {

        // For this x (column) determine if a wall or boundary is visible and if so, what is the distance to it.
        ViewAtX view_at_x = getViewAtX(window, map, x);

        // Calculate distance to ceiling and floor.
        int ceiling = (int) ((((double) window.getHeight()) / 2.0) -
                             (((double) window.getHeight()) / view_at_x.distance_to_wall));
        int floor = window.getHeight() - ceiling;

        // For this y (row) determine if we show floor, ceiling, wall, or boundary.
        for (int y = 0; y < window.getHeight(); y++) {
            if (y < ceiling) {
                // This point is ceiling, so set it to blank.
                window.setAt(x, y, ' ');
            } else if (y > ceiling && y <= floor) {
                // This point is a wall or boundary, so set that.
                window.setAt(x, y, view_at_x.pixel);
            } else {
                // This point is a floor, so set it based on the distance to it.
                double floor_distance =
                        1.0 -
                        (((double) y - ((double) window.getHeight() / 2.0)) / ((double) window.getHeight() / 2.0));
                window.setAt(x, y, getPixelForFloor(floor_distance));
            }
        }
    }
}

Player::ViewAtX Player::getViewAtX(GameWindow &window, const GameMap &map, int x) {
    // Calculate "view".
    double rayAngle =
            (angle_ - fov_ / 2.0) + ((double) x / (double) window.getWidth()) * fov_;
    double eye_x = cos(rayAngle);
    double eye_y = sin(rayAngle);

    double distance_to_wall = 0.0;
    bool hit_wall = false;
    wchar_t shade = ' ';
    // Find the distance to a wall, if there is one.
    while (!hit_wall && distance_to_wall < depth_) {
        distance_to_wall += 0.1;

        int test_x = (int) (position_x_ + eye_x * distance_to_wall);
        int test_y = (int) (position_y_ + eye_y * distance_to_wall);

        if (test_x < 0 || test_x > map.getWidth() || test_y < 0 || test_y > map.getHeight()) {
            // Exhausted our range, but didn't find a wall.
            hit_wall = true;
            distance_to_wall = depth_;
        } else if (map.getAt(test_x, test_y) == '#') {
            // Found a wall.
            hit_wall = true;

            if (isBoundary(eye_x, eye_y, test_x, test_y)) {
                // If this happens to be a boundary, show that.
                shade = '|';
            } else {
                // Not a boundary, so show the appropriate character for the distance to the wall.
                shade = getPixelForWall(distance_to_wall);
            }
        }
    }

    ViewAtX view_at_x{};
    view_at_x.distance_to_wall = distance_to_wall;
    view_at_x.pixel = shade;
    return view_at_x;
}

// Determine if the point is a boundary based on the view.
bool Player::isBoundary(double eye_x, double eye_y, int test_x, int test_y) const {
    bool boundary = false;

    // Look for boundaries.
    std::vector<std::pair<double, double>> p;
    for (int tx = 0; tx < 2; tx++) {
        for (int ty = 0; ty < 2; ty++) {
            auto vy = (double) test_y + (double) ty - position_y_;
            auto vx = (double) test_x + (double) tx - position_x_;
            auto d = sqrt(vx * vx + vy * vy);
            double dot = (eye_x * vx / d) + (eye_y * vy / d);
            p.emplace_back(d, dot);
        }
    }

    // Sort pairs closest to farthest.
    std::sort(p.begin(), p.end(),
              [](const std::pair<double, double> &left, const std::pair<double, double> &right) {
                  return left.first < right.first;
              });

    double bound = 0.004;
    if (acos(p.at(0).second) < bound) boundary = true;
    if (acos(p.at(1).second) < bound) boundary = true;
    if (acos(p.at(2).second) < bound) boundary = true;

    return boundary;
}

// Determine the appropriate character based on the distance to the wall.
wchar_t Player::getPixelForWall(double distance_to_wall) const {
    wchar_t pixel;

    if (distance_to_wall <= depth_ / 4.0) {
        pixel = 0x2588; // Unicode Full Block
    } else if (distance_to_wall < depth_ / 3.0) {
        pixel = 0x2593; // Unicode Dark Shade
    } else if (distance_to_wall < depth_ / 2.0) {
        pixel = 0x2592; // Unicode Medium Shade
    } else if (distance_to_wall < depth_) {
        pixel = 0x2591; // Unicode Light Shade
    } else {
        pixel = ' ';
    }

    return pixel;
}

// Determine the appropriate character based on the floor distance.
wchar_t Player::getPixelForFloor(double floor_distance) {
    wchar_t pixel;

    if (floor_distance < 0.25) {
        pixel = '#';
    } else if (floor_distance < 0.5) {
        pixel = 'X';
    } else if (floor_distance < 0.75) {
        pixel = '.';
    } else if (floor_distance < 0.9) {
        pixel = '-';
    } else {
        pixel = ' ';
    }

    return pixel;
}

double Player::fudgeFactor(double elapsed_time) {
    return elapsed_time * 100.0;
}
