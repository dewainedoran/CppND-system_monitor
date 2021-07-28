#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>
#include <iostream>
#include <time.h>

#include "process.h"
#include "processor.h"

class System {
 public:

long millisPerJiff();



static bool sortCpu(Process uno, Process dos){
	return (uno.CpuUtilization() < dos.CpuUtilization());	
}

static bool rsortCpu(Process uno, Process dos){
	return (uno.CpuUtilization() > dos.CpuUtilization());	
}

static bool sortPpid(Process uno, Process dos){
	return (uno.PPid() < dos.PPid());	
}
static bool rsortPpid(Process uno, Process dos){
	return (uno.PPid() >  dos.PPid());	
}

static bool sortMem(Process uno, Process dos){
    return (uno.Ram() < dos.Ram());
}
static bool rsortMem(Process uno, Process dos){
    return (uno.Ram() >  dos.Ram());
}

static bool sortUser(Process uno, Process dos){
    return (uno.User() < dos.User());
}	
static bool rsortUser(Process uno, Process dos){
    return (uno.User() > dos.User());
}	

static bool sortPid(Process uno, Process dos){
    return (uno.Pid() < dos.Pid());	
}	
static bool rsortPid(Process uno, Process dos){
    return (uno.Pid() > dos.Pid());	
}	

static bool sortTime(Process uno, Process dos){
    return (uno.UpTime() < dos.UpTime());	
}	
static bool rsortTime(Process uno, Process dos){
    return (uno.UpTime() > dos.UpTime());	
}	

//sortBy SB();
  Processor& Cpu();
  void reverse();
  std::vector<Process>& Processes(); 
  float MemoryUtilization();        
  long UpTime();                     
  int TotalProcesses();              
  int RunningProcesses();          
  std::string Kernel();             
  std::string OperatingSystem();      
  void sortByCpu();
  void sortByPpid();
  void sortByPid();
  void sortByMem();
  void sortByName();
  void sortByTime();
  bool sortedByCpu();
  bool sortedByPpid();
  bool sortedByPid();
  bool sortedByMem();
  bool sortedByName();
  bool sortedByTime();
  bool ascending();
  void sortAscend();
  void sortDescend();
  // TODO: Define any necessary private members
 private:
  bool ascend{false};
  enum sortBy{CPU, MEM, PID, NAME, TIME,PPID};
  sortBy sb {CPU};
  Processor cpu_ = {};
  std::vector<Process> processes_= {};
};


	
#endif
