#include <time.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <algorithm>
#include <unistd.h>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }



// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    //get pids
    vector<int> pids = LinuxParser::Pids(); 
    bool exists;
    std::string fn = std::to_string(cpu_.grabDeltaJiffs());
	long ajiffs;// deltajiffs;
	for (uint i=0;i<pids.size();i++){
	   exists = false;	
	   ///test to see if pid already exist in process vector.
	   for (uint j=0;j<processes_.size();j++){
	        if (pids[i] == processes_[j].Pid()) exists = true;
	   }
           if (!exists) {  ///if it doesn't, add it	
		   ///set the things that don't change once then update the process every second
	       Process p(pids[i],LinuxParser::PPID(pids[i]),LinuxParser::User(pids[i]),LinuxParser::Command(pids[i]));
	       processes_.push_back(p);
	   }
	}
    ///reap dead processes	
    for (uint i=0;i<processes_.size();i++){  
	exists = false;    
        for (uint j=0;j<pids.size();j++){
            if (pids[j] == processes_[i].Pid()) exists = true;
	    }	
           if (!exists)  processes_.erase(processes_.begin() + i);
    }
        
    /// update processes
    for (uint i=0;i<processes_.size();i++){	
	    processes_[i].SetRam(LinuxParser::Ram(processes_[i].Pid()));
        processes_[i].SetUptime(LinuxParser::UpTime(processes_[i].Pid()));
	ajiffs = LinuxParser::ActiveJiffies(processes_[i].Pid());
        //deltajiffs = cpugrabDeltaJiffs();
	long phz = (long)(sysconf(_SC_CLK_TCK));
        //long millisPerJiff = (long)(phz/10);	
        processes_[i].SetCpu(ajiffs,phz);
        processes_[i].SetState(LinuxParser::State(processes_[i].Pid()));
	long starttime = LinuxParser::UpTime(processes_[i].Pid());
	processes_[i].SetUptime(LinuxParser::UpTime() -((long)(starttime/phz)));	
    }	
    ///sort processes
       if (sb == CPU) {
	       if (ascend) std::sort(processes_.begin(), processes_.end(),sortCpu);
	       else std::sort(processes_.begin(), processes_.end(),rsortCpu);
       }       
       else if (sb == NAME) {
	       if (ascend) std::sort(processes_.begin(), processes_.end(),sortUser);
	       else std::sort(processes_.begin(), processes_.end(),rsortUser);
       }
       else if (sb == MEM) {
	       if (ascend) std::sort(processes_.begin(), processes_.end(),sortMem);		
	       else std::sort(processes_.begin(), processes_.end(),rsortMem);		
      }	       
      else if (sb == TIME) {
	      if (ascend) std::sort(processes_.begin(), processes_.end(),sortTime);
	      else  std::sort(processes_.begin(), processes_.end(),rsortTime);
      }
      else if (sb == PID) {
	      if (ascend) std::sort(processes_.begin(), processes_.end(),sortPid);	       	       
	      else std::sort(processes_.begin(), processes_.end(),rsortPid);	       	       
      }	     
      else if (sb == PPID) {
	      if (ascend) std::sort(processes_.begin(), processes_.end(),sortPpid);	       	       
	      else std::sort(processes_.begin(), processes_.end(),rsortPpid);	       	       
      } 
      return processes_;
}

//Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

//Return the system's memory utilization
float System::MemoryUtilization() {
       return LinuxParser::MemoryUtilization();	
}
//Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// Return the number of processes actively running on the system
int System::RunningProcesses() { 
   int runnin = 0;
   for (uint i=0;i<processes_.size();i++){
      if (processes_[i].state == 1) runnin++;
   }	   
   return runnin;

}
//++++++++++++++++++++++++++++++++++++++++++++
void System::sortByMem(){ 
        sb = MEM; 
}

void System::sortByPpid(){ 
        sb = PPID;         
}
void System::sortByCpu(){ 
	sb = CPU;
}
void System::sortByPid() {
       sb = PID;
}
void System::sortByName(){ 
	sb = NAME;
}
void System::sortByTime(){ 
        sb = TIME;
}
bool System::sortedByTime(){return (sb == TIME);}
bool System::sortedByPpid(){return (sb == PPID);}
bool System::sortedByMem(){ return (sb == MEM);}
bool System::sortedByCpu(){ return (sb == CPU);}
bool System::sortedByPid() { return (sb == PID);}
bool System::sortedByName(){ return (sb == NAME);}
bool System::ascending(){ return ascend;}

void System::sortAscend(){ 
	ascend = true;
}		

void System::sortDescend(){ 
	ascend = false;
}	
	

//Return the total number of processes on the system
int System::TotalProcesses() { return processes_.size(); }


// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }
