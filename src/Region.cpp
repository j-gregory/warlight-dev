#include "Region.h"
#include <stdio.h>
#include <iostream>
#include "SuperRegion.h"


Region::Region()
{
  //ctor
}

Region::Region(int p_id, int super_region)
{
  this->super_region = super_region;
  id = p_id;
}

Region::~Region()
{
  //dtor
}
void Region::addNeighbors(int n)
{
  neighbors.push_back(n);
}

void Region::setArmies(int num_armies)
{
  armies = num_armies;
}
void Region::setOwner(std::string o)
{
  owner = o;
}
int Region::getArmies()
{
  return armies;
}
std::string Region::getOwner()
{
  return owner;
}

int Region::getSuperRegion()
{
  return super_region;
}

std::vector<int>& Region::getNeighbors()
{
  return neighbors;
}

