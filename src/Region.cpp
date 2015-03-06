#include "Region.h"
#include <stdio.h>
#include <iostream>
#include "SuperRegion.h"


Region::Region() : num_neighbors(0), armies(0)
{
  //ctor
}

Region::Region(int p_id, int super_region) : num_neighbors(0), armies(0)
{
  this->super_region = super_region;
  id = p_id;
}

Region::~Region()
{
  //dtor
}

void Region::setOwner(std::string o)
{
  owner = o;
}

void Region::setArmies(int num_armies)
{
  armies = num_armies;
}

void Region::addNeighbors(int n)
{
  neighbors.push_back(n);
}






