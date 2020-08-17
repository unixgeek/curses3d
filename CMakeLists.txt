cmake_minimum_required(VERSION 3.13)
project(curses3d)

set(CMAKE_CXX_STANDARD 14)

find_package(PkgConfig REQUIRED)

pkg_check_modules(NCURSES REQUIRED ncursesw)
include_directories(${NCURSES_INCLUDE_DIRS})

add_executable(curses3d main.cpp GameWindow.cpp GameWindow.h GameMap.cpp GameMap.h Player.cpp Player.h MiniMap.cpp MiniMap.h)
target_link_libraries(curses3d ${NCURSES_LIBRARIES})