// GameView.c ... GameView ADT implementation

#include <stdlib.h>
#include <assert.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
// #include "Map.h" ... if you decide to use the Map ADT
     
struct gameView {
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    int round;
    int location;
    int turn;
    int dracHealth;
    int lordGHealth;
    int vanHealth;
    int minaHealth;
    int docHealth;
    int score;
};
     

// Creates a new GameView to summarise the current state of the game
GameView newGameView(char *pastPlays, PlayerMessage messages[])
{

    if(gameView->turn == 0) { // if the turn in the struct = 0 makes a freshhhh game.
    GameView gameView = malloc(sizeof(struct gameView));
    gameView->turn = 0;
    gameView->score = GAME_START_SCORE;
    gameView->dracHealth = GAME_START_BLOOD_POINTS;
    gameView->lordGHealth = GAME_START_HUNTER_LIFE_POINTS;
    gameView->vanHealth = GAME_START_HUNTER_LIFE_POINTS;
    gameView->minaHealth = GAME_START_HUNTER_LIFE_POINTS;
    gameView->docHealth = GAME_START_HUNTER_LIFE_POINTS;  
    
    } else {
       disposeGameView(gameView); // calls the dispose functions that will delete stuff.
       GameView gameView = malloc(sizeof(struct gameView));
       gameView->turn = getScore(gameView);
       
    }
    return gameView;     
     
// Frees all memory previously allocated for the GameView toBeDeleted
void disposeGameView(GameView toBeDeleted)
{
    //COMPLETE THIS IMPLEMENTATION
<<<<<<< HEAD
    free(toBeDeleted);
    toBeDeleted = NULL;
=======
    FEELS BAD MAN
    free( toBeDeleted );
>>>>>>> d3f793a5315eb668f82f6d7ad4467c206374d047
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

    return currentView->health;
}

// Get the current location id of a given player
LocationID getLocation(GameView currentView, PlayerID player)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    
    return currentView->location;
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
