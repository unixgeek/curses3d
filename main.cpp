#include <chrono>
#include <clocale>
#include <iostream>
#include <ncurses.h>
#include "GameWindow.h"
#include "GameMap.h"
#include "Player.h"
#include "MiniMap.h"

bool initCurses() {
    if (setlocale(LC_CTYPE, "") == nullptr) return false;
    if (initscr() == nullptr) return false;
    if (cbreak() == ERR) return false;
    if (noecho() == ERR) return false;
    if (keypad(stdscr, TRUE) == ERR) return false;
    if (nodelay(stdscr, TRUE) == ERR) return false;
    if (refresh() == ERR) return false;

    return true;
}

int main(int argc, char *argv[]) {
    // Check to see if a map file was provided.
    std::string map_file;
    if (argc == 2) {
        map_file = argv[1];
    } else if (argc > 2) {
        std::string name(argv[0]);
        int pos = name.find_last_of('/');
        std::cerr << "usage: " << name.substr(pos + 1, name.length() - pos) << " [MAP_FILE]" << std::endl;
        return 1;
    }

    GameMap game_map;
    if (!map_file.empty()) {
        // If a map file was specified, use that.
        game_map = GameMap(map_file);
        if (!game_map.isOkay()) {
            std::cerr << "There was an error reading map file " << map_file << std::endl;
            return 1;
        }
    }

    if (!initCurses()) {
        std::cerr << "Error initializing ncurses" << std::endl;
        return 1;
    }

    // For debug.
    // char c; while ((c = getch()) != 'c') {}

    // COLS and LINES is set by ncurses.
    int width = COLS;
    int height = LINES;
    // todo need to check the size of the map vs the size of the screen. or write code to handle that.
    GameWindow game_window(width, height);

    Player player(1, 1);

    MiniMap mini_map(0, 1, 16, game_map);

    auto tp1 = std::chrono::system_clock::now();
    auto tp2 = std::chrono::system_clock::now();

    char stats[47]{' '};

    bool quit = false;
    bool do_update = true;

    while (!quit) {
        tp2 = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsedTime = tp2 - tp1;
        tp1 = tp2;

        if (do_update) {
            // Draw what the player sees.
            player.renderView(game_window, game_map);

            // Draw map_. y + 1 for map_ offset on screen.
            mini_map.renderView(game_window, player);

            do_update = false;
        }

        sprintf(stats, "X=%3.2f, Y=%3.2f, A=%3.2f FPS=%3.2f", player.getX(), player.getY(), player.getAngle(),
                1.0f / elapsedTime.count());
        game_window.setAt(0, 0, stats);

        game_window.updateWindow();

        int ch = getch();
        switch (ch) {
            case KEY_UP:
            case 'w':
                player.moveForward(game_map, elapsedTime.count());
                do_update = true;
                break;
            case KEY_DOWN:
            case 's':
                player.moveBackward(game_map, elapsedTime.count());
                do_update = true;
                break;
            case KEY_LEFT:
            case 'a':
                player.turnLeft(elapsedTime.count());
                do_update = true;
                break;
            case KEY_RIGHT:
            case 'd':
                player.turnRight(elapsedTime.count());
                do_update = true;
                break;
            default:
                break;
        }
        if (ch == KEY_END || ch == 'q') {
            quit = true;
        }
    }

    endwin();

    return 0;
}
