#ifndef __OPPONENTBOT_H__
#define __OPPONENTBOT_H__

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <tuple>
#include <stdio.h>
#include <string>
#include <vector>

#include "Parser.h"
#include "Region.h"
#include "SuperRegion.h"
#include "MCTSManager.h"
#include "Bot.h"

using namespace std;

/*Typedefs*/
typedef tuple<int, int> SuperRegionCost;   //1st param: SuperRegiod id, 2nd param; cost left to own this superRegion
typedef std::vector< tuple<int, int, int, bool> > regionToSrMapping; //1st param: (int) - Super_Region id
                                                                    //2nd param: (int) - Region id
                                                                    //3rd param: (int) - troop in that region
                                                                    //4th param: (bool) - do we own it ?
class Bot;
class SuperRegion;

/*class OpponentBot */
class OpponentBot
{

private:

    Bot *bot;

    bool setPlayersNameStatus = false;
    bool setRegionsStatus = false;
    bool setSuperRegionsSatus = false;
    bool setNeighborsStatus = false;

    string name;
    int reinforcementThisTurn;	                        //Reinforcement guessed for opponent in this turn
    int troopsLeftThisTurn;		                        //Gets subtracted from every estimated attack / placement of the Opponent
    Region maxMomentRegion;		                        //If no delta is recognized, vote the region with most troop concentration

    std::vector<int> SuperRegionOwns;                   //Super Regions speculted to be owned by the opponent Bot
    std::vector<SuperRegionCost> SuperRegionPursueList; //1st parameter is cost of owning a superRegion, 2nd is the id of SuperRegion

    SuperRegionCost SuperRegionAttemptingNow;           //SuperRegion currently persuing from the list of eligible SuperRegions @  SuperRegionPursue
    std::vector<int> RegionsAttemptingNow;


    void simulate();            //where the actual one to many mappoing, region selection and battle simulation happens
                                //calls MCTSManager::simlateBattle via a function pointer
                                //gets called by RecalculateStrength()
    void output_list_contents(std::vector<tuple<int, int, int, bool>> &myList);
    void output_list_contents(std::vector<tuple<int, int>> &myList);


public:

    //constructors/destructors
    OpponentBot();
    //~OpponentBot();             //kq; commented, let the compiler deal with this. Detail it only when there are good reasons to.

    /*Enums*/
    typedef enum RegionOwner
    {
        PlayerBot = 1, OppoentBot=2, Neutral=3
    }RegionOwner;

    //public fields
    std::vector<tuple<int, int>> OpponentQueueRegions;
    std::vector<tuple<int, int>> OpponentTotalRegion;
    regionToSrMapping regionToSuperRegionMap;
    regionToSrMapping superRegionToRegionMap;

    /*Getters and Setters*/
    string GetName();
    void SetName(string input);

    //public methods
    void Handshake(Bot *b);     //Bot exposes itself to OpponetBot class via a pointer
    void regionToSuperRegionMapping();  //creates a tuple of regions to SuperRegions, used for quick look-up
    void Refresh();             //Is the main method to be called for update.
                                //this calculates Momentum, SuperRegions owns, cost of owning other SuperRegions it's operating in.
                                //Must make this call as minimum as possible. Perhaps before the initiation of bot UCT.
    void Refresh(int Region);
    void ObserveRegion(RegionOwner in, int RegionID);
    void AddRegion(int, int);	    //Adds a region to oppoent; is this being done already ? Refresh()
                                //call when wasn't in previous visible vis_opponent_region or dark_opponent_region
    void SubRegion(int, int);	    //Deletes a region from oponent; is this being done already ? Refresh()
                                //call when previously thought to be opponent's is found neutral or it is being annexed
    int GetReinforcement();
    int GetTroopsLeftThisTurn();
    int GetMaxMomentRegion();	//Use Region members - getNumNeighbors, getNumArmies to predict where the next attack could come from
                                //Focus on single attack where most damage could occur - loss of superRegion, choke-point, towards superRegion

    void RecieveStatus(int msg);    //Recives status from Bot, whether list of regions and Super_Regions have been recived from Engine
    std::string printStatus();      //Only for debug


    //call it in Parser::parseUpdateMap() & Parser::parseOpponentMoves
};

#endif // __OPPONENTBOT_H__
