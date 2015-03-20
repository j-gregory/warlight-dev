#include "State.h"

State::State()
{

}

State::State(std::string n, std::vector<Region> r)
{
  name = n;
  regions_owned = r;
}

State::~State()
{

}

void State::setName(std::string n)
{
  name = n;
  // strcpy(name, n);
}

void State::setRegionsOwned(std::vector<Region> r)
{
  regions_owned = r;
}
