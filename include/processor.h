#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <iostream>
#include <fstream>
#include "linux_parser.h"

class Processor {
 public:
   Processor(){
	active = 1;
	idle = 1;
	
	
   }  
  long grabDeltaJiffs(); 

  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
    long active, idle, deltajiffs, totaldeltajiffs;
};

#endif
