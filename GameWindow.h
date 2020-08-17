#ifndef FPS_GAMEWINDOW_H
#define FPS_GAMEWINDOW_H

#include <ncurses.h>

class GameWindow {
public:
    GameWindow(int width, int height);

    ~GameWindow();

    GameWindow(const GameWindow &) = delete;

    GameWindow &operator=(const GameWindow &) = delete;

    void updateWindow();

    int getWidth() const;

    int getHeight() const;

    void setAt(int x, int y, wchar_t value);

    void setAt(int x, int y, const char *value);

private:
    int width_;
    int height_;
    wchar_t *buffer_;
    WINDOW *window_;
};

#endif //FPS_GAMEWINDOW_H
