#include "GameWindow.h"

GameWindow::GameWindow(int width, int height) : width_(width), height_(height) {
    buffer_ = new wchar_t[width * height]{' '};
    window_ = newwin(height, width, 0, 0);
}

GameWindow::~GameWindow() {
    delwin(window_);
    delete[] buffer_;
}

void GameWindow::updateWindow() {
    mvwaddnwstr(window_, 0, 0, buffer_, width_ * height_);
    wrefresh(window_);
}

int GameWindow::getWidth() const {
    return width_;
}

int GameWindow::getHeight() const {
    return height_;
}

void GameWindow::setAt(int x, int y, wchar_t value) {
    buffer_[width_ * y + x] = value;
}

void GameWindow::setAt(int x, int y, const char *value) {
    char ch;
    while ((ch = *value++) != '\0') {
        setAt(x, y, ch);
        x++;
    }
}
