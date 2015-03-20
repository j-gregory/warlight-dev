#ifndef __STATE_H_INCLUDED__
#define __STATE_H_INCLUDED__

//#include <stdlib.h>
#include <string>
//#include <map>
//#include <vector>

#include "Region.h"

class State
{
 public:
  State();
  ~State();

 protected:
  std::string name;
  std::vector<Region> regions_owned;

};

#endif  // __STATE_H_INCLUDED__
