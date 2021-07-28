#ifndef NCURSES_DISPLAY_H
#define NCURSES_DISPLAY_H

#include <curses.h>
#include <vector>
#include "process.h"
#include "system.h"

namespace NCursesDisplay {
void Display(System& system, int n = 15);
void DisplaySystem(System& system, WINDOW* window);
void DisplayProcesses(System & system, std::vector<Process>& processes, WINDOW* window, int n);
std::string ProgressBar(float percent);
};  // namespace NCursesDisplay

#endif
