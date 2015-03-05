#include "SuperRegion.h"

SuperRegion::SuperRegion()
{
  //ctor
}

SuperRegion::~SuperRegion()
{
  //dtor
}

SuperRegion::SuperRegion(int p_reward)
{
  reward = p_reward;
}
void SuperRegion::addRegion(int no_region)
{
  regions.push_back(no_region);
  num_regions++;
}

int SuperRegion::getNbRegions()
{
  return regions.size();
}
