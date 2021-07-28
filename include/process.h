#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid();
  long pjiffs{0};	
  int PPid(); 
  int state;                                
  std::string User();                     
  std::string Command();                   
  float CpuUtilization();                  
  int Ram();                      
  long int UpTime();                       
  bool operator<(Process const& a) const; 
  void SetState(int nstate);
  void SetUser(std::string nuser); 
  void SetCpu(long pidjiffs, long sysjiffs);	  
  void SetRam(std::string nram);
  void SetUptime(long int uptime);
  Process(int npid, int ppid, std::string nuser, std::string ncommand);	  
  
  // TODO: Declare any necessary private members
 private:
   int pid;
   int  ppid;
   std::string user;
   std::string command;
   float cpuutilization;
   int ram;
   long int uptime;
};

#endif
