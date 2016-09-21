// GameView.c ... GameView ADT implementation

#include <stdlib.h>
#include <assert.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"

#define FIRST_ROUND 1

// #include "Map.h" ... if you decide to use the Map ADT

typedef struct _player {
    int health;
    int location;
} player;

struct gameView {
   player playerStats[NUM_PLAYERS];
   int score;
   int turns;
   int round;
};



// Creates a new GameView to summarise the current state of the game
GameView newGameView(char *pastPlays, PlayerMessage messages[])   {
    assert(pastPlays != NULL); //Check inputs are valid
    assert(messages != NULL);  // Check inputs are valid
   
    if(gameView->round == 0) { // if the turn in the struct = 0 makes a freshhhh game.
    GameView gameView = malloc(sizeof(struct gameView));
    gameView->turn = 0;
    gameView->score = GAME_START_SCORE;

    } else {
       disposeGameView(gameView); // calls the dispose functions that will delete stuff.
       GameView gameView = malloc(sizeof(struct gameView));
       gameView->turn = getScore(gameView);
    }
    return gameView;
}

// Frees all memory previously allocated for the GameView toBeDeleted
void disposeGameView(GameView toBeDeleted)
{

    //COMPLETE THIS IMPLEMENTATION
    free(toBeDeleted);
    toBeDeleted = NULL;
    free( toBeDeleted );
}


//// Functions to return simple information about the current state of the game

// Get the current round
Round getRound(GameView currentView)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return currentView->round;
}

// Get the id of current player - ie whose turn is it?
PlayerID getCurrentPlayer(GameView currentView)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return currentView->turn;
}

// Get the current score
int getScore(GameView currentView)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return currentView->score;
}

// Get the current health points for a given player
int getHealth(GameView currentView, PlayerID player)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION

    return currentView->playerStats[player].health;
}

// Get the current location id of a given player
LocationID getLocation(GameView currentView, PlayerID player)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION

    return currentView->playerStats[player].location;
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void getHistory(GameView currentView, PlayerID player,
                            LocationID trail[TRAIL_SIZE])
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
}

//// Functions that query the map to find information about connectivity

// Returns an array of LocationIDs for all directly connected locations

LocationID *connectedLocations(GameView currentView, int *numLocations,
                               LocationID from, PlayerID player, Round round,
                               int road, int rail, int sea)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return NULL;
}
