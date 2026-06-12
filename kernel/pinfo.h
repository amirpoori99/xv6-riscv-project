#ifndef _PINFO_H_
#define _PINFO_H_

#include "types.h"

struct pinfo {
  int num_processes;
  int pids[64];
  int states[64];
  uint64 sizes[64];
  char names[64][16];
  int priorities[64];  
  int tickets[64];    
};

#endif