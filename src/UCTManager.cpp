#include "UCTManager.h"

UCTManager::UCTManager() : root(NULL)
{

}

UCTManager::~UCTManager()
{

}


void UCTManager::createTree(std::string name, std::vector<Region> regions)
{
  // Create root node with current state of game
  root = new Node(State(name, regions));
}

bool UCTManager::execute(double timelimit, std::string result)
{
  // Selection
  
  // Expansion

  // Simulation

  // Back Propagation

  // Choose best move

  // Save move to <result>

  return true;
}

