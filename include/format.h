#ifndef FORMAT_H
#define FORMAT_H

#include <string>

namespace Format {
std::string ElapsedTime(long times);  // TODO: See src/format.cpp
std::string PID(int pid);
std::string User(std::string user);
std::string Cmd(std::string cmd,int l);
std::string Ram(int ram);
std::string PPid(std::string ppid);
std::string CPU(std::string  c);
std::string JustLeft(std::string in, uint sz);
std::string JustRight(std::string in, uint sz);
};                                    // namespace Format

#endif
