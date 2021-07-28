#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// DO: Return this process's ID
int Process::Pid() { return pid;}

// DO: Return this process's CPU utilization
float Process::CpuUtilization() { return cpuutilization;}

// DO: Return the command that generated this process
std::string Process::Command() { return command; }

// DO: Return this process's memory utilization
int Process::Ram() { return ram; }

int Process::PPid(){ return ppid;}

// DO: Return the user (name) that generated this process
std::string Process::User() { return user; }

// DO: Return the age of this process (in seconds)
long int Process::UpTime() { return uptime; }

// DO: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { 
    if (this->cpuutilization < a.cpuutilization) return true;
	else return false;
 }

  void Process::SetState(int nstate){
      state = nstate;
  }	  
  void Process::SetCpu(long njiffs, long sysjiffs){
	  long deltaj = njiffs - pjiffs;
	  pjiffs = njiffs;
	  
	  cpuutilization = (float)deltaj/(float)sysjiffs;  
  }
  
  void Process::SetRam(std::string nram){
      ram =(int)((std::stoi(nram))/1024);	  
  }
  
  void Process::SetUptime(long int nuptime){
	 uptime = nuptime; 
  }

    void Process::SetUser(std::string nuser){  
	        user = nuser;	     
	}		
  
  Process::Process(int npid, int  nppid, std::string nuser, std::string ncommand)   {
	 pid = npid;
	 ppid = nppid;
     command = ncommand;
     SetUser(nuser);	
     cpuutilization = 0;
     ram = 0;
     uptime = 0;
      
  }	

