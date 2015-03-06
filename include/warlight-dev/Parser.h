#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include <stdio.h>
#include <string>
#include <vector>

class Bot;
class Parser
{
  Bot* bot;
  
 public:
  Parser();
  virtual ~Parser();

  /* Parser: Controller */
  void initParser(Bot* b);
  void parseInput();

  /* Parser: High-level Type */
  void parseRoundNumber();
  void parseSettings();
  void parseSetupMap();
  void parsePickStartingRegions();
  void parseUpdateMap();
  void parseOpponentMoves();
  void parseGo();

  /* Parser: setup_map arguments */
  void parseSuperRegions();
  void parseRegions();
  void parseNeighbors();
  void parseWastelands();
  void parseOpponentStartingRegions();

 protected:

 private:
  std::vector<std::string>& splitString(std::string String, std::vector<std::string>& flds, char delim);
};

#endif // PARSER_H_INCLUDED
