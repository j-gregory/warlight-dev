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

void Region::setNumNeighbors(int n)
{
  num_neighbors = n;
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
  setNumNeighbors(neighbors.size());
}

void Region::print()
{
  // TODO
}

bool operator== (const Region& r1, const Region& r2)
{
  return ( (r1.neighbors == r2.neighbors) && 
	   //std::equal(r1.neighbors.begin(), r1.neighbors.end(), r2.neighbors) &&
	   (r1.id == r2.id) && 
	   (r1.num_neighbors == r2.num_neighbors) &&
	   (r1.super_region == r2.super_region) &&
	   (r1.owner == r2.owner) &&
	   (r1.armies == r2.armies) );
}



