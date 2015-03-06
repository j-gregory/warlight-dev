#include "SuperRegion.h"


SuperRegion::SuperRegion() : num_regions(0)
{
  //ctor
}

SuperRegion::SuperRegion(int p_id, int p_reward) : num_regions(0)
{
  id = p_id;
  reward = p_reward;
}

SuperRegion::~SuperRegion()
{
  //dtor
}

void SuperRegion::addRegion(int region_id)
{
  regions.push_back(region_id);
  num_regions++;
}

void SuperRegion::print()
{
  //std::cout << "Num regions: " << num_regions << "\n";
  //std::cout << "Regions.size: " << regions.size() << "\n";
  //for (int i = 0; i < regions.size(); i++)
}

bool SuperRegion::operator< (const SuperRegion& other) const
{
  return (reward < other.getReward());
}
