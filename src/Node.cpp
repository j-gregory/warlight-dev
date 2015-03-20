#include "Node.h"

Node::Node() : parent(NULL)
{

}

Node::Node(State s) : parent(NULL)
{
  current_state->setName(s.getName());
  current_state->setRegionsOwned(s.getRegionsOwned());
  win_percentage = 0.5;
}

Node::Node(State s, Node p)
{
  current_state->setName(s.getName());
  current_state->setRegionsOwned(s.getRegionsOwned());
  *parent = p;
  win_percentage = 0.5;
}

Node::~Node()
{

}

void Node::setCurrentState(State& s)
{
  current_state->setName(s.getName());
  current_state->setRegionsOwned(s.getRegionsOwned());
}

void Node::setParent(Node& p)
{
  // 
}

void Node::setChildren(std::vector<Node*> c)
{
  children = c;
}


void Node::setWinPercentage(float p)
{
  win_percentage = p;
}
