// HunterView.c ... HunterView ADT implementation

#include <stdlib.h>
#include <assert.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "HunterView.h"
#include "Map.h" //... if you decide to use the Map ADT
     
#define DEATH 0
#define FIRST_ROUND 0

struct hunterView {
   GameView gState;
};
     
// Creates a new HunterView to summarise the current state of the game
HunterView newHunterView(char *pastPlays, PlayerMessage messages[])
{
    assert(pastPlays != NULL); assert(messages != NULL);

    HunterView hunterView = malloc(sizeof(struct hunterView));
    hunterView->gState = newGameView(pastPlays, messages);
    
    return hunterView;
}
    
// Frees all memory previously allocated for the HunterView toBeDeleted
void disposeHunterView(HunterView toBeDeleted)
{
    // we have to dispose the gameView gameState 
    // before we free the actual hunterView itself
    disposeGameView(toBeDeleted->gState);
    free(toBeDeleted); toBeDeleted = NULL;
}

//// Functions to return simple information about the current state of the game

// Get the current round
Round giveMeTheRound(HunterView currentView)
{
    return getRound(currentView->gState); 
}

// Get the id of current player
PlayerID whoAmI(HunterView currentView)
{
    return getCurrentPlayer(currentView->gState);
}

// Get the current score
int giveMeTheScore(HunterView currentView)
{
    return getScore(currentView->gState);
}

// Get the current health points for a given player
int howHealthyIs(HunterView currentView, PlayerID player)
{    
    return getHealth(currentView->gState, player);
}

// Get the current location id of a given player
LocationID whereIs(HunterView currentView, PlayerID player)
{
    return getLocation(currentView->gState, player);
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void giveMeTheTrail(HunterView currentView, PlayerID player,
                            LocationID trail[TRAIL_SIZE])
{
    return getHistory(currentView->gState, player, trail);
}

//// Functions that query the map to find information about connectivity

// What are my possible next moves (locations)
LocationID *whereCanIgo(HunterView currentView, int *numLocations,
                        int road, int rail, int sea)
{
    PlayerID hunter = whoAmI(currentView);
    LocationID location = whereIs(currentView, hunter);
    Round round = giveMeTheRound(currentView);
    int health = howHealthyIs(currentView, hunter);

    // if the hunters are dead, then obviously their location is at the hospital lol
    if (health == 0) {
        return connectedLocations(currentView->gState, numLocations,
                                  ST_JOSEPH_AND_ST_MARYS, hunter,
                                  round, road, rail, sea);
    }
    // if not then the hunters are at some location...
    return connectedLocations(currentView->gState, numLocations,
                              location, hunter,
                              round, road, rail, sea);
}

// What are the specified player's next possible moves
LocationID *whereCanTheyGo(HunterView currentView, int *numLocations,
                           PlayerID player, int road, int rail, int sea)
{
    LocationID location = whereIs(currentView, player);
    Round round = giveMeTheRound(currentView);

    return connectedLocations(currentView->gState, numLocations,
                              location, player,
                              round, road, rail, sea);
}
