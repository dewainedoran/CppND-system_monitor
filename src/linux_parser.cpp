#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include "linux_parser.h"
#include "processor.h"


using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
          int pid = stoi(filename);
          pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

//Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string line;	
  string total;
  string key;
  string free;
  std::ifstream filestream(kProcDirectory+kMeminfoFilename);
  if (filestream.is_open()) {
      std::getline(filestream, line);
      std::istringstream linestream(line);
      linestream >> key >> total;
      std::getline(filestream, line);
      std::istringstream linestream2(line);
      linestream2 >> key >> free;
      return ((std::stof(total) - std::stof(free))/std::stof(total));
    } 
    return 0;
}

//Read and return the system uptime
long LinuxParser::UpTime() { 
  string line,up;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line , '.');
    std::istringstream linestream(line);
    linestream >> up;
    return std::stol(up);
  }
  return 0;
}

//Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
    string line, cpu,user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
    std::ifstream stream (kProcDirectory + kStatFilename);
    if (stream.is_open()){
       std::getline(stream, line);
       std::istringstream linestream (line);
       linestream >> cpu;
       if (cpu == "cpu") {
          linestream >> user >> nice >> system >> idle >>  iowait >> irq >> softirq >> steal >> guest >> guest_nice;
	  return std::stol(user) + std::stol(nice) + std::stol(system) +  std::stol(irq) + std::stol(softirq) + std::stol(steal);	  
       }	       
   }	  
   return 0;  
}

//Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
       string line;	
    std::ifstream stream (kProcDirectory + std::to_string(pid) + kStatFilename);
	string pidstring[17];
    if (stream.is_open()){
       std::getline(stream, line);
       std::istringstream linestream (line);
	   for (int i=0;i<17;i++){
		   linestream >> pidstring[i];
	   }
	   return std::stol(pidstring[13]) + std::stol(pidstring[14]) + std::stol(pidstring[15]) + std::stol(pidstring[16]);
    }
    return 0;
}

int  LinuxParser::PPID(int pid){
    string line, key, value;
    std::ifstream stream(kProcDirectory + std::to_string(pid)+ kStatusFilename);
    if (stream.is_open()){
        while (std::getline(stream, line)){
            std::istringstream linestream(line);
            while (linestream >> key >> value){
                if (key == "PPid:") return stoi(value);
	    }
	}		
    } 
    return 0;    
}	

//Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
string line, cpu,user, nice, system, idle, iowait, irq, softirq, steal;
    std::ifstream stream (kProcDirectory + kStatFilename);
    if (stream.is_open()){
       while (std::getline(stream, line)) {
         std::istringstream linestream (line);
         linestream >> cpu;
         if (cpu == "cpu") {
              linestream >> user >> nice >> system >> idle >>  iowait >> irq >> softirq >> steal;
              return  std::stol(user) + std::stol(nice) + std::stol(system) + std::stol(irq) + std::stol(softirq) + std::stol(steal);
          } 
       }
   }
   return 0; 
}

//Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
	string line, cpu,user, nice, system, idle, iowait, irq, softirq, steal;
    std::ifstream stream (kProcDirectory + kStatFilename);
    if (stream.is_open()){
       while (std::getline(stream, line)) {
         std::istringstream linestream (line);
         linestream >> cpu;
         if (cpu == "cpu") {
            linestream >> user >> nice >> system >> idle >>  iowait >> irq >> softirq >> steal;
	    return std::stol(idle) + std::stol(iowait);
         }
       }
   }
   return 0; 
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { return 0; }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses()  {
	return 0;
}

int LinuxParser::State(int pid){  
   string line,non,state;
   std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
   if (stream.is_open()) {
      while (std::getline(stream, line)) {
         std::istringstream linestream(line);
         linestream >> non >> non >> state;
	 if (state == "R") return 1;
	 return 0;           
      }	   
   }
   return 0;
}

//Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string line;
  std::ifstream stream(kProcDirectory +std::to_string(pid)+ kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
  }
  std::replace(line.begin(), line.end(), '\0' ,' ');//scrub embedded null characters that cause issues with .c_str() which in turn screws the command formatting in ncurses. 
  return line; 
}

// TODO: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
   std::string line,key,value;	
   std::ifstream filestream(kProcDirectory+std::to_string(pid)+kStatusFilename);
   if (filestream.is_open()){ 
       while (std::getline(filestream, line)) {
         std::istringstream linestream(line);
	     linestream >> key >> value;
	     if (key == "VmSize:") {
		     return value;
	    }     
       }
   }
   return "0";
}


//Read and return the user ID associated with a process
string LinuxParser::User(int pid) {  	
  string line;
  string key;
  string value;
  string uid;
  std::ifstream filestream(kProcDirectory+std::to_string(pid)+kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
	  while (linestream >> key >> value) {
        if (key == "Uid:") {
		  std::ifstream fstream(kPasswordPath);
		  if (fstream.is_open()){
             while(std::getline(fstream,line)){
                std::istringstream lstream(line);
		        std::getline(lstream ,uid, ':');
		        std::getline(lstream ,key, ':');//skip this token...
		        std::getline(lstream ,key, ':');
                if (key == value) return uid;		        	   	       	       
	         }
		  }	
        }
      }
    }
  }
  return "";  
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process//////////////////////////////////////////////////////////////////
long LinuxParser::UpTime(int pid) { 
	std::string stime[55];
	std::string line;
	std::ifstream filestream(kProcDirectory + std::to_string(pid)+kStatFilename);
	if (filestream.is_open()){
	    std::getline(filestream, line);
            int c=0;
	    std::istringstream linestream(line);
            while (linestream >> stime[c++]){}
            std::string r = stime[c-32];///count backwards to avoid issues with whitespace in the process name.
            return std::stol(r);
	}	
	return 0; 
}
