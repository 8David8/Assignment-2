// GameView.c ... GameView ADT implementation

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"

#define FIRST_ROUND 1
#define NUM_HUNTERS 4

// below defines the number of characters that represents various data
// about a particular player and his/her move and actions
// mainly used for the pastPlay string
// read the data page for more information about Past Plays
#define NUM_CHAR_PLAYER 1
#define NUM_CHAR_NEW_LOCATION 2
#define NUM_CHAR_ENCOUNTER_HUNTER 4
#define NUM_CHAR_ENCOUNTER_DRACULA 2
#define NUM_CHAR_ACTION_DRACULA 1

// #include "Map.h" ... if you decide to use the Map ADT

typedef struct _player {
    int health;
    // LocationID is typedef'd as an int in the Places.h file
    // which is linked via the Game.h file
    LocationID location; //location of the hunters
    LocationID trail[TRAIL_SIZE]; //TRAIL_SIZE is defined in Game.h
} player;

struct gameView {
    player playerStats[NUM_PLAYERS]; // NUM_PLAYERS is defined in Globals.h
    // the trail array stores dracula last 6 moves
    // which translate to dracula's last 6 recent location,
    // moves = locationID confirmed in the rules page
    int score;
    int turns;
    int rounds;  // Can be treated as current round or number of round occurred
};

// ----------------------------------------------
// --- function prototypes that we created :D ---
// ----------------------------------------------

static PlayerID convertPlayerNameAbbrevToID(char *abbrev);

// ----------------------------------------------

// IMPORTANT: newGameView gets called once and only once
// to elaborate it gets called once in HunterView.c and DracView.c
// GameView is a pointer to the gameView struct, typedef'd in GameView.h
// Creates a new GameView to summarise the current state of the game
GameView newGameView(char *pastPlays, PlayerMessage messages[])   {
    assert(pastPlays != NULL); assert(messages != NULL);  // Check parameters are valid

    GameView gView = malloc(sizeof(struct gameView));
    assert(gView != NULL); // Check if the new gameView is initialised

    gView->score = GAME_START_SCORE;
    gView->turns = (strlen(pastPlays)+1)/8;
    gView->round = gView->turns/NUM_PLAYERS;

    // initialise all the hunter players stats
    int hunterCount;
    for (hunterCount = 0; hunterCount < NUM_HUNTERS; hunterCount++){
        gameView->playerStats[hunterCounter].health = GAME_START_HUNTER_LIFE_POINT; // sets all player health to default
    }

    // initialise Dracula's stats
    gameView->playerStats[PLAYER_DRACULA].health = GAME_START_BLOOD_POINTS; // sets dracs blood points in the beginning

    // pastPlays is a string containing a history of moves and actions
    // it represents everything that has happened so far in the game
    // Each play/turn is represented by 7 characters seperated by a space(another character)
    // the seven characters indicate what has happened during the player's turn
    // so to loop through each play/turn we increment index by 8 :D
    int index;
    for(index = 0; pastPlays[index] != '\0'; index += 8){
        // get the name abbrev for the current play and store it in a seperate array
        char playerNameAbbrev[NUM_CHAR_PLAYER+1];
        playerNameAbbrev[0] = pastPlays[index]; player[1] = '\0';

        // get the location abbrev for the current play and store it in a seperate array
        char newLocation[NUM_CHAR_NEW_LOCATION+1];
        newLocation[0] = pastPlays[index+1];
        newLocation[1] = pastPlays[index+2];
        newLocation[2] = '\0';

        PlayerID currPlayer = convertPlayerNameAbbrevToID(playerNameAbbrev);

        // current player is Dracula
        if (currPlayer == PLAYER_DRACULA){

            //injure or heal Dracula, it gets performed here


        //current player is a hunter
        } else {
             Player currHunter = currPlayer;
             //then check where the hunter is
             //if at hospital heal him
             if (currHunter->location == ST_JOSEPH_AND_ST_MARYS)
                currHunter->health = GAME_START_HUNTER_LIFE_POINT;
             //if he/she encountered Drac or a trap, injured him >:
             if (currHunter->location == )
             //if he/she died teleport him to nearest hospital
             //update current position
             //you get the point :D
        }
    }
    return gView;
}

// Frees all memory previously allocated for the GameView toBeDeleted
void disposeGameView(GameView toBeDeleted)
{

    //COMPLETE THIS IMPLEMENTATION
    free(toBeDeleted);
    toBeDeleted = NULL;
    free( toBeDeleted );
}


// ---------------------------------------------------------------------------
// --- all functions that we created and hence was not in the default file ---
// ---------------------------------------------------------------------------

static PlayerID convertPlayerNameAbbrevToID(char *abbrev)
{
    PlayerID currPlayer; int firstIndex = 0;
    switch (abbrev[firstIndex]){
        case 'G': currPlayer = PLAYER_LORD_GODALMING; break;
        case 'S': currPlayer = PLAYER_DR_SEWARD; break;
        case 'H': currPlayer = PLAYER_VAN_HELSING; break;
        case 'M': currPlayer = PLAYER_MINA_HARKER; break;
        case 'D': currPlayer = PLAYER_DRACULA; break;
        default: exit(1);
    }
    return currPlayer;
}

// ----------------------------------------------------------------------------------
// --- Functions to return simple information about the current state of the game ---
// ----------------------------------------------------------------------------------

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

// -----------------------------------------------------------------------
// --- Functions that return information about the history of the game ---
// -----------------------------------------------------------------------

// Fills the trail array with the location ids of the last 6 turns
void getHistory(GameView currentView, PlayerID player,
                            LocationID trail[TRAIL_SIZE])
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
}

// ---------------------------------------------------------------------------
// --- Functions that query the map to find information about connectivity ---
// ---------------------------------------------------------------------------

// Returns an array of LocationIDs for all directly connected locations
LocationID *connectedLocations(GameView currentView, int *numLocations,
                               LocationID from, PlayerID player, Round round,
                               int road, int rail, int sea)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return NULL;
}
