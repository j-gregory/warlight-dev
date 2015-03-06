#ifndef SUPERREGION_H_INCLUDED
#define SUPERREGION_H_INCLUDED

#include <vector>
#include "Region.h"

class SuperRegion
{
  int id;
  std::vector<int> regions;
  int num_regions;
  int reward;

 public:
  SuperRegion();
  SuperRegion(int p_id, int p_reward);
  //SuperRegion(int p_id, std::vector<int> p_regions, int p_reward);
  virtual ~SuperRegion();

  /* Getters */
  int getID()                   const { return id; };
  std::vector<int> getRegions() const { return regions; };
  int getNumRegions()           const { return num_regions; };
  int getReward()               const { return reward; };

  void addRegion(int no_region);  
  void print();

  bool operator< (const SuperRegion& other) const;

 protected:

 private:

};



#endif // SUPERREGION_H_INCLUDED
