#ifndef FPS_PLAYER_H
#define FPS_PLAYER_H

#include "GameMap.h"
#include "GameWindow.h"

class Player {
public:
    Player(int x, int y);

    void moveForward(const GameMap &game_map, double elapsed_time);

    void moveBackward(const GameMap &game_map, double elapsed_time);

    void turnLeft(double elapsed_time);

    void turnRight(double elapsed_time);

    double getX() const;

    double getY() const;

    double getAngle() const;

    void renderView(GameWindow &window, const GameMap &map);

private:
    double position_x_;
    double position_y_;
    double angle_;
    double speed_;
    double turn_speed_;
    double fov_;
    double depth_;
    double circumference_;

    struct ViewAtX {
        double distance_to_wall;
        wchar_t pixel;
    };

    ViewAtX getViewAtX(GameWindow &window, const GameMap &map, int x);

    bool isBoundary(double eye_x, double eye_y, int test_x, int test_y) const;

    wchar_t getPixelForWall(double distance_to_wall) const;

    static wchar_t getPixelForFloor(double floor_distance);

    static double fudgeFactor(double elapsed_time);
};

#endif //FPS_PLAYER_H
