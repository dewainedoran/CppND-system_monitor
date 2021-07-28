#include <string>
#include <iostream>

#include "format.h"

using std::string;


std::string Format::ElapsedTime(long seconds) { 
       int ss, mm, hh;
	   hh = (int)(seconds/3600);
	   mm = (int)((seconds%3600)/60);
	   ss = (int)(seconds%60);
	   std::string fh,fm,fs;
	   if (hh < 10) fh = "0" + std::to_string(hh);
	   else fh = std::to_string(hh);
	   if (mm < 10) fm = "0" + std::to_string(mm);
	   else fm = std::to_string(mm);
	   if (ss < 10) fs = "0" + std::to_string(ss);
	   else fs = std::to_string(ss); 
	   std::string upt = fh +  ":" + fm + ":" + fs;
	   return upt;
 }
 
 std::string Format::JustLeft(std::string in, uint sz){
	 std::string pad;
        if (in.size() < sz)  {
			pad.insert(0,sz - in.size(), ' ');
            return (in +  pad );	
        } else return in.substr(0,sz);		
 }	 
 


 std::string Format::JustRight(std::string in, uint sz){
	 std::string pad;
        if (in.size() < sz)  {
			pad.insert(0,sz - in.size(), ' ');
            return ( pad + in );	
        } else return in.substr(0,sz);		
 }

	

 
 
