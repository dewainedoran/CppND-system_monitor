#include "processor.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
     long nactive = LinuxParser::ActiveJiffies();
     long nidle = LinuxParser::IdleJiffies();
     //calcdelta
     deltajiffs = nactive - active;
     long idledelta = nidle - idle;
     idle = nidle;
     active = nactive;
     totaldeltajiffs = deltajiffs + idledelta;
     return  ((float)deltajiffs/((float)deltajiffs + (float)idledelta));
}

long Processor::grabDeltaJiffs(){ return totaldeltajiffs;}
