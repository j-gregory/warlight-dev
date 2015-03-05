#include <iostream>
#include "Bot.h"

int main(int argc, char *argv[])
{
  std::cout.sync_with_stdio(0);

  //std::cout << "Creating bot" << std::endl;

  Bot bot;
  bot.playGame();
  return 0;
}
