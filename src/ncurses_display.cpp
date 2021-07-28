#include <curses.h>
#include <chrono>
#include <string>
#include <thread>
#include <vector>

#include "format.h"
#include "ncurses_display.h"
#include "system.h"

using std::string;
using std::to_string;

// 50 bars uniformly displayed from 0 - 100 %
// 2% is one bar(|)
std::string NCursesDisplay::ProgressBar(float percent) {
  std::string result{"0%"};
  int size{50};
  float bars{percent * size};

  for (int i{0}; i < size; ++i) {
    result += i <= bars ? '|' : ' ';
  }

  string display{to_string(percent * 100).substr(0, 4)};
  if (percent < 0.1 || percent == 1.0)
    display = " " + to_string(percent * 100).substr(0, 3);
  return result + " " + display + "/100%";
}

void NCursesDisplay::DisplaySystem(System& system, WINDOW* window) {
  int row{0};
  mvwprintw(window, ++row, 2, ("OS: " + system.OperatingSystem()).c_str());
  mvwprintw(window, ++row, 2, ("Kernel: " + system.Kernel()).c_str());
  mvwprintw(window, ++row, 2, "CPU: ");
  wattron(window, COLOR_PAIR(1));
  mvwprintw(window, row, 10, "");
  wprintw(window, ProgressBar(system.Cpu().Utilization()).c_str());
  wattroff(window, COLOR_PAIR(1));
  mvwprintw(window, ++row, 2, "Memory: ");
  wattron(window, COLOR_PAIR(1));
  mvwprintw(window, row, 10, "");
  wprintw(window, ProgressBar(system.MemoryUtilization()).c_str());
  wattroff(window, COLOR_PAIR(1));
  mvwprintw(window, ++row, 2,
            ("Total Processes: " + to_string(system.TotalProcesses())).c_str());
  mvwprintw(
      window, ++row, 2,
      ("Running Processes: " + to_string(system.RunningProcesses())).c_str());
 mvwprintw(window, ++row, 2,
           ("Up Time: " + Format::ElapsedTime(system.UpTime())).c_str());
  wrefresh(window);
}

void NCursesDisplay::DisplayProcesses(System & system, std::vector<Process>& processes,
                                      WINDOW* window, int n) {
  int row{0};
  int const pid_column{2}; /// 7//       6
  int const ppid_column{8};        //      7
  int const user_column{14};///          10
  int const cpu_column{24};/// 7//      6
  int const ram_column{31};/// 9//     9
  int const time_column{40}; ///10//  10
  int const command_column{50}; ///??
  int cmdsize  = window->_maxx - command_column - 2;
  if (system.sortedByPid())  {
      wattron(window, COLOR_PAIR(1));
      mvwprintw(window, ++row, pid_column, "PID ");
  } else {
      wattron(window, COLOR_PAIR(3));
      mvwprintw(window, ++row, pid_column, "P");
      wattron(window, COLOR_PAIR(1));
      mvwprintw(window, row, pid_column+1, "ID   ");
  }
  if (system.sortedByPpid()){
      wattron(window, COLOR_PAIR(1));
      mvwprintw(window, row, ppid_column, "PPID ");
 } else {	 
      wattron(window, COLOR_PAIR(1));
      mvwprintw(window, row, ppid_column, "PP");
      wattron(window, COLOR_PAIR(3));
      mvwprintw(window, row, ppid_column+2, "I");
      wattron(window, COLOR_PAIR(1));
      mvwprintw(window, row, ppid_column+3, "D   ");
 }
  if (system.sortedByName()){
       wattron(window, COLOR_PAIR(1));
      mvwprintw(window, row, user_column, "U");
      mvwprintw(window, row, user_column+1, "SER");
     
 } else {	  
      wattron(window, COLOR_PAIR(3));
      mvwprintw(window, row, user_column, "U");
       wattron(window, COLOR_PAIR(1));
      mvwprintw(window, row, user_column+1, "SER");
  } 
  if (system.sortedByCpu()){
       wattron(window, COLOR_PAIR(1));
      mvwprintw(window, row, cpu_column, "C");
      mvwprintw(window, row, cpu_column+1, "PU[%%]");
      
 } else {	  
      wattron(window, COLOR_PAIR(3));
      mvwprintw(window, row, cpu_column, "C");
       wattron(window, COLOR_PAIR(1));
      mvwprintw(window, row, cpu_column+1, "PU[%%]");
  }
  if (system.sortedByMem()){
      wattron(window, COLOR_PAIR(1));
      mvwprintw(window, row, ram_column, "R");
      mvwprintw(window, row, ram_column+1, "AM[MB]");
 
 } else {	  
      wattron(window, COLOR_PAIR(3));
      mvwprintw(window, row, ram_column, "R");
      wattron(window, COLOR_PAIR(1));
      mvwprintw(window, row, ram_column+1, "AM[MB]");
 }
  if (system.sortedByTime()){
      wattron(window, COLOR_PAIR(1));
     mvwprintw(window, row, time_column, "T");
     mvwprintw(window, row, time_column+1, "IME+");

 } else {	 
     wattron(window, COLOR_PAIR(3));
     mvwprintw(window, row, time_column, "T");
      wattron(window, COLOR_PAIR(1));
     mvwprintw(window, row, time_column+1, "IME+");
 } 
  mvwprintw(window, row, command_column, "COMMAND");
  wattroff(window, COLOR_PAIR(2));
  for (int i = 0; i < n; ++i) {
    mvwprintw(window, ++row, pid_column, Format::JustRight(std::to_string(processes[i].Pid()), 5).c_str());
    mvwprintw(window, row, ppid_column, Format::JustRight(std::to_string(processes[i].PPid()),5).c_str());
    mvwprintw(window, row, user_column, Format::JustLeft(processes[i].User(),10).c_str());
    float cpu = processes[i].CpuUtilization() * 100;
    mvwprintw(window, row, cpu_column+1, Format::JustRight(to_string(cpu),4).c_str());
    mvwprintw(window, row, ram_column, Format::JustRight(std::to_string(processes[i].Ram()),7).c_str());
    mvwprintw(window, row, time_column,
              Format::ElapsedTime(processes[i].UpTime()).c_str());
    mvwprintw(window, row, command_column,Format::JustLeft(processes[i].Command(),cmdsize).c_str());
  }
  wattron(window, COLOR_PAIR(3));
  mvwprintw(window, ++row, 2, "Q");
  wattron(window, COLOR_PAIR(1));
  mvwprintw(window, row, 3, "uit   ");
  if (!system.ascending()) wattron(window, COLOR_PAIR(3));
  mvwprintw(window, row, 7, "A");
  wattron(window, COLOR_PAIR(1));
  mvwprintw(window, row, 8, "scending  ");
  if (system.ascending()) wattron(window, COLOR_PAIR(3));
   mvwprintw(window, row, 17, "D");
  wattron(window, COLOR_PAIR(1));
  mvwprintw(window, row, 18, "escending   ");
}

void NCursesDisplay::Display(System& system, int n) {
  initscr();      // start ncurses
  noecho();       // do not print input values
  cbreak();       // terminate ncurses on ctrl + c
  start_color();  // enable color
  nodelay(stdscr, TRUE);

   int x_max{getmaxx(stdscr)};
   char in;
   int go {1};
   WINDOW* system_window = newwin(9, x_max - 1, 0, 0);
   WINDOW* process_window =
      newwin(4 + n, x_max - 1, system_window->_maxy + 1, 0);
   std::vector <int> keypress;
   bool pressed{false};	   
  while (go) {
	while ((in = getch()) != ERR) keypress.push_back(in);
	pressed = false;
	for (uint i = 0; i < keypress.size() && !pressed;i++){
	

            if (keypress[i] == 'r'){
		   system.sortByMem();
		   pressed = true;
             } else if (keypress[i] == 'c'){
              system.sortByCpu();
		   pressed = true;
		 break;
             } else if (keypress[i] == 'u'){
               system.sortByName();
		   pressed = true;
	       
            } else if (keypress[i] == 'p'){
               system.sortByPid();
		   pressed = true;
	    } else if (keypress[i] == 't'){
                system.sortByTime();
		   pressed = true;
            } else if (keypress[i] == 'q'){
                go = 0;
		   pressed = true;
           } else if (keypress[i] == 'i'){
	      system.sortByPpid();
		   pressed = true;
           } else if (keypress[i] == 'a'){
	      system.sortAscend();
		   pressed = true;
           } else if (keypress[i] == 'd'){
	      system.sortDescend();
		   pressed = true;
           }     
           
	}
	keypress.clear();

    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_BLACK, COLOR_CYAN);
    box(system_window, 0, 0);
    box(process_window, 0, 0);
    DisplaySystem(system, system_window);
    DisplayProcesses(system, system.Processes(), process_window, n);
    wrefresh(system_window);
    wrefresh(process_window);
    refresh();
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  endwin();
}
